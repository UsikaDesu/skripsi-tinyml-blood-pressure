"""
AUTO-TUNE ESP32 V4: Memory-Constrained Hyperparameter Tuning untuk ESP32
Mencari model terbaik secara otomatis dari paling ringan sampai batas memori ESP32
dengan parameter lengkap yang disesuaikan dari hasil pengujian Sandbox.

Fitur: ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']
Metode Validasi: 5-Fold Stratified Group K-Fold (Strict Subject-Independent, 20 Subjek)
"""
import os
import sys
import time
import shutil
import warnings
import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import StratifiedGroupKFold, cross_val_predict
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from sklearn.pipeline import Pipeline
from joblib import Parallel, delayed
from micromlgen import port

if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')
warnings.filterwarnings('ignore')

# ==========================================
# KONFIGURASI PATH & MEMORI
# ==========================================
BASE_DIR          = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
DATA_PATH         = os.path.join(BASE_DIR, 'dataset_v4', 'extracted_features_v4.csv')

OUTPUT_HEADER_RF  = os.path.join(BASE_DIR, 'esp32_bp_inference_rf_v4', 'rf_model_v4.h')
OUTPUT_HEADER_RF_NO_OLED = os.path.join(BASE_DIR, 'esp32_bp_inference_rf_v4_no_oled', 'rf_model_v4.h')

OUTPUT_HEADER_SVM = os.path.join(BASE_DIR, 'esp32_bp_inference_svm_v4', 'svm_model_v4.h')
OUTPUT_HEADER_SVM_NO_OLED = os.path.join(BASE_DIR, 'esp32_bp_inference_svm_v4_no_oled', 'svm_model_v4.h')

OUTPUT_SCALER_SVM = os.path.join(BASE_DIR, 'esp32_bp_inference_svm_v4', 'svm_scaler_v4.h')
OUTPUT_SCALER_SVM_NO_OLED = os.path.join(BASE_DIR, 'esp32_bp_inference_svm_v4_no_oled', 'svm_scaler_v4.h')

# 4 Fitur Fisiologis PPG
FEATURES          = ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']

# Batas memori Flash ESP32 untuk model
ESP32_FLASH_LIMIT_KB = 400.0

# ==========================================
# LOAD DATASET
# ==========================================
df = pd.read_csv(DATA_PATH)
X      = df[FEATURES].values
y      = df['bp_class'].values
groups = df['subject_id'].values

sgkf = StratifiedGroupKFold(n_splits=5, shuffle=True, random_state=42)
splits = list(sgkf.split(X, y, groups))

# Pra-kalkulasi fold data untuk SVM (Z-score scaling per fold bebas data leakage)
svm_folds_data = []
for train_idx, test_idx in splits:
    scaler = StandardScaler()
    X_tr_sc = scaler.fit_transform(X[train_idx])
    X_te_sc = scaler.transform(X[test_idx])
    svm_folds_data.append((X_tr_sc, y[train_idx], X_te_sc, y[test_idx]))

# ==========================================
# FUNGSI EVALUASI WORKER PARALEL
# ==========================================
def eval_svm_worker(cfg):
    tr_accs, te_accs = [], []
    clf = SVC(
        kernel=cfg['kernel'],
        C=cfg['C'],
        gamma=cfg.get('gamma', 'scale'),
        degree=cfg.get('degree', 3),
        class_weight='balanced',
        random_state=42
    )
    for X_tr, y_tr, X_te, y_te in svm_folds_data:
        clf.fit(X_tr, y_tr)
        tr_accs.append(accuracy_score(y_tr, clf.predict(X_tr)) * 100)
        te_accs.append(accuracy_score(y_te, clf.predict(X_te)) * 100)
    
    cv_mean = np.mean(te_accs)
    cv_std  = np.std(te_accs)
    tr_mean = np.mean(tr_accs)
    gap     = tr_mean - cv_mean
    lb      = cv_mean - cv_std
    
    # Cek ukuran model jika lolos ambang awal
    size_kb = 25.0
    try:
        scaler_full = StandardScaler()
        X_full_sc = scaler_full.fit_transform(X)
        clf.fit(X_full_sc, y)
        cpp_code = port(clf)
        size_kb = len(cpp_code) / 1024.0
    except Exception:
        size_kb = 9999.0

    fits = (size_kb <= ESP32_FLASH_LIMIT_KB)
    
    return {
        'type': 'SVM',
        'name': cfg['name'],
        'n_trees': None,
        'max_depth': None,
        'max_leaf_nodes': None,
        'min_samples_leaf': None,
        'criterion': None,
        'max_features': None,
        'kernel': cfg['kernel'],
        'C': cfg['C'],
        'gamma': cfg.get('gamma', None),
        'degree': cfg.get('degree', None),
        'train_acc': tr_mean,
        'cv_mean': cv_mean,
        'cv_std': cv_std,
        'gap': gap,
        'lower_bound': lb,
        'scores': te_accs,
        'size_kb': size_kb,
        'fits': fits
    }

def eval_rf_worker(nt, d, ln, ml, crit, mf):
    tr_accs, te_accs = [], []
    clf = RandomForestClassifier(
        n_estimators=nt,
        max_depth=d,
        max_leaf_nodes=ln,
        min_samples_leaf=ml,
        criterion=crit,
        max_features=mf,
        class_weight='balanced',
        random_state=42
    )
    for tr_idx, te_idx in splits:
        clf.fit(X[tr_idx], y[tr_idx])
        tr_accs.append(accuracy_score(y[tr_idx], clf.predict(X[tr_idx])) * 100)
        te_accs.append(accuracy_score(y[te_idx], clf.predict(X[te_idx])) * 100)
        
    cv_mean = np.mean(te_accs)
    cv_std  = np.std(te_accs)
    tr_mean = np.mean(tr_accs)
    gap     = tr_mean - cv_mean
    lb      = cv_mean - cv_std
    
    # Hitung ukuran model via porting jika memiliki performa kompetitif
    size_kb = 15.0
    if lb >= 65.0:
        try:
            clf.fit(X, y)
            cpp_code = port(clf)
            size_kb = len(cpp_code) / 1024.0
        except Exception:
            size_kb = 9999.0
            
    fits = (size_kb <= ESP32_FLASH_LIMIT_KB)
    
    d_str  = f"d{d}" if d is not None else "dInf"
    ln_str = f"L{ln}" if ln is not None else "LInf"
    name   = f"RF-{nt}t-{d_str}-{ln_str}-min{ml}-mf{mf}-{crit[:4]}"
    
    return {
        'type': 'RF',
        'name': name,
        'n_trees': nt,
        'max_depth': d,
        'max_leaf_nodes': ln,
        'min_samples_leaf': ml,
        'criterion': crit,
        'max_features': mf,
        'kernel': None,
        'C': None,
        'gamma': None,
        'degree': None,
        'train_acc': tr_mean,
        'cv_mean': cv_mean,
        'cv_std': cv_std,
        'gap': gap,
        'lower_bound': lb,
        'scores': te_accs,
        'size_kb': size_kb,
        'fits': fits
    }

def main():
    print("=" * 80)
    print("AUTO-TUNE ESP32 V4: OPTIMALISASI HYPERPARAMETER LENGKAP (5-FOLD SGKF)")
    print("=" * 80)
    print(f"Dataset      : {len(df)} jendela (20 Subjek Independen)")
    print(f"Fitur        : {FEATURES}")
    print(f"Batas Flash  : {ESP32_FLASH_LIMIT_KB} KB")
    print(f"Distribusi   : {(y==0).sum()} Normal / {(y==1).sum()} Prehipertensi\n")

    # 1. Konfigurasi SVM
    svm_configs = []
    for kernel in ['linear', 'rbf', 'poly', 'sigmoid']:
        for C in [0.01, 0.1, 1.0, 5.0, 10.0, 20.0, 30.0, 50.0, 100.0]:
            if kernel == 'linear':
                svm_configs.append({'name': f"SVM-LIN-C{C}", 'kernel': 'linear', 'C': C})
            else:
                for gamma in [0.001, 0.01, 0.05, 0.1, 0.25, 0.5, 1.0, 2.0]:
                    if kernel == 'poly':
                        for degree in [2, 3]:
                            svm_configs.append({'name': f"SVM-POL-C{C}-d{degree}-g{gamma}", 'kernel': 'poly', 'C': C, 'gamma': gamma, 'degree': degree})
                    else:
                        k_name = kernel[:3].upper()
                        svm_configs.append({'name': f"SVM-{k_name}-C{C}-g{gamma}", 'kernel': kernel, 'C': C, 'gamma': gamma})

    # 2. Konfigurasi Random Forest
    n_trees_list   = [10, 15, 20, 25, 30]
    max_depth_list = [3, 4, 5, 6, 8, None]
    max_leaf_list  = [6, 8, 10, 12, 16, None]
    min_leaf_list  = [1, 2, 4, 6, 8, 10]
    crit_list      = ['gini', 'entropy']
    max_feat_list  = [1, 2, 3, 4]

    rf_tasks = []
    for nt in n_trees_list:
        for d in max_depth_list:
            for ln in max_leaf_list:
                if d is not None and ln is not None and ln >= 2**d:
                    continue
                for ml in min_leaf_list:
                    for crit in crit_list:
                        for mf in max_feat_list:
                            rf_tasks.append((nt, d, ln, ml, crit, mf))

    total_configs = len(svm_configs) + len(rf_tasks)
    print(f"Total konfigurasi yang diuji: {total_configs} model")
    print(f"  - SVM Models : {len(svm_configs)}")
    print(f"  - RF Models  : {len(rf_tasks)}")
    print(f"Memulai komputasi paralel...\n")

    t_start = time.time()

    # Eksekusi paralel
    svm_res = Parallel(n_jobs=-1, batch_size=32)(delayed(eval_svm_worker)(cfg) for cfg in svm_configs)
    rf_res  = Parallel(n_jobs=-1, batch_size=64)(delayed(eval_rf_worker)(*task) for task in rf_tasks)

    results = svm_res + rf_res
    total_time = time.time() - t_start
    print(f"Selesai komputasi seluruh {len(results)} model dalam {total_time:.1f} detik ({total_time/60:.2f} menit).\n")

    df_results = pd.DataFrame(results)
    df_fits = df_results[df_results['fits']].sort_values(by=['lower_bound', 'cv_mean', 'gap'], ascending=[False, False, True]).reset_index(drop=True)
    df_rf_fits = df_fits[df_fits['type'] == 'RF'].reset_index(drop=True)
    df_svm_fits = df_fits[df_fits['type'] == 'SVM'].reset_index(drop=True)

    # ==========================================
    # TAMPILKAN 3 TABEL RANKING LENGKAP
    # ==========================================
    def print_ranking_table(title, df_subset, top_n=10):
        print("=" * 115)
        print(f"{title} (Top {top_n} diurutkan berdasar Lower Bound)")
        print("=" * 115)
        print(f"{'Rank':>4} {'Tipe':<5} {'Model Name':<32} {'Train':>7} {'Test':>7} {'Gap':>6} {'Std':>6} {'L-Bound':>8} {'Scores':<32} {'Ukuran':>8}")
        print("-" * 115)
        for rank, (_, row) in enumerate(df_subset.head(top_n).iterrows(), 1):
            folds_str = "[" + ", ".join([f"{s:.1f}" for s in row['scores']]) + "]"
            print(f"{rank:>4} {row['type']:<5} {row['name']:<32} {row['train_acc']:>6.1f}% {row['cv_mean']:>6.1f}% {row['gap']:>5.1f}% {row['cv_std']:>5.1f}% {row['lower_bound']:>7.1f}% {folds_str:<32} {row['size_kb']:>6.1f} KB")
        print("=" * 115 + "\n")

    print_ranking_table("TABEL PARETO: KESELURUHAN (GABUNGAN RF & SVM)", df_fits, top_n=10)
    print_ranking_table("TABEL PERINGKAT: KHUSUS RANDOM FOREST (RF)", df_rf_fits, top_n=10)
    print_ranking_table("TABEL PERINGKAT: KHUSUS SUPPORT VECTOR MACHINE (SVM)", df_svm_fits, top_n=10)

    # ==========================================
    # DEPLOY PEMENANG RANDOM FOREST & SVM
    # ==========================================
    best_rf  = df_rf_fits.iloc[0] if len(df_rf_fits) > 0 else None
    best_svm = df_svm_fits.iloc[0] if len(df_svm_fits) > 0 else None

    if best_rf is not None:
        clf_rf = RandomForestClassifier(
            n_estimators=int(best_rf['n_trees']),
            max_depth=int(best_rf['max_depth']) if pd.notna(best_rf['max_depth']) else None,
            max_leaf_nodes=int(best_rf['max_leaf_nodes']) if pd.notna(best_rf['max_leaf_nodes']) else None,
            min_samples_leaf=int(best_rf['min_samples_leaf']),
            criterion=best_rf['criterion'],
            max_features=int(best_rf['max_features']),
            class_weight='balanced',
            random_state=42
        )
        clf_rf.fit(X, y)
        cpp_code_rf = port(clf_rf)
        
        for out_path in [OUTPUT_HEADER_RF, OUTPUT_HEADER_RF_NO_OLED]:
            os.makedirs(os.path.dirname(out_path), exist_ok=True)
            with open(out_path, 'w', encoding='utf-8') as f:
                f.write(cpp_code_rf)
            print(f"[DEPLOY RF] Model RF berhasil diekspor ke: {out_path} ({len(cpp_code_rf)/1024.0:.1f} KB)")

        y_pred_rf = cross_val_predict(clf_rf, X, y, groups=groups, cv=sgkf)
        print(f"\n{'='*70}")
        print(f"[WINNER RF] {best_rf['name']}")
        print(f"   Akurasi CV  : {best_rf['cv_mean']:.1f}% +/- {best_rf['cv_std']:.1f}%")
        print(f"   Precision   : {precision_score(y, y_pred_rf)*100:.1f}%")
        print(f"   Recall      : {recall_score(y, y_pred_rf)*100:.1f}%")
        print(f"   F1-Score    : {f1_score(y, y_pred_rf)*100:.1f}%")
        print(f"   Lower Bound : {best_rf['lower_bound']:.1f}%")
        print(f"   Ukuran C++  : {len(cpp_code_rf)/1024.0:.1f} KB")
        print(f"{'='*70}\n")

    if best_svm is not None:
        clf_svm = SVC(
            kernel=best_svm['kernel'],
            C=best_svm['C'],
            gamma=best_svm['gamma'] if best_svm['gamma'] is not None else 'scale',
            degree=int(best_svm['degree']) if pd.notna(best_svm['degree']) else 3,
            class_weight='balanced',
            random_state=42
        )
        scaler_full = StandardScaler()
        X_sc = scaler_full.fit_transform(X)
        clf_svm.fit(X_sc, y)
        cpp_code_svm = port(clf_svm)

        if "sigmoid(" in cpp_code_svm and "inline float sigmoid" not in cpp_code_svm:
            sigmoid_def = "#include <math.h>\n\ninline float sigmoid(float x) {\n    return 1.0f / (1.0f + exp(-x));\n}\n\n"
            cpp_code_svm = cpp_code_svm.replace("#include <cstdarg>", "#include <cstdarg>\n" + sigmoid_def)

        for out_path in [OUTPUT_HEADER_SVM, OUTPUT_HEADER_SVM_NO_OLED]:
            os.makedirs(os.path.dirname(out_path), exist_ok=True)
            with open(out_path, 'w', encoding='utf-8') as f:
                f.write(cpp_code_svm)
            print(f"[DEPLOY SVM] Model SVM berhasil diekspor ke: {out_path} ({len(cpp_code_svm)/1024.0:.1f} KB)")

        means = scaler_full.mean_
        scales = scaler_full.scale_
        scaler_header = "// AUTO-GENERATED BY auto_tune_esp32_v4.py\n"
        scaler_header += "#ifndef SVM_SCALER_V4_H\n#define SVM_SCALER_V4_H\n\n"
        scaler_header += f"const float FEATURE_MEANS[4] = {{{means[0]:.8f}f, {means[1]:.8f}f, {means[2]:.8f}f, {means[3]:.8f}f}};\n"
        scaler_header += f"const float FEATURE_SCALES[4] = {{{scales[0]:.8f}f, {scales[1]:.8f}f, {scales[2]:.8f}f, {scales[3]:.8f}f}};\n\n"
        scaler_header += "#endif\n"

        for out_sc in [OUTPUT_SCALER_SVM, OUTPUT_SCALER_SVM_NO_OLED]:
            with open(out_sc, 'w', encoding='utf-8') as f:
                f.write(scaler_header)
            print(f"[DEPLOY SCALER] Parameter Z-Score diekspor ke: {out_sc}")

    # ==========================================
    # EKSPOR KE CSV
    # ==========================================
    csv_full = os.path.join(os.path.dirname(__file__), 'auto_tune_full_results_v4.csv')
    csv_pareto = os.path.join(os.path.dirname(__file__), 'auto_tune_pareto_v4.csv')
    df_results.to_csv(csv_full, index=False)
    df_fits.to_csv(csv_pareto, index=False)
    print(f"\n[EKSPOR] Seluruh {len(df_results)} hasil diekstrak ke: {csv_full}")
    print(f"[EKSPOR] Tabel Pareto ({len(df_fits)} model) diekstrak ke: {csv_pareto}")
    print("\nProses Auto-Tune ESP32 V4 Selesai dengan sukses!")

if __name__ == '__main__':
    main()
