import os
import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import classification_report, accuracy_score, confusion_matrix
from sklearn.preprocessing import StandardScaler

DATA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset', 'extracted_features.csv')
FEATURES = ['ir_mean', 'red_mean', 'ir_std', 'red_std']

df = pd.read_csv(DATA_PATH)
X = df[FEATURES].values
y = df['bp_class'].values

# SVM butuh normalisasi data (berbeda dari RF)
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.2, random_state=42, stratify=y)

print('=' * 60)
print('TINYML SVM PIPELINE')
print('=' * 60)
print(f'Training: {len(X_train)}, Testing: {len(X_test)}')

clf = SVC(kernel='rbf', class_weight='balanced', random_state=42)
clf.fit(X_train, y_train)

preds = clf.predict(X_test)
acc = accuracy_score(y_test, preds)

print(f'\n--- Model Evaluation ---')
print(f'Accuracy: {acc*100:.2f}%\n')
print('Classification Report (WHO/Indonesia Standard):')
print(classification_report(y_test, preds, labels=[0,1], target_names=['Non-Hipertensi','Hipertensi'], zero_division=0))

cm = confusion_matrix(y_test, preds, labels=[0,1])
print('Confusion Matrix:')
print(f'                  Prediksi')
print(f'                  Non-HT  HT')
print(f'  Aktual Non-HT  [{cm[0][0]:4d}  {cm[0][1]:4d}]')
print(f'  Aktual HT      [{cm[1][0]:4d}  {cm[1][1]:4d}]')

cv = cross_val_score(clf, X_scaled, y, cv=5, scoring='accuracy')
print(f'\n--- Cross-Validation (5-Fold) ---')
print(f'Rata-rata CV: {cv.mean()*100:.2f}% (+/- {cv.std()*100:.2f}%)')
