"""
SANDBOX: Memory-Constrained AutoML untuk ESP32
Mencari model terbaik secara otomatis dari paling ringan sampai batas memori ESP32.

Cara kerja:
  1. Coba semua kombinasi RF dan SVM dari ringan ke berat
  2. Ekspor setiap model ke .h via micromlgen
  3. Cek ukuran file .h sebagai proxy memori ESP32
  4. Filter yang melebihi batas
  5. Tampilkan tabel Pareto: Akurasi vs Ukuran Model
"""
import os
import tempfile
import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import StratifiedGroupKFold, cross_val_score
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from micromlgen import port

# ==========================================
# KONFIGURASI
# ==========================================
DATA_PATH      = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_v4', 'extracted_features_v4.csv'))
OUTPUT_HEADER_RF  = os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_rf_v4', 'rf_model_v4.h')
OUTPUT_HEADER_SVM = os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_svm_v4', 'svm_model_v4.h')
FEATURES       = ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']

# Batas memori ESP32 (ukuran file .h yang aman)
# Estimasi: Flash ESP32 = 4MB, firmware + stack ~1.5MB, sisa ~2MB untuk model
# Praktisnya kita batasi 400KB agar ada ruang cukup untuk runtime
ESP32_FLASH_LIMIT_KB = 400.0

# ==========================================
# LOAD DATA
# ==========================================
print("=" * 70)
print("MEMORY-CONSTRAINED AUTOML UNTUK ESP32")
print("=" * 70)

df = pd.read_csv(DATA_PATH)
print(f"Dataset: {len(df)} windows dari {df['subject_id'].nunique()} subjek")
print(f"Class dist: {(df['bp_class']==0).sum()}N / {(df['bp_class']==1).sum()}H")
print(f"Batas Flash ESP32: {ESP32_FLASH_LIMIT_KB} KB\n")

X      = df[FEATURES].values
y      = df['bp_class'].values
groups = df['subject_id'].values

sgkf = StratifiedGroupKFold(n_splits=5)

# ==========================================
# GRID KONFIGURASI (Ringan → Berat)
# ==========================================
configs = []

# SVM (Sangat ringan, diekspansi ekstensif)
for kernel in ['linear', 'rbf', 'poly', 'sigmoid']:
    for C in [0.01, 0.1, 1.0, 10.0, 50.0]:
        if kernel == 'linear':
            configs.append({
                'type':   'SVM',
                'name':   f'SVM-LIN-C{C}',
                'kernel': 'linear',
                'C':      C,
                'gamma':  'scale',
                'degree': 3,
            })
        else:
            for gamma in [0.001, 0.01, 0.1, 1.0, 'scale']:
                if kernel == 'poly':
                    for degree in [2, 3, 4]:
                        configs.append({
                            'type':   'SVM',
                            'name':   f'SVM-POL-C{C}-d{degree}-g{gamma}',
                            'kernel': 'poly',
                            'C':      C,
                            'gamma':  gamma,
                            'degree': degree,
                        })
                else:
                    k_name = kernel[:3].upper()
                    configs.append({
                        'type':   'SVM',
                        'name':   f'SVM-{k_name}-C{C}-g{gamma}',
                        'kernel': kernel,
                        'C':      C,
                        'gamma':  gamma,
                        'degree': 3,
                    })

# RF (dari sangat ringan ke batas maksimal ESP32)
for n_trees in [10, 15, 20, 25, 30, 40, 50, 75, 100]:
    for depth in [3, 4, 5, 6, 7]:
        for min_leaf in [1, 2, 3]:
            configs.append({
                'type':      'RF',
                'name':      f'RF-{n_trees}t-d{depth}-L{min_leaf}',
                'n_trees':   n_trees,
                'max_depth': depth,
                'min_leaf':  min_leaf,
            })

print(f"Total konfigurasi yang akan diuji: {len(configs)}\n")

# ==========================================
# EVALUASI SETIAP KONFIGURASI
# ==========================================
results = []
os.makedirs(os.path.dirname(OUTPUT_HEADER_RF), exist_ok=True)
tmp_file = os.path.join(os.path.dirname(OUTPUT_HEADER_RF), '_auto_tune_tmp.h')

print(f"{'No':>3} {'Model':<28} {'CV Acc':>7} {'CV Std':>7} {'Size':>8} {'Status'}")
print("-" * 70)

for i, cfg in enumerate(configs, 1):
    try:
        # Buat model
        if cfg['type'] == 'SVM':
            clf = SVC(kernel=cfg['kernel'], C=cfg['C'], gamma=cfg['gamma'], degree=cfg.get('degree', 3),
                      class_weight='balanced', random_state=42)
            pipe = Pipeline([('sc', StandardScaler()), ('clf', clf)])
            scaler = StandardScaler()
            X_train = scaler.fit_transform(X)
        else:
            clf = RandomForestClassifier(n_estimators=cfg['n_trees'], max_depth=cfg['max_depth'],
                                         min_samples_leaf=cfg['min_leaf'],
                                         class_weight='balanced', random_state=42)
            pipe = clf
            scaler = None
            X_train = X

        # CV Score
        scores = cross_val_score(pipe, X, y, groups=groups, cv=sgkf, scoring='accuracy')
        cv_mean = scores.mean()
        cv_std  = scores.std()

        # Train on semua data untuk ekspor
        clf.fit(X_train, y)

        # Ekspor ke C++ via micromlgen
        try:
            cpp_code = port(clf)
            with open(tmp_file, 'w') as f:
                f.write(cpp_code)
            size_kb = os.path.getsize(tmp_file) / 1024.0
        except Exception:
            size_kb = 9999.0  # Tidak bisa diekspor

        # Cek batas
        if size_kb > ESP32_FLASH_LIMIT_KB:
            status = f"[SKIP] (Max {ESP32_FLASH_LIMIT_KB:.0f} KB)"
            fits   = False
        else:
            status = "[OK]"
            fits   = True

        results.append({
            'name':    cfg['name'],
            'type':    cfg['type'],
            'cv_mean': cv_mean,
            'cv_std':  cv_std,
            'size_kb': size_kb,
            'fits':    fits,
            'config':  cfg,
            'scaler':  scaler,
            'model':   clf,
        })

        print(f"{i:>3} {cfg['name']:<28} {cv_mean*100:>6.1f}% {cv_std*100:>6.1f}% {size_kb:>7.1f}KB  {status}")

    except Exception as e:
        print(f"{i:>3} {cfg['name']:<28}  ERROR: {e}")

# Hapus file tmp
if os.path.exists(tmp_file):
    os.remove(tmp_file)

# ==========================================
# HASIL AKHIR: TABEL PARETO
# ==========================================
df_results = pd.DataFrame([{
    'Model': r['name'],
    'Type':  r['type'],
    'CV_Acc': r['cv_mean'],
    'CV_Std': r['cv_std'],
    'Lower_Bound': r['cv_mean'] - r['cv_std'],
    'Size_KB': r['size_kb'],
    'Fits_ESP32': r['fits'],
} for r in results])

df_fits = df_results[df_results['Fits_ESP32']].sort_values('Lower_Bound', ascending=False)

print("\n" + "=" * 80)
print("TABEL PARETO -- MODEL YANG MUAT DI ESP32 (Diurutkan berdasar Lower Bound)")
print("=" * 80)
print(f"{'Rank':>4} {'Model':<28} {'Akurasi':>8} {'Std':>6} {'L-Bound':>8} {'Ukuran':>9}")
print("-" * 80)
for rank, (_, row) in enumerate(df_fits.head(15).iterrows(), 1):
    print(f"{rank:>4} {row['Model']:<28} {row['CV_Acc']*100:>7.1f}% {row['CV_Std']*100:>5.1f}% {row['Lower_Bound']*100:>7.1f}% {row['Size_KB']:>7.1f} KB")

# ==========================================
# DEPLOY PEMENANG PER KATEGORI
# ==========================================
rf_results = sorted([r for r in results if r['fits'] and r['type'] == 'RF'], key=lambda x: x['cv_mean'] - x['cv_std'], reverse=True)
svm_results = sorted([r for r in results if r['fits'] and r['type'] == 'SVM'], key=lambda x: x['cv_mean'] - x['cv_std'], reverse=True)

best_rf = rf_results[0] if rf_results else None
best_svm = svm_results[0] if svm_results else None

if best_rf:
    print(f"\n{'='*70}")
    print(f"[WINNER RF] {best_rf['name']}")
    print(f"   Akurasi CV  : {best_rf['cv_mean']*100:.1f}% +/- {best_rf['cv_std']*100:.1f}%")
    print(f"   Ukuran Model: {best_rf['size_kb']:.1f} KB")
    print(f"{'='*70}")
    
    cpp_code_rf = port(best_rf['model'])
    os.makedirs(os.path.dirname(OUTPUT_HEADER_RF), exist_ok=True)
    with open(OUTPUT_HEADER_RF, 'w') as f:
        f.write(cpp_code_rf)
    print(f"[SAVED] Model RF diekspor ke: {OUTPUT_HEADER_RF}")

if best_svm:
    print(f"\n{'='*70}")
    print(f"[WINNER SVM] {best_svm['name']}")
    print(f"   Akurasi CV  : {best_svm['cv_mean']*100:.1f}% +/- {best_svm['cv_std']*100:.1f}%")
    print(f"   Ukuran Model: {best_svm['size_kb']:.1f} KB")
    print(f"{'='*70}")
    
    cpp_code_svm = port(best_svm['model'])
    os.makedirs(os.path.dirname(OUTPUT_HEADER_SVM), exist_ok=True)
    with open(OUTPUT_HEADER_SVM, 'w') as f:
        f.write(cpp_code_svm)
    print(f"[SAVED] Model SVM diekspor ke: {OUTPUT_HEADER_SVM}")
    
    if best_svm['scaler'] is not None:
        print("\n[ESP32] Parameter Z-Score Normalization untuk SVM (hardcode ke firmware):")
        for i, feat in enumerate(FEATURES):
            print(f"  {feat:18} Mean: {best_svm['scaler'].mean_[i]:.8f}  Scale: {best_svm['scaler'].scale_[i]:.8f}")

print("\nSelesai.")
