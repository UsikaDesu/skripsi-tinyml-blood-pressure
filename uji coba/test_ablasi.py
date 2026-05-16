import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import recall_score, precision_score, accuracy_score
from sklearn.preprocessing import StandardScaler
import os

DATA = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset', 'extracted_features.csv')
df = pd.read_csv(DATA)
y = df['bp_class'].values

combos = {
    # === FITUR TUNGGAL ===
    'ir_mean':               ['ir_mean'],
    'red_mean':              ['red_mean'],
    'red_std':               ['red_std'],
    'ir_std':                ['ir_std'],
    
    # === GABUNGAN BERDASARKAN KEKUATAN ===
    'KUAT+KUAT (mean+mean)': ['ir_mean', 'red_mean'],
    'KUAT+SEDANG (ir_m+r_s)':['ir_mean', 'red_std'],
    'SEDANG+LEMAH (stds)':   ['ir_std', 'red_std'],
    
    # === 3 FITUR TERBAIK (buang ir_std yang terlemah) ===
    '3 TERBAIK (tanpa ir_std)': ['ir_mean', 'red_mean', 'red_std'],
    
    # === SEMUA 4 ===
    'SEMUA 4 fitur':         ['ir_mean', 'red_mean', 'ir_std', 'red_std'],
}

models = {
    'SVM': lambda: SVC(kernel='rbf', class_weight='balanced', random_state=42),
    'RF':  lambda: RandomForestClassifier(n_estimators=30, max_depth=10, random_state=42, class_weight='balanced'),
}

print('=' * 85)
print('STUDI ABLASI: GABUNGAN FITUR BERDASARKAN KEMIRIPAN PERFORMA')
print('=' * 85)

for model_name, model_fn in models.items():
    print(f"\n{'─'*85}")
    print(f"  {model_name} (class_weight=balanced)")
    print(f"{'─'*85}")
    header = f"{'Kombinasi':<30s} {'Jml':>3s} {'Acc':>6s} {'Recall':>7s} {'Prec':>6s} {'F1':>6s} {'CV-5':>7s}"
    print(header)
    print('-' * 78)
    
    prev_count = -1
    for name, feats in combos.items():
        # Separator
        if len(feats) != prev_count:
            if prev_count > 0:
                print()
            prev_count = len(feats)
        
        X = df[feats].values
        scaler = StandardScaler()
        X_s = scaler.fit_transform(X)
        X_tr, X_te, y_tr, y_te = train_test_split(X_s, y, test_size=0.2, random_state=42, stratify=y)
        
        clf = model_fn()
        clf.fit(X_tr, y_tr)
        preds = clf.predict(X_te)
        
        acc = accuracy_score(y_te, preds)
        rec = recall_score(y_te, preds, zero_division=0)
        pre = precision_score(y_te, preds, zero_division=0)
        f1 = 2*pre*rec/(pre+rec) if (pre+rec) > 0 else 0
        cv = cross_val_score(clf, X_s, y, cv=5, scoring='accuracy').mean()
        
        marker = ' ***' if f1 >= 0.85 else ''
        print(f"{name:<30s} {len(feats):>3d} {acc*100:5.1f}% {rec*100:6.1f}% {pre*100:5.1f}% {f1*100:5.1f}%  {cv*100:5.1f}%{marker}")

print()
print("*** = F1 >= 85% (kandidat kuat)")
print()
print("Keterangan:")
print("  KUAT   = Fitur dengan Recall individual >= 80%")  
print("  SEDANG = Fitur dengan Recall individual 40-79%")
print("  LEMAH  = Fitur dengan Recall individual < 40%")
