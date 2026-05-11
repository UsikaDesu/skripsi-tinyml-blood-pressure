import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import recall_score, precision_score, accuracy_score
from sklearn.preprocessing import StandardScaler
import os

DATA = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset', 'extracted_features.csv')
df = pd.read_csv(DATA)
FEATURES = ['ir_mean', 'red_mean', 'ir_std', 'red_std']
X = df[FEATURES].values
y = df['bp_class'].values
seeds = [1, 7, 13, 21, 42, 55, 73, 88, 99, 123]

models = {
    'RF':  lambda s: RandomForestClassifier(n_estimators=30, max_depth=10, random_state=s, class_weight='balanced'),
    'SVM': lambda s: SVC(kernel='rbf', class_weight='balanced', random_state=s),
}

for model_name, model_fn in models.items():
    print('=' * 55)
    print(f'  {model_name}: Ulangi 10x dengan random_state BERBEDA')
    print('=' * 55)
    print(f"{'Run':>4s} {'Seed':>5s} {'Acc':>7s} {'Recall':>8s} {'Prec':>7s} {'F1':>7s}")
    print('-' * 50)
    
    accs, recs, pres, f1s = [], [], [], []
    for i, seed in enumerate(seeds):
        scaler = StandardScaler()
        X_s = scaler.fit_transform(X)
        X_tr, X_te, y_tr, y_te = train_test_split(X_s, y, test_size=0.2, random_state=seed, stratify=y)
        clf = model_fn(seed)
        clf.fit(X_tr, y_tr)
        preds = clf.predict(X_te)
        acc = accuracy_score(y_te, preds)
        rec = recall_score(y_te, preds, zero_division=0)
        pre = precision_score(y_te, preds, zero_division=0)
        f1 = 2*pre*rec/(pre+rec) if (pre+rec) > 0 else 0
        accs.append(acc); recs.append(rec); pres.append(pre); f1s.append(f1)
        print(f"  {i+1:2d}  {seed:>4d}  {acc*100:5.1f}%  {rec*100:6.1f}%  {pre*100:5.1f}%  {f1*100:5.1f}%")
    
    print('-' * 50)
    print(f" AVG        {np.mean(accs)*100:5.1f}%  {np.mean(recs)*100:6.1f}%  {np.mean(pres)*100:5.1f}%  {np.mean(f1s)*100:5.1f}%")
    print(f" STD       +-{np.std(accs)*100:4.1f}% +-{np.std(recs)*100:5.1f}% +-{np.std(pres)*100:4.1f}% +-{np.std(f1s)*100:4.1f}%")
    print()
