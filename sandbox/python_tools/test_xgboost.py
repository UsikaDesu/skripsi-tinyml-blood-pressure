import os
import pandas as pd
import numpy as np
import xgboost as xgb
from sklearn.model_selection import StratifiedGroupKFold, cross_val_score
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline

# ==========================================
# KONFIGURASI
# ==========================================
DATA_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v3', 'dataset', 'extracted_features_v3.csv'))
FEATURES  = ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']

print("=" * 60)
print("TEST XGBOOST CLASSIFIER (DATASET V3)")
print("=" * 60)

# Load data
df = pd.read_csv(DATA_PATH)
X = df[FEATURES].values
y = df['bp_class'].values
groups = df['subject_id'].values

print(f"Data termuat: {len(df)} windows dari {len(np.unique(groups))} subjek.")

# Hyperparameter grid untuk XGBoost yang ringan
learning_rates = [0.01, 0.1, 0.2]
max_depths = [2, 3, 4, 5]
n_estimators_list = [10, 20, 30, 50]
min_child_weights = [1, 2, 3]

results = []

cv = StratifiedGroupKFold(n_splits=5)

print("\nMulai Evaluasi (Mencari konfigurasi terbaik dari 144 kombinasi)...")
for lr in learning_rates:
    for depth in max_depths:
        for n_est in n_estimators_list:
            for mcw in min_child_weights:
                model = xgb.XGBClassifier(
                    n_estimators=n_est,
                    max_depth=depth,
                    learning_rate=lr,
                    min_child_weight=mcw,
                    use_label_encoder=False,
                    eval_metric='logloss',
                    random_state=42
                )
                
                pipeline = Pipeline([
                    ('scaler', StandardScaler()),
                    ('classifier', model)
                ])
                
                scores = cross_val_score(pipeline, X, y, cv=cv, groups=groups, scoring='accuracy')
                
                results.append({
                    'name': f'XGB-lr{lr}-d{depth}-n{n_est}-mcw{mcw}',
                    'lr': lr,
                    'depth': depth,
                    'n_est': n_est,
                    'mcw': mcw,
                    'cv_mean': scores.mean(),
                    'cv_std': scores.std()
                })

# Urutkan berdasarkan akurasi tertinggi, lalu std terendah
df_res = pd.DataFrame(results)
df_res = df_res.sort_values(by=['cv_mean', 'cv_std'], ascending=[False, True]).reset_index(drop=True)

print("\nTOP 10 KONFIGURASI XGBOOST:")
print("-" * 60)
print(f"{'Rank':<5} {'Model':<25} {'Akurasi':<10} {'Std':<10}")
print("-" * 60)

for i in range(min(10, len(df_res))):
    row = df_res.iloc[i]
    print(f"{i+1:<5} {row['name']:<25} {row['cv_mean']*100:.1f}%      {row['cv_std']*100:.1f}%")

print("=" * 60)
print(f"Pemenang XGBoost: {df_res.iloc[0]['name']} dengan akurasi {df_res.iloc[0]['cv_mean']*100:.1f}%")
print(f"Bandingkan dengan Random Forest terbaik (RF-5t-d6-L2): 75.9%")
