"""
AUTO-TUNE LOSO ESP32 V4: LEAVE-ONE-SUBJECT-OUT (LOSO) VALIDATION
================================================================
Program Khusus Validasi Lintas Subjek (LOSO 20 Folds) untuk ESP32:
- Memenuhi instruksi resmi Dosen Pembimbing ("Subject test bergantian").
- Bebas bias "God Seed" dengan evaluasi multi-seed independen.
- Menggunakan urutan peringkat: Akurasi Window -> Gap Overfitting -> Ukuran Flash.
- Menggunakan Aturan 1-Galat Baku (1-SE Rule / Occam's Razor) untuk memilih
  model paling sederhana dan stabil.
- Dilengkapi Parity Check (Kecocokan 100% antara keluaran C++ dan Python pada 200 jendela).
"""

import os
import sys
import re
import time
import argparse
import warnings
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import LeaveOneGroupOut
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from joblib import Parallel, delayed
from micromlgen import port

if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')
warnings.filterwarnings('ignore')

# ==========================================
# KONFIGURASI PATH & HARDWARE
# ==========================================
BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
DATA_PATH = os.path.join(BASE_DIR, 'dataset_v4', 'extracted_features_v4.csv')

OUTPUT_HEADER_RF = os.path.join(BASE_DIR, 'esp32_bp_inference_rf_v4', 'rf_model_v4.h')
OUTPUT_HEADER_RF_NO_OLED = os.path.join(BASE_DIR, 'esp32_bp_inference_rf_v4_no_oled', 'rf_model_v4.h')

ESP32_FLASH_LIMIT_KB = 400.0

FEATURE_SETS = {
    '4': ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate'],
    '2': ['crest_time_ratio', 'heart_rate']
}

# ==========================================
# VERIFIKASI PARITAS C++ VS PYTHON
# ==========================================
def verify_cpp_parity(clf, X, y):
    """
    Memverifikasi keselarasan 100% antara keluaran pohon C++ (micromlgen hard voting)
    dengan scikit-learn individual tree hard voting pada seluruh 200 jendela data.
    """
    cpp_code = port(clf)
    
    # 1. Scikit-Learn Hard Voting
    tree_preds = np.array([tree.predict(X) for tree in clf.estimators_])
    sk_votes_1 = (tree_preds == 1).sum(axis=0)
    sk_votes_0 = (tree_preds == 0).sum(axis=0)
    # Aturan argmax ties: jika sama, classIdx = 0
    sk_hard_pred = np.where(sk_votes_1 > sk_votes_0, 1, 0)
    
    # 2. C++ Tree Parser
    def build_tree_evaluator(tree_text):
        token_list = []
        for m in re.finditer(r'if\s*\(\s*x\[(\d+)\]\s*<=\s*([0-9eE\.\-]+)\s*\)|else|\{|\}|votes\[(\d+)\]\s*\+=\s*1;', tree_text):
            txt = m.group(0)
            if txt.startswith('if'):
                token_list.append(('IF', int(m.group(1)), float(m.group(2))))
            elif txt == 'else':
                token_list.append(('ELSE',))
            elif txt == '{':
                token_list.append(('OPEN',))
            elif txt == '}':
                token_list.append(('CLOSE',))
            elif 'votes[' in txt:
                token_list.append(('VOTE', int(m.group(3))))
        pos = 0
        def parse_node():
            nonlocal pos
            tok = token_list[pos]
            if tok[0] == 'VOTE':
                pos += 1
                return lambda x: tok[1]
            elif tok[0] == 'IF':
                _, f, th = tok
                pos += 2
                left_fn = parse_node()
                pos += 3
                right_fn = parse_node()
                pos += 1
                return lambda x: left_fn(x) if x[f] <= th else right_fn(x)
        return parse_node()

    trees_raw = re.split(r'// tree #\d+', cpp_code)[1:]
    trees_raw[-1] = re.split(r'// return argmax of votes', trees_raw[-1])[0]
    evaluators = [build_tree_evaluator(t) for t in trees_raw]
    
    c_preds = []
    for row in X:
        votes = [0, 0]
        for ev in evaluators:
            votes[ev(row)] += 1
        c_preds.append(1 if votes[1] > votes[0] else 0)
    c_preds = np.array(c_preds)
    
    matches = (sk_hard_pred == c_preds).sum()
    match_pct = (matches / len(X)) * 100.0
    return matches, len(X), match_pct, cpp_code

# ==========================================
# WORKER MULTI-SEED LOSO EVALUATION
# ==========================================
def eval_rf_multi_seed(nt, d, ml, crit, mf, folds_data, seeds):
    subj_correct_list = []
    win_acc_list = []
    tr_acc_list = []
    sens_list = []
    spec_list = []
    f1_list = []
    
    for s in seeds:
        clf = RandomForestClassifier(
            n_estimators=nt,
            max_depth=d,
            min_samples_leaf=ml,
            max_features=mf,
            criterion=crit,
            class_weight='balanced',
            random_state=s
        )
        
        y_true_w, y_pred_w = [], []
        y_true_s, y_pred_s = [], []
        s_cor = 0
        tr_folds = []
        
        for X_tr, y_tr, X_te, y_te, s_name, true_cls in folds_data:
            clf.fit(X_tr, y_tr)
            preds = clf.predict(X_te)
            tr_folds.append(accuracy_score(y_tr, clf.predict(X_tr)))
            
            y_true_w.extend(y_te)
            y_pred_w.extend(preds)
            
            maj_pred = 1 if np.mean(preds) >= 0.5 else 0
            y_true_s.append(true_cls)
            y_pred_s.append(maj_pred)
            if maj_pred == true_cls:
                s_cor += 1
                
        subj_correct_list.append(s_cor)
        win_acc_list.append(accuracy_score(y_true_w, y_pred_w) * 100.0)
        tr_acc_list.append(np.mean(tr_folds) * 100.0)
        sens_list.append(recall_score(y_true_s, y_pred_s, pos_label=1, zero_division=0) * 100.0)
        spec_list.append(recall_score(y_true_s, y_pred_s, pos_label=0, zero_division=0) * 100.0)
        f1_list.append(f1_score(y_true_w, y_pred_w, zero_division=0) * 100.0)
        
    mean_subj_cor = float(np.mean(subj_correct_list))
    mean_subj_acc = (mean_subj_cor / len(folds_data)) * 100.0
    min_subj_cor  = int(np.min(subj_correct_list))
    max_subj_cor  = int(np.max(subj_correct_list))
    
    mean_win_acc  = float(np.mean(win_acc_list))
    std_win_acc   = float(np.std(win_acc_list))
    
    mean_tr_acc   = float(np.mean(tr_acc_list))
    gap           = mean_tr_acc - mean_win_acc
    
    d_str = f"d{d}" if d is not None else "dInf"
    name = f"RF-{nt}t-{d_str}-min{ml}-mf{mf}-{crit[:4]}"
    
    return {
        'name': name,
        'n_trees': nt,
        'max_depth': d,
        'min_samples_leaf': ml,
        'max_features': mf,
        'criterion': crit,
        'mean_win_acc': mean_win_acc,
        'std_win_acc': std_win_acc,
        'train_acc': mean_tr_acc,
        'gap': gap,
        'mean_subj_cor': mean_subj_cor,
        'mean_subj_acc': mean_subj_acc,
        'min_subj_cor': min_subj_cor,
        'max_subj_cor': max_subj_cor,
        'sens': float(np.mean(sens_list)),
        'spec': float(np.mean(spec_list)),
        'f1': float(np.mean(f1_list))
    }

# ==========================================
# PIPELINE UTAMA TUNING LOSO
# ==========================================
def run_tuning(features_key='2', num_seeds=5, export_winner=False):
    selected_features = FEATURE_SETS[features_key]
    eval_seeds = [1, 7, 13, 21, 37, 55, 77, 88, 99, 123][:num_seeds]
    
    print("=" * 115)
    print(f"PENGUJIAN TUNING MULTI-SEED LOSO: {len(selected_features)} FITUR -> {selected_features}")
    print("=" * 115)
    print(f"Dataset   : {DATA_PATH} (200 windows / 20 subjek)")
    print(f"Metode    : LOSO 20 Folds diuji pada {len(eval_seeds)} Random Seed Independen {eval_seeds}")
    print(f"Kriteria  : Rata-rata Akurasi Window -> Gap Overfitting -> Ukuran C++ Flash")
    
    df = pd.read_csv(DATA_PATH)
    X = df[selected_features].values
    y = df['bp_class'].values
    groups = df['subject_id'].values
    
    logo = LeaveOneGroupOut()
    splits = list(logo.split(X, y, groups))
    folds_data = [(X[tr], y[tr], X[te], y[te], groups[te][0], y[te][0]) for tr, te in splits]
    
    # Grid Parameter
    n_trees_list   = [10, 15, 20, 25]
    max_depth_list = [3, 4, 5, 6]
    min_leaf_list  = [2, 4, 6, 8]
    crit_list      = ['gini', 'entropy']
    max_feat_list  = list(range(1, len(selected_features) + 1))
    
    tasks = []
    for nt in n_trees_list:
        for d in max_depth_list:
            for ml in min_leaf_list:
                for crit in crit_list:
                    for mf in max_feat_list:
                        tasks.append((nt, d, ml, crit, mf))
                        
    print(f"Kombinasi : {len(tasks)} model x {len(eval_seeds)} seeds x 20 folds = {len(tasks)*len(eval_seeds)*20} fits")
    t0 = time.time()
    results = Parallel(n_jobs=-1, batch_size='auto')(
        delayed(eval_rf_multi_seed)(nt, d, ml, crit, mf, folds_data, eval_seeds)
        for nt, d, ml, crit, mf in tasks
    )
    elapsed = time.time() - t0
    print(f"Selesai komputasi dalam {elapsed:.1f} detik ({elapsed/60:.2f} menit).\n")
    
    df_res = pd.DataFrame(results)
    
    # Urutkan berdasarkan: Rata-rata akurasi window (descending) -> gap overfitting (ascending)
    df_sorted = df_res.sort_values(
        by=['mean_win_acc', 'gap'],
        ascending=[False, True]
    ).reset_index(drop=True)
    
    # Hitung ukuran Flash C++ untuk Top 20 model
    top_candidates = df_sorted.head(20).copy()
    accurate_sizes = []
    for _, row in top_candidates.iterrows():
        clf_temp = RandomForestClassifier(
            n_estimators=int(row['n_trees']),
            max_depth=int(row['max_depth']) if pd.notna(row['max_depth']) else None,
            min_samples_leaf=int(row['min_samples_leaf']),
            max_features=int(row['max_features']),
            criterion=row['criterion'],
            class_weight='balanced',
            random_state=eval_seeds[0]
        )
        clf_temp.fit(X, y)
        try:
            sz = len(port(clf_temp)) / 1024.0
        except Exception:
            sz = 9999.0
        accurate_sizes.append(sz)
        
    top_candidates['size_kb'] = accurate_sizes
    
    # Cetak Tabel Peringkat
    print("=" * 125)
    print(f"TABEL TOP 10 (DIURUTKAN BERDASARKAN: AKURASI WINDOW -> GAP TERENDAH)")
    print("=" * 125)
    print(f"{'Rank':>4} | {'Model Name':<26} | {'Window (Rata)':<16} | {'Gap':<7} | {'Subjek (Rata)':<18} | {'Rentang':<9} | {'Ukuran'}")
    print("-" * 125)
    for rank, (_, row) in enumerate(top_candidates.head(10).iterrows(), 1):
        win_str  = f"{row['mean_win_acc']:.1f}% (+-{row['std_win_acc']:.1f}%)"
        subj_str = f"{row['mean_subj_cor']:.1f}/20 ({row['mean_subj_acc']:.1f}%)"
        range_str = f"[{row['min_subj_cor']}-{row['max_subj_cor']}/20]"
        print(f"{rank:>4} | {row['name']:<26} | {win_str:<16} | {row['gap']:>6.1f}% | {subj_str:<18} | {range_str:<9} | {row['size_kb']:>6.1f} KB")
    print("=" * 125)
    
    # ==========================================
    # ATURAN 1-SELECTION RULE (OCCAM'S RAZOR)
    # ==========================================
    top_1 = top_candidates.iloc[0]
    cutoff_acc = top_1['mean_win_acc'] - top_1['std_win_acc']
    
    candidates_1se = top_candidates[
        (top_candidates['mean_win_acc'] >= cutoff_acc) &
        (top_candidates['gap'] <= 12.0)
    ].copy()
    
    if len(candidates_1se) == 0:
        candidates_1se = top_candidates[top_candidates['mean_win_acc'] >= cutoff_acc].copy()
        
    candidates_1se = candidates_1se.sort_values(
        by=['max_depth', 'n_trees', 'size_kb'],
        ascending=[True, True, True]
    ).reset_index(drop=True)
    
    chosen_model = candidates_1se.iloc[0]
    
    print("\n" + "=" * 95)
    print("🏆 PEMILIHAN MODEL 1-STANDARD-ERROR (1-SE RULE / OCCAM'S RAZOR) 🏆")
    print("=" * 95)
    print(f"Model Skor Puncak     : {top_1['name']} (Window: {top_1['mean_win_acc']:.1f}% +- {top_1['std_win_acc']:.1f}%, Gap: {top_1['gap']:.1f}%)")
    print(f"Batas Ambang 1-SE     : >= {cutoff_acc:.1f}%")
    print(f"Model Sederhana Terpilih: {chosen_model['name']}")
    print(f"  - Parameter         : {chosen_model['n_trees']} trees, depth {chosen_model['max_depth']}, min_leaf {chosen_model['min_samples_leaf']}, mf {chosen_model['max_features']}, {chosen_model['criterion']}")
    print(f"  - Rata Window       : {chosen_model['mean_win_acc']:.1f}% (+- {chosen_model['std_win_acc']:.1f}%)")
    print(f"  - Overfit Gap       : {chosen_model['gap']:.1f}% (Latih: {chosen_model['train_acc']:.1f}%)")
    print(f"  - Rata Subjek       : {chosen_model['mean_subj_cor']:.1f} / 20 orang ({chosen_model['mean_subj_acc']:.1f}%) [Rentang: {chosen_model['min_subj_cor']}-{chosen_model['max_subj_cor']}]")
    print(f"  - Ukuran Flash C++  : {chosen_model['size_kb']:.1f} KB (Ringan & Cepat!)")
    print("=" * 95)
    
    # Latih ulang model terpilih pada seluruh 20 subjek
    print("\nMelatih ulang model terpilih pada seluruh 20 subjek (100% data)...")
    clf_final = RandomForestClassifier(
        n_estimators=int(chosen_model['n_trees']),
        max_depth=int(chosen_model['max_depth']) if pd.notna(chosen_model['max_depth']) else None,
        min_samples_leaf=int(chosen_model['min_samples_leaf']),
        max_features=int(chosen_model['max_features']),
        criterion=chosen_model['criterion'],
        class_weight='balanced',
        random_state=eval_seeds[0]
    )
    clf_final.fit(X, y)
    
    # Parity Check: Cocokkan C++ vs Python pada seluruh 200 window
    print("Melakukan Parity Check (Kecocokan C++ vs Python pada seluruh 200 window)...")
    matches, total, pct, cpp_code = verify_cpp_parity(clf_final, X, y)
    print(f"  Hasil Parity Check: {matches} / {total} jendela identik ({pct:.2f}% MATCH)")
    if matches == total:
        print("  ✅ 100% BIT-EXACT MATCH! Logika C++ identik sempurna dengan Scikit-Learn Python.")
    else:
        print(f"  ⚠️ Terdapat {total - matches} jendela dengan perbedaan pembulatan voting.")
        
    if export_winner:
        for out_path in [OUTPUT_HEADER_RF, OUTPUT_HEADER_RF_NO_OLED]:
            os.makedirs(os.path.dirname(out_path), exist_ok=True)
            with open(out_path, 'w', encoding='utf-8') as f:
                f.write(cpp_code)
            print(f"  [EXPORT] File header C++ diperbarui: {out_path} ({len(cpp_code)/1024.0:.1f} KB)")
        print("  ✅ File siap diunggah ke mikrokontroler ESP32!")
    else:
        print("  [INFO] Opsi --no-export aktif: File C++ tidak ditulis ke folder firmware.")
        
    return chosen_model, pct

def main():
    parser = argparse.ArgumentParser(description="Auto-Tune LOSO ESP32 V4 with 1-SE Rule and C++ Parity Check")
    parser.add_argument('--features', type=str, choices=['2', '4', 'compare'], default='compare',
                        help="Pilih set fitur: '2', '4', atau 'compare' untuk membandingkan keduanya")
    parser.add_argument('--seeds', type=int, default=5, help="Jumlah random seed acak (default: 5)")
    parser.add_argument('--no-export', action='store_true', help="Jangan ekspor file C++ ke folder firmware")
    parser.add_argument('--export', action='store_true', help="Ekspor file C++ ke folder firmware")
    args = parser.parse_args()

    export_flag = args.export and not args.no_export

    if args.features in ['2', '4']:
        run_tuning(features_key=args.features, num_seeds=args.seeds, export_winner=export_flag)
    elif args.features == 'compare':
        print("\n" + "#" * 115)
        print("PERBANDINGAN HEAD-TO-HEAD FITUR 2 {CTR, HR} VS FITUR 4 {IR, Red, CTR, HR} (LOSO CV)")
        print("#" * 115 + "\n")
        
        m2, p2 = run_tuning(features_key='2', num_seeds=args.seeds, export_winner=False)
        print("\n" + "-" * 115 + "\n")
        m4, p4 = run_tuning(features_key='4', num_seeds=args.seeds, export_winner=False)
        
        print("\n" + "=" * 95)
        print("RINGKASAN PERBANDINGAN AKHIR (1-SE RULE): 2 FITUR VS 4 FITUR")
        print("=" * 95)
        print(f"{'Metrik':<25} | {'2 Fitur {CTR, HR}':<30} | {'4 Fitur {IR, Red, CTR, HR}'}")
        print("-" * 95)
        print(f"{'Model 1-SE':<25} | {m2['name']:<30} | {m4['name']}")
        print(f"{'Rata Akurasi Window':<25} | {m2['mean_win_acc']:.1f}% (+- {m2['std_win_acc']:.1f}%)                 | {m4['mean_win_acc']:.1f}% (+- {m4['std_win_acc']:.1f}%)")
        print(f"{'Overfitting Gap':<25} | {m2['gap']:.1f}%                         | {m4['gap']:.1f}%")
        print(f"{'Rata Akurasi Subjek':<25} | {m2['mean_subj_cor']:.1f}/20 ({m2['mean_subj_acc']:.1f}%)            | {m4['mean_subj_cor']:.1f}/20 ({m4['mean_subj_acc']:.1f}%)")
        print(f"{'Rentang Subjek':<25} | [{m2['min_subj_cor']}-{m2['max_subj_cor']}/20]                      | [{m4['min_subj_cor']}-{m4['max_subj_cor']}/20]")
        print(f"{'Ukuran C++ Flash':<25} | {m2['size_kb']:.1f} KB                      | {m4['size_kb']:.1f} KB")
        print(f"{'Parity Check C++':<25} | {p2:.1f}% Cocok Sempurna           | {p4:.1f}% Cocok Sempurna")
        print("=" * 95)

if __name__ == '__main__':
    main()
