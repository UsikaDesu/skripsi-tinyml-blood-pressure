"""
Train RF sandbox with optimizations:
1. Try multiple hyperparameter configs
2. Subject-Wise split preserved
"""
import os
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import StratifiedGroupKFold, GroupShuffleSplit, cross_val_score
from sklearn.metrics import classification_report, accuracy_score, confusion_matrix
from sklearn.preprocessing import StandardScaler
from micromlgen import port

DATA_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_sandbox', 'extracted_features_sandbox.csv'))
MODEL_OUTPUT_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_rf_sandbox', 'rf_model_sandbox.h'))



def main():
    print("======================================================================")
    print("OPTIMASI MODEL SANDBOX: StratifiedGroupKFold Test")
    print("======================================================================")
    
    df = pd.read_csv(DATA_PATH)
    print(f"Loaded {len(df)} samples, {df['subject_id'].nunique()} subjects")
    print(f"Class dist: {(df['bp_class']==0).sum()}N / {(df['bp_class']==1).sum()}H\n")
    
    # Gunakan fitur: ir_ratio, red_ratio, crest_time_ratio, heart_rate
    features = ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']
    X_base = df[features].values
    y = df['bp_class'].values
    groups = df['subject_id'].values
    
   # ── Hyperparameter Configs (TINYML SAFE LIMITS) ──
    # ── Hyperparameter Configs (TinyML GRID SEARCH) ──
    # Kombinasi sistematis untuk 100 Dataset dan 4 Fitur
    configs = [
        # Pohon sangat ringan (10 trees)
        {'name': 'RF-10t-d3-L1', 'n_estimators': 10, 'max_depth': 3, 'min_samples_leaf': 1},
        {'name': 'RF-10t-d4-L1', 'n_estimators': 10, 'max_depth': 4, 'min_samples_leaf': 1},
        {'name': 'RF-10t-d5-L2', 'n_estimators': 10, 'max_depth': 5, 'min_samples_leaf': 2},

        # Pohon ringan (15 trees)
        {'name': 'RF-15t-d3-L1', 'n_estimators': 15, 'max_depth': 3, 'min_samples_leaf': 1},
        {'name': 'RF-15t-d4-L2', 'n_estimators': 15, 'max_depth': 4, 'min_samples_leaf': 2},
        {'name': 'RF-15t-d5-L2', 'n_estimators': 15, 'max_depth': 5, 'min_samples_leaf': 2},

        # Pohon menengah (20 trees) - Sweet Spot!
        {'name': 'RF-20t-d3-L1', 'n_estimators': 20, 'max_depth': 3, 'min_samples_leaf': 1},
        {'name': 'RF-20t-d4-L2', 'n_estimators': 20, 'max_depth': 4, 'min_samples_leaf': 2},
        {'name': 'RF-20t-d5-L2', 'n_estimators': 20, 'max_depth': 5, 'min_samples_leaf': 2},

        # Pohon padat (25 trees)
        {'name': 'RF-25t-d3-L1', 'n_estimators': 25, 'max_depth': 3, 'min_samples_leaf': 1},
        {'name': 'RF-25t-d4-L2', 'n_estimators': 25, 'max_depth': 4, 'min_samples_leaf': 2},
        {'name': 'RF-25t-d5-L2', 'n_estimators': 25, 'max_depth': 5, 'min_samples_leaf': 2},

        # Pohon batas maksimal TinyML ESP32 (30 trees)
        {'name': 'RF-30t-d3-L2', 'n_estimators': 30, 'max_depth': 3, 'min_samples_leaf': 2},
        {'name': 'RF-30t-d4-L2', 'n_estimators': 30, 'max_depth': 4, 'min_samples_leaf': 2},
        {'name': 'RF-30t-d5-L3', 'n_estimators': 30, 'max_depth': 5, 'min_samples_leaf': 3},
    ]
    
    best_score = 0
    best_config = None
    best_name = None
    
    print("─" * 70)
    print(f"{'Config':<20} | {'CV Mean':>8} | {'CV Std':>7} | {'Fold Scores'}")
    print("─" * 70)
    
    for cfg in configs:
        clf = RandomForestClassifier(
            n_estimators=cfg['n_estimators'],
            max_depth=cfg['max_depth'],
            min_samples_leaf=cfg['min_samples_leaf'],
            class_weight='balanced',
            random_state=42
        )
        
        sgkf = StratifiedGroupKFold(n_splits=5)
        scores = cross_val_score(clf, X_base, y, groups=groups, cv=sgkf, scoring='accuracy')
        
        fold_str = ', '.join([f'{s*100:.0f}%' for s in scores])
        print(f"{cfg['name']:<20} | {scores.mean()*100:>7.1f}% | {scores.std()*100:>6.1f}% | [{fold_str}]")
        
        if scores.mean() > best_score:
            best_score = scores.mean()
            best_config = cfg
            best_name = cfg['name']
    
    print("─" * 70)
    print(f"Terbaik: {best_name} ({best_score*100:.1f}%)")
    

    
    # ── Final Model: Train on all data, report feature importance ──
    print(f"\n{'=' * 70}")
    print("FINAL: Train & Holdout Test (Subject-Wise)")
    print("=" * 70)
    
    sgkf_holdout = StratifiedGroupKFold(n_splits=4, shuffle=True, random_state=42)
    train_idx, test_idx = next(sgkf_holdout.split(X_base, y, groups))
    X_train, X_test = X_base[train_idx], X_base[test_idx]
    y_train, y_test = y[train_idx], y[test_idx]
    train_groups = groups[train_idx]
    test_groups = groups[test_idx]
    
    print(f"Train: {len(X_train)} windows dari {len(set(train_groups))} subjek")
    print(f"Test:  {len(X_test)} windows dari {len(set(test_groups))} subjek (unseen)")
    
    X_train_res, y_train_res = X_train, y_train
    
    clf = RandomForestClassifier(
        n_estimators=best_config['n_estimators'],
        max_depth=best_config['max_depth'],
        min_samples_leaf=best_config['min_samples_leaf'],
        class_weight='balanced',
        random_state=42
    )
    clf.fit(X_train_res, y_train_res)
    
    preds = clf.predict(X_test)
    acc = accuracy_score(y_test, preds)
    
    print(f"\nAkurasi: {acc*100:.2f}%")
    print("\nClassification Report:")
    print(classification_report(y_test, preds, labels=[0, 1], 
          target_names=["Non-Hipertensi", "Hipertensi"], zero_division=0))
    
    cm = confusion_matrix(y_test, preds, labels=[0, 1])
    print(f"Confusion Matrix:")
    print(f"                  Prediksi")
    print(f"                  Non-HT  HT")
    print(f"  Aktual Non-HT  [{cm[0][0]:4d}  {cm[0][1]:4d}]")
    print(f"  Aktual HT      [{cm[1][0]:4d}  {cm[1][1]:4d}]")
    
    print("\nFeature Importance:")
    features = ['ir_ratio', 'red_ratio', 'rise_time_ms', 'heart_rate']
    for feat, imp in sorted(zip(features, clf.feature_importances_), key=lambda x: -x[1]):
        bar = '#' * int(imp * 40)
        print(f"  {feat:15s}: {imp:.3f} {bar}")

    # ── Exporting Final Model ──
    print(f"\n{'=' * 70}")
    print("EXPORTING MODEL TO C HEADER (TinyML)")
    print("=" * 70)
    
    # Train ulang menggunakan SELURUH DATASET untuk model produksi akhir
    print(f"Training final production model using all {len(X_base)} windows...")
    clf_final = RandomForestClassifier(
        n_estimators=best_config['n_estimators'],
        max_depth=best_config['max_depth'],
        min_samples_leaf=best_config['min_samples_leaf'],
        class_weight='balanced',
        random_state=42
    )
    clf_final.fit(X_base, y)
    
    # Port to C code
    try:
        c_code = port(clf_final, classmap={0: "Normal", 1: "Hipertensi"})
        with open(MODEL_OUTPUT_PATH, "w") as f:
            f.write(c_code)
        print(f"[SUCCESS] Random Forest Model successfully exported to {MODEL_OUTPUT_PATH}")
        print(f"   (File size: {os.path.getsize(MODEL_OUTPUT_PATH)} bytes)")
    except Exception as e:
        print(f"[ERROR] Failed to export model: {e}")

if __name__ == "__main__":
    main()
