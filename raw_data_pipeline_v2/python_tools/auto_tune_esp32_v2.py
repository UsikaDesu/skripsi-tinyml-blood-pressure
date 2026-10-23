"""
AutoML V2 (PREPROCESSING + GROUPING) — Tanpa SQA
=================================================
Menggunakan StratifiedGroupKFold untuk mencegah data leakage antar subjek.
Tidak ada penyeleksian kualitas sinyal (SQA) — semua window lolos asalkan
puncak dapat terdeteksi.
"""
import os
import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import StratifiedGroupKFold, cross_val_score, cross_val_predict
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import precision_score, recall_score, f1_score
from sklearn.pipeline import Pipeline
from micromlgen import port

DATA_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_v2', 'extracted_features_v2.csv'))
OUTPUT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__)))
FEATURES  = ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']
ESP32_FLASH_LIMIT_KB = 400.0

print("=" * 70)
print("AUTO-TUNE V2: Preprocessing + Grouping (Tanpa SQA)")
print("=" * 70)

df = pd.read_csv(DATA_PATH)
print(f"Dataset: {len(df)} windows dari {df['subject_id'].nunique()} subjek")
print(f"Class dist: {(df['bp_class']==0).sum()}N / {(df['bp_class']==1).sum()}H\n")

X      = df[FEATURES].values
y      = df['bp_class'].values
groups = df['subject_id'].values
sgkf   = StratifiedGroupKFold(n_splits=5)

configs = []
for kernel in ['linear', 'rbf', 'poly', 'sigmoid']:
    for C in [0.01, 0.1, 1.0, 10.0, 50.0]:
        if kernel == 'linear':
            configs.append({'type':'SVM','name':f'SVM-LIN-C{C}','kernel':'linear','C':C,'gamma':0.001,'degree':3})
        else:
            for gamma in [0.001, 0.01, 0.1, 0.25, 1.0]:
                if kernel == 'poly':
                    for degree in [2, 3, 4]:
                        configs.append({'type':'SVM','name':f'SVM-POL-C{C}-d{degree}-g{gamma}','kernel':'poly','C':C,'gamma':gamma,'degree':degree})
                else:
                    k_name = kernel[:3].upper()
                    configs.append({'type':'SVM','name':f'SVM-{k_name}-C{C}-g{gamma}','kernel':kernel,'C':C,'gamma':gamma,'degree':3})

for n_trees in [10, 15, 20, 25, 30, 40, 50, 75, 100]:
    for depth in [3, 4, 5, 6, 7]:
        for min_leaf in [1, 2, 3]:
            configs.append({'type':'RF','name':f'RF-{n_trees}t-d{depth}-L{min_leaf}','n_trees':n_trees,'max_depth':depth,'min_leaf':min_leaf})

print(f"Total konfigurasi: {len(configs)}\n")

results = []
tmp_file = os.path.join(OUTPUT_DIR, '_tmp.h')

print(f"{'No':>3} {'Model':<28} {'CV Acc':>7} {'CV Std':>7}  {'5-Fold Scores':<35} {'Size':>8} {'Status'}")
print("-" * 110)

for i, cfg in enumerate(configs, 1):
    try:
        if cfg['type'] == 'SVM':
            clf = SVC(kernel=cfg['kernel'], C=cfg['C'], gamma=cfg['gamma'], degree=cfg.get('degree', 3),
                      class_weight='balanced', random_state=42)
            pipe = Pipeline([('sc', StandardScaler()), ('clf', clf)])
            scaler = StandardScaler(); X_train = scaler.fit_transform(X)
        else:
            clf = RandomForestClassifier(n_estimators=cfg['n_trees'], max_depth=cfg['max_depth'],
                                         min_samples_leaf=cfg['min_leaf'], class_weight='balanced', random_state=42)
            pipe = clf; scaler = None; X_train = X

        scores = cross_val_score(pipe, X, y, groups=groups, cv=sgkf, scoring='accuracy')
        cv_mean = scores.mean(); cv_std = scores.std()
        clf.fit(X_train, y)
        try:
            cpp_code = port(clf)
            with open(tmp_file, 'w') as f: f.write(cpp_code)
            size_kb = os.path.getsize(tmp_file) / 1024.0
        except Exception: size_kb = 9999.0

        fits   = size_kb <= ESP32_FLASH_LIMIT_KB
        status = "[OK]" if fits else f"[SKIP] (Max {ESP32_FLASH_LIMIT_KB:.0f} KB)"
        results.append({'type':cfg['type'],'name':cfg['name'],'cv_mean':cv_mean,'cv_std':cv_std,
                        'scores':scores.tolist(),'size_kb':size_kb,'fits':fits,'config':cfg,'scaler':scaler,'model':clf})
        folds_str = "[" + ", ".join([f"{s*100:.1f}" for s in scores]) + "]"
        print(f"{i:>3} {cfg['name']:<28} {cv_mean*100:>6.1f}% {cv_std*100:>6.1f}%   {folds_str:<35} {size_kb:>7.1f}KB  {status}")
    except Exception as e:
        print(f"{i:>3} {cfg['name']:<28}  ERROR: {e}")

if os.path.exists(tmp_file): os.remove(tmp_file)

df_results = pd.DataFrame([{'Model':r['name'],'Type':r['type'],'CV_Acc':r['cv_mean'],'CV_Std':r['cv_std'],
    'Lower_Bound':r['cv_mean']-r['cv_std'],'Size_KB':r['size_kb'],'Fits_ESP32':r['fits'],'Scores':r['scores']} for r in results])
df_fits = df_results[df_results['Fits_ESP32']].sort_values('Lower_Bound', ascending=False)

print("\n" + "=" * 80)
print("TABEL PARETO V2 -- MODEL YANG MUAT DI ESP32")
print("=" * 80)
print(f"{'Rank':>4} {'Model':<28} {'Akurasi':>8} {'Std':>6} {'L-Bound':>8}  {'5-Fold Scores':<35} {'Ukuran':>9}")
print("-" * 115)
for rank, (_, row) in enumerate(df_fits.head(15).iterrows(), 1):
    folds_str = "[" + ", ".join([f"{s*100:.1f}" for s in row['Scores']]) + "]"
    print(f"{rank:>4} {row['Model']:<28} {row['CV_Acc']*100:>7.1f}% {row['CV_Std']*100:>5.1f}% {row['Lower_Bound']*100:>7.1f}%  {folds_str:<35} {row['Size_KB']:>7.1f} KB")

rf_results  = sorted([r for r in results if r['fits'] and r['type']=='RF'],  key=lambda x: x['cv_mean']-x['cv_std'], reverse=True)
svm_results = sorted([r for r in results if r['fits'] and r['type']=='SVM'], key=lambda x: x['cv_mean']-x['cv_std'], reverse=True)

for label, best, clf_type in [('RF', rf_results[0] if rf_results else None, 'RF'),
                               ('SVM', svm_results[0] if svm_results else None, 'SVM')]:
    if not best: continue
    if clf_type == 'SVM':
        pipe = Pipeline([('sc', StandardScaler()), ('clf', best['model'])])
        y_pred = cross_val_predict(pipe, X, y, groups=groups, cv=sgkf)
    else:
        y_pred = cross_val_predict(best['model'], X, y, groups=groups, cv=sgkf)
    print(f"\n{'='*70}")
    print(f"[WINNER {label}] {best['name']}")
    print(f"   Akurasi CV  : {best['cv_mean']*100:.1f}% +/- {best['cv_std']*100:.1f}%")
    print(f"   Precision   : {precision_score(y, y_pred)*100:.1f}%")
    print(f"   Recall      : {recall_score(y, y_pred)*100:.1f}%")
    print(f"   F1-Score    : {f1_score(y, y_pred)*100:.1f}%")
    print(f"   Detail Folds: {[round(s*100, 1) for s in best['scores']]} %")
    print(f"   Ukuran Model: {best['size_kb']:.1f} KB")
    print(f"{'='*70}")

output_excel_full   = os.path.join(OUTPUT_DIR, 'auto_tune_full_results_v2.xlsx')
output_excel_pareto = os.path.join(OUTPUT_DIR, 'auto_tune_pareto_v2.xlsx')
try:
    df_results.to_excel(output_excel_full, index=False)
    df_fits.to_excel(output_excel_pareto, index=False)
    print(f"\n[EKSPOR] Full  : {output_excel_full}")
    print(f"[EKSPOR] Pareto: {output_excel_pareto}")
except Exception:
    df_results.to_csv(output_excel_full.replace('.xlsx', '.csv'), index=False)
    df_fits.to_csv(output_excel_pareto.replace('.xlsx', '.csv'), index=False)

print("\nSelesai V2.")
