import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import recall_score, precision_score, accuracy_score
from sklearn.preprocessing import StandardScaler
import os

DATA = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset', 'extracted_features.csv')
df = pd.read_csv(DATA)
FEATURES = ['ir_mean', 'red_mean', 'ir_std', 'red_std']
X = df[FEATURES].values
y = df['bp_class'].values

kernels = {
    'Linear':     {'kernel': 'linear'},
    'RBF':        {'kernel': 'rbf'},
    'Polynomial': {'kernel': 'poly', 'degree': 3},
    'Sigmoid':    {'kernel': 'sigmoid'},
}

seeds = [1, 7, 13, 21, 42, 55, 73, 88, 99, 123]

print('=' * 75)
print('PERBANDINGAN 4 KERNEL SVM (Rata-rata 10 seed)')
print(f'Data: {len(df)} baris | Fitur: {FEATURES}')
print('=' * 75)

print(f"\n{'Kernel':<14s} {'Acc':>7s} {'Recall':>8s} {'Prec':>7s} {'F1':>7s} {'CV-5':>7s}")
print('-' * 55)

for name, params in kernels.items():
    accs, recs, pres, f1s, cvs = [], [], [], [], []
    
    for seed in seeds:
        scaler = StandardScaler()
        X_s = scaler.fit_transform(X)
        X_tr, X_te, y_tr, y_te = train_test_split(X_s, y, test_size=0.2, random_state=seed, stratify=y)
        
        clf = SVC(class_weight='balanced', random_state=seed, **params)
        clf.fit(X_tr, y_tr)
        preds = clf.predict(X_te)
        
        acc = accuracy_score(y_te, preds)
        rec = recall_score(y_te, preds, zero_division=0)
        pre = precision_score(y_te, preds, zero_division=0)
        f1 = 2*pre*rec/(pre+rec) if (pre+rec) > 0 else 0
        cv = cross_val_score(clf, X_s, y, cv=5, scoring='accuracy').mean()
        
        accs.append(acc); recs.append(rec); pres.append(pre); f1s.append(f1); cvs.append(cv)
    
    print(f"{name:<14s} {np.mean(accs)*100:5.1f}%  {np.mean(recs)*100:6.1f}%  {np.mean(pres)*100:5.1f}%  {np.mean(f1s)*100:5.1f}%  {np.mean(cvs)*100:5.1f}%")

print('-' * 55)
print()

# Detail per seed untuk RBF vs Linear (2 terkuat)
print('=' * 75)
print('DETAIL PER SEED: RBF vs Linear')
print('=' * 75)
print(f"{'Seed':>5s}  {'--- RBF ---':^25s}  {'--- Linear ---':^25s}")
print(f"{'':>5s}  {'Acc':>6s} {'Rec':>6s} {'F1':>6s}  {'Acc':>6s} {'Rec':>6s} {'F1':>6s}")
print('-' * 60)

rbf_wins = 0
lin_wins = 0

for seed in seeds:
    scaler = StandardScaler()
    X_s = scaler.fit_transform(X)
    X_tr, X_te, y_tr, y_te = train_test_split(X_s, y, test_size=0.2, random_state=seed, stratify=y)
    
    results = {}
    for kname, kparams in [('rbf', {'kernel':'rbf'}), ('linear', {'kernel':'linear'})]:
        clf = SVC(class_weight='balanced', random_state=seed, **kparams)
        clf.fit(X_tr, y_tr)
        preds = clf.predict(X_te)
        acc = accuracy_score(y_te, preds)
        rec = recall_score(y_te, preds, zero_division=0)
        pre = precision_score(y_te, preds, zero_division=0)
        f1 = 2*pre*rec/(pre+rec) if (pre+rec) > 0 else 0
        results[kname] = (acc, rec, f1)
    
    r = results['rbf']
    l = results['linear']
    winner = '<' if r[2] > l[2] else ('>' if r[2] < l[2] else '=')
    if r[2] > l[2]: rbf_wins += 1
    elif r[2] < l[2]: lin_wins += 1
    
    print(f"{seed:>5d}  {r[0]*100:5.1f}% {r[1]*100:5.1f}% {r[2]*100:5.1f}%  {winner}  {l[0]*100:5.1f}% {l[1]*100:5.1f}% {l[2]*100:5.1f}%")

print('-' * 60)
print(f"RBF menang: {rbf_wins}x | Linear menang: {lin_wins}x | Seri: {10-rbf_wins-lin_wins}x")
