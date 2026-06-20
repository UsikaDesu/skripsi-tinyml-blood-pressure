import os
import pandas as pd
import numpy as np
from sklearn.svm import SVC
from sklearn.model_selection import StratifiedGroupKFold, GroupShuffleSplit, cross_val_score
from sklearn.metrics import classification_report, accuracy_score, confusion_matrix
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline # WAJIB untuk mencegah data leakage
from micromlgen import port # Untuk ekspor ke C++ nanti

DATA_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset', 'extracted_features_v3.csv'))
MODEL_OUTPUT_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_svm_v3', 'svm_model_v3.h'))

def main():
    print("=" * 70)
    print("OPTIMASI MODEL V2: HYPERPARAMETER TUNING (SVM TinyML)")
    print("=" * 70)
    
    if not os.path.exists(DATA_PATH):
        print(f"File {DATA_PATH} tidak ditemukan!")
        return

    df = pd.read_csv(DATA_PATH)
    print(f"Loaded {len(df)} samples, {df['subject_id'].nunique()} subjects")
    print(f"Class dist: {(df['bp_class']==0).sum()}N / {(df['bp_class']==1).sum()}H\n")
    
    X_base = df[['ir_ratio', 'red_ratio', 'heart_rate', 'crest_time_ratio']].values
    y = df['bp_class'].values
    groups = df['subject_id'].values
    
    # ── Hyperparameter Configs SVM (TinyML Safe) ──
    # Menggunakan nilai gamma eksplisit (angka) agar micromlgen bisa melakukan ekspor C++
    configs = [
        {'name': 'SVM-Linear-C0.1', 'kernel': 'linear', 'C': 0.1, 'gamma': 0.001},
        {'name': 'SVM-Linear-C1.0', 'kernel': 'linear', 'C': 1.0, 'gamma': 0.001},
        {'name': 'SVM-Linear-C10.0', 'kernel': 'linear', 'C': 10.0, 'gamma': 0.001},
        {'name': 'SVM-RBF-C1.0-g0.1', 'kernel': 'rbf', 'C': 1.0, 'gamma': 0.1},
        {'name': 'SVM-RBF-C1.0-g0.01', 'kernel': 'rbf', 'C': 1.0, 'gamma': 0.01},
        {'name': 'SVM-RBF-C10.0-g0.1', 'kernel': 'rbf', 'C': 10.0, 'gamma': 0.1},
        {'name': 'SVM-RBF-C10.0-g0.01', 'kernel': 'rbf', 'C': 10.0, 'gamma': 0.01},
    ]
    
    best_score = 0
    best_config = None
    best_name = None
    
    print("─" * 70)
    print(f"{'Config':<20} | {'CV Mean':>8} | {'CV Std':>7} | {'Fold Scores'}")
    print("─" * 70)
    
    for cfg in configs:
        clf = SVC(
            kernel=cfg['kernel'],
            C=cfg['C'],
            gamma=cfg['gamma'],
            class_weight='balanced',
            random_state=42
        )
        
        # PATCH 1: PIPELINE ANTI DATA-LEAKAGE
        pipe = Pipeline([
            ('scaler', StandardScaler()),
            ('svm', clf)
        ])
        
        sgkf = StratifiedGroupKFold(n_splits=5)
        # Gunakan X_base MENTAH. Pipeline akan melakukan scaling aman di dalam tiap lipatan CV!
        scores = cross_val_score(pipe, X_base, y, groups=groups, cv=sgkf, scoring='accuracy')
        
        fold_str = ', '.join([f'{s*100:.0f}%' for s in scores])
        print(f"{cfg['name']:<20} | {scores.mean()*100:>7.1f}% | {scores.std()*100:>6.1f}% | [{fold_str}]")
        
        if scores.mean() > best_score:
            best_score = scores.mean()
            best_config = cfg
            best_name = cfg['name']
    
    print("─" * 70)
    print(f"Terbaik: {best_name} ({best_score*100:.1f}%)\n")
    
    # ── Final Model: Train on Holdout (Subject-Wise) ──
    print(f"{'=' * 70}")
    print("FINAL: Train & Holdout Test (Subject-Wise)")
    print("=" * 70)
    
    sgkf_holdout = StratifiedGroupKFold(n_splits=4, shuffle=True, random_state=42)
    train_idx, test_idx = next(sgkf_holdout.split(X_base, y, groups))
    
    X_train, X_test = X_base[train_idx], X_base[test_idx]
    y_train, y_test = y[train_idx], y[test_idx]
    
    # Skala khusus untuk Holdout
    final_scaler = StandardScaler()
    X_train_scaled = final_scaler.fit_transform(X_train)
    X_test_scaled = final_scaler.transform(X_test)
    
    # PATCH 2: CETAK PARAMETER SCALING UNTUK HARDCODE DI ESP32 C++
    print("\n[WAJIB SALIN KE ESP32] Parameter StandardScaler (Z-Score Normalization):")
    features = ['ir_ratio', 'red_ratio', 'heart_rate', 'crest_time_ratio']
    for i, feat in enumerate(features):
        print(f"  float mean_{feat} = {final_scaler.mean_[i]:.6f}f;")
        print(f"  float std_{feat}  = {final_scaler.scale_[i]:.6f}f;")
    print("-" * 50)
    
    clf_final = SVC(
        kernel=best_config['kernel'],
        C=best_config['C'],
        gamma=best_config['gamma'],
        class_weight='balanced',
        random_state=42
    )
    clf_final.fit(X_train_scaled, y_train)
    
    preds = clf_final.predict(X_test_scaled)
    acc = accuracy_score(y_test, preds)
    
    print(f"\nAkurasi Holdout (Unseen): {acc*100:.2f}%")
    print("\nClassification Report:")
    print(classification_report(y_test, preds, labels=[0, 1], target_names=["Non-Hipertensi", "Hipertensi"], zero_division=0))
    
    cm = confusion_matrix(y_test, preds, labels=[0, 1])
    print(f"Confusion Matrix:")
    print(f"                  Prediksi")
    print(f"                  Non-HT  HT")
    print(f"  Aktual Non-HT  [{cm[0][0]:4d}  {cm[0][1]:4d}]")
    print(f"  Aktual HT      [{cm[1][0]:4d}  {cm[1][1]:4d}]")
    
    # ── Ekspor ke C++ ──
    print("\nExtracting and porting SVM to C++ ...")
    try:
        cpp_code = port(clf_final)
        with open(MODEL_OUTPUT_PATH, 'w') as f:
            f.write(cpp_code)
        size_kb = os.path.getsize(MODEL_OUTPUT_PATH) / 1024
        print(f"[INFO] SVM berhasil diekspor ke: {MODEL_OUTPUT_PATH}")
        print(f"[INFO] Ukuran Sketch (Estimasi): {size_kb:.1f} KB")
    except Exception as e:
        print(f"[ERROR] Gagal mengekspor C++: {e}")

if __name__ == "__main__":
    main()
