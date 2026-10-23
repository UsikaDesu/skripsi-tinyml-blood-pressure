"""
EVALUASI RESMI LEAVE-ONE-SUBJECT-OUT (LOSO) CROSS-VALIDATION
Sesuai Instruksi Dosen Pembimbing:
- Subject test bergantian satu per satu (20 Folds)
- Model dilatih pada 19 subjek, diuji murni pada 1 subjek independen
- Evaluasi disajikan pada tingkat jendela rekaman (Window-Level) dan diagnosis per orang (Subject-Level via Majority Voting)
"""

import os
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import LeaveOneGroupOut
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
DATA_PATH = os.path.join(BASE_DIR, 'dataset_v4', 'extracted_features_v4.csv')

df = pd.read_csv(DATA_PATH)

groups = df['subject_id'].values
y = df['bp_class'].values
subjects = np.unique(groups)

models = {
    '2 Fitur {CTR, HR} (Model Terpilih)': {
        'feats': ['crest_time_ratio', 'heart_rate'],
        'clf': RandomForestClassifier(n_estimators=20, max_depth=3, min_samples_leaf=4, max_features=1, criterion='entropy', random_state=42, class_weight='balanced')
    },
    '3 Fitur {CTR, HR, Red Ratio}': {
        'feats': ['crest_time_ratio', 'heart_rate', 'red_ratio'],
        'clf': RandomForestClassifier(n_estimators=20, max_depth=5, min_samples_leaf=2, max_features=1, criterion='gini', random_state=42, class_weight='balanced')
    },
    '4 Fitur {IR, Red, CTR, HR}': {
        'feats': ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate'],
        'clf': RandomForestClassifier(n_estimators=20, max_depth=4, min_samples_leaf=6, max_features=2, criterion='gini', random_state=42, class_weight='balanced')
    }
}

logo = LeaveOneGroupOut()
summary = {}
subject_predictions = {s: {} for s in subjects}

for name, cfg in models.items():
    X = df[cfg['feats']].values
    clf = cfg['clf']
    
    y_true_win, y_pred_win = [], []
    y_true_subj, y_pred_subj = [], []
    
    for tr_idx, te_idx in logo.split(X, y, groups=groups):
        subj = groups[te_idx][0]
        true_cls = y[te_idx][0]
        
        clf.fit(X[tr_idx], y[tr_idx])
        preds = clf.predict(X[te_idx])
        
        y_true_win.extend(y[te_idx])
        y_pred_win.extend(preds)
        
        # Majority voting diagnosis klinis per subjek
        maj_pred = 1 if np.mean(preds) >= 0.5 else 0
        y_true_subj.append(true_cls)
        y_pred_subj.append(maj_pred)
        
        win_acc = accuracy_score(y[te_idx], preds) * 100
        subject_predictions[subj][name] = {
            'win_acc': win_acc,
            'subj_pred': maj_pred,
            'is_correct': (maj_pred == true_cls)
        }
        
    summary[name] = {
        'win_acc': accuracy_score(y_true_win, y_pred_win) * 100,
        'win_prec': precision_score(y_true_win, y_pred_win) * 100,
        'win_rec': recall_score(y_true_win, y_pred_win) * 100,
        'win_f1': f1_score(y_true_win, y_pred_win) * 100,
        'subj_acc': accuracy_score(y_true_subj, y_pred_subj) * 100,
        'subj_prec': precision_score(y_true_subj, y_pred_subj) * 100,
        'subj_rec': recall_score(y_true_subj, y_pred_subj) * 100,
        'subj_f1': f1_score(y_true_subj, y_pred_subj) * 100,
        'subj_cm': confusion_matrix(y_true_subj, y_pred_subj)
    }

def print_report():
    print("=" * 100)
    print("HASIL EVALUASI LEAVE-ONE-SUBJECT-OUT (LOSO) VALIDATION")
    print("Skema: 20 Putaran Bergantian (19 Subjek Latih, 1 Subjek Uji Independen)")
    print("=" * 100)

    print("\n--- 1. EVALUASI TINGKAT SUBJEK (DIAGNOSIS KLINIS PER ORANG) ---")
    print(f"{'Konfigurasi':<34} | {'Akurasi Subjek':<15} | {'Presisi':<10} | {'Recall':<10} | {'F1-Score':<10} | {'Confusion Matrix'}")
    print("-" * 105)
    for name, s in summary.items():
        cm = s['subj_cm']
        cm_str = f"TN={cm[0,0]}, FP={cm[0,1]}, FN={cm[1,0]}, TP={cm[1,1]}"
        print(f"{name:<34} | {s['subj_acc']:>5.1f}% ({int(s['subj_acc']*20/100)}/20)    | {s['subj_prec']:>5.1f}%    | {s['subj_rec']:>5.1f}%    | {s['subj_f1']:>5.1f}%    | {cm_str}")

    print("\n--- 2. EVALUASI TINGKAT JENDELA REKAMAN (WINDOW-LEVEL, 200 WINDOWS) ---")
    print(f"{'Konfigurasi':<34} | {'Akurasi Window':<15} | {'Presisi':<10} | {'Recall':<10} | {'F1-Score':<10}")
    print("-" * 85)
    for name, s in summary.items():
        print(f"{name:<34} | {s['win_acc']:>5.1f}%          | {s['win_prec']:>5.1f}%    | {s['win_rec']:>5.1f}%    | {s['win_f1']:>5.1f}%")

    print("\n" + "=" * 105)
    print("--- 3. TABEL DETAIL DIAGNOSIS 20 SUBJEK SECARA BERGANTIAN ---")
    print("=" * 105)
    print(f"{'ID Subjek':<12} | {'Kelas Nyata':<11} | {'Tensi (S/D)':<11} | {'2 Fitur':<18} | {'3 Fitur':<18} | {'4 Fitur':<18}")
    print("-" * 105)

    for subj in sorted(subjects):
        s_df = df[df['subject_id'] == subj]
        true_cls = s_df['bp_class'].iloc[0]
        true_str = "Hipertensi" if true_cls == 1 else "Normal"
        bp_str = f"{s_df['systolic_bp'].mean():.0f}/{s_df['diastolic_bp'].mean():.0f}"
        
        r2 = subject_predictions[subj]['2 Fitur {CTR, HR} (Model Terpilih)']
        r3 = subject_predictions[subj]['3 Fitur {CTR, HR, Red Ratio}']
        r4 = subject_predictions[subj]['4 Fitur {IR, Red, CTR, HR}']
        
        s2_str = f"{'BENAR [OK]' if r2['is_correct'] else 'SALAH [X]'} ({r2['win_acc']:.0f}%)"
        s3_str = f"{'BENAR [OK]' if r3['is_correct'] else 'SALAH [X]'} ({r3['win_acc']:.0f}%)"
        s4_str = f"{'BENAR [OK]' if r4['is_correct'] else 'SALAH [X]'} ({r4['win_acc']:.0f}%)"
        
        print(f"{subj:<12} | {true_str:<11} | {bp_str:<11} | {s2_str:<18} | {s3_str:<18} | {s4_str:<18}")

if __name__ == '__main__':
    print_report()
