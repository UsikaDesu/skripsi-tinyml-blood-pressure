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
    'IR saja (mean+std)': ['ir_mean', 'ir_std'],
    'RED saja (mean+std)': ['red_mean', 'red_std'],
    'Mean saja (ir+red)': ['ir_mean', 'red_mean'],
    'STD saja (ir+red)': ['ir_std', 'red_std'],
    'SEMUA 4 fitur': ['ir_mean', 'red_mean', 'ir_std', 'red_std'],
}

models = {
    'SVM': lambda: SVC(kernel='rbf', class_weight='balanced', random_state=42),
    'RF':  lambda: RandomForestClassifier(n_estimators=30, max_depth=10, random_state=42, class_weight='balanced'),
}

print('=' * 80)
print('STUDI ABLASI FITUR: SVM vs RANDOM FOREST')
print('=' * 80)

for model_name, model_fn in models.items():
    print(f"\n--- {model_name} (class_weight=balanced) ---")
    header = f"{'Kombinasi Fitur':<30s} {'Acc':>6s} {'Recall':>7s} {'Prec':>6s} {'CV-5':>7s}"
    print(header)
    print('-' * 60)
    
    for name, feats in combos.items():
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
        cv = cross_val_score(clf, X_s, y, cv=5, scoring='accuracy').mean()
        print(f"{name:<30s} {acc*100:5.1f}% {rec*100:6.1f}% {pre*100:5.1f}% {cv*100:5.1f}%")
    
    print('-' * 60)

print()
print("Keterangan:")
print("  Acc    = Accuracy keseluruhan")
print("  Recall = Berapa % Hipertensi yang tertangkap")
print("  Prec   = Berapa % tebakan Hipertensi yang benar")
print("  CV-5   = Rata-rata Cross-Validation 5-Fold")
