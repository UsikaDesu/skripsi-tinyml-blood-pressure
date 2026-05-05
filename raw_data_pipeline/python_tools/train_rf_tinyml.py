import os
import glob
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import classification_report, accuracy_score, confusion_matrix
from micromlgen import port

# ==========================================
# CONFIGURATION
# ==========================================
DATA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset', 'extracted_features.csv')
MODEL_OUTPUT_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'rf_model.h')

FEATURES = ['ir_mean', 'red_mean', 'ir_std', 'red_std']

def load_data():
    if not os.path.exists(DATA_PATH):
        print(f"ERROR: Dataset not found at {DATA_PATH}")
        print("Please run extract_features.py first!")
        return None
        
    print(f"Loading isolated extracted dataset from {DATA_PATH}...")
    
    try:
        combined_df = pd.read_csv(DATA_PATH)
        print(f"  + Loaded {len(combined_df)} raw-extracted samples")
    except Exception as e:
        print(f"  - Failed to load dataset: {e}")
        return None
    
    # bp_class sudah dihitung oleh extract_features.py (WHO/Indonesia Standard)
    # Kelas 0 = Non-Hipertensi, Kelas 1 = Hipertensi (Systolic>=140 ATAU Diastolic>=90)
    
    # Clean NaNs
    combined_df.dropna(subset=FEATURES + ['bp_class'], inplace=True)
    
    print(f"\nTotal usable samples: {len(combined_df)}")
    print("Class Distribution:")
    print(combined_df['bp_class'].value_counts().sort_index())
    
    X = combined_df[FEATURES].values
    y = combined_df['bp_class'].values
    
    return X, y

def train_and_export():
    print("=" * 70)
    print("TINYML RANDOM FOREST PIPELINE")
    print("=" * 70)
    
    data = load_data()
    if data is None:
        return
        
    X, y = data
    
    # Train-test split (handle very small datasets gracefully)
    if len(X) < 5:
        print("WARNING: Dataset is extremely small. Using entire dataset for both training and testing!")
        X_train, X_test, y_train, y_test = X, X, y, y
    else:
        try:
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)
        except ValueError:
            # Drop stratify if classes are too few
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    
    print(f"\nTraining on {len(X_train)} samples, Testing on {len(X_test)} samples...")
    
    # Instantiate Random Forest with class_weight='balanced'
    # class_weight='balanced' mengatasi ketimpangan data (85:15)
    # n_estimators kecil agar muat di ESP32 Flash memory
    clf = RandomForestClassifier(n_estimators=30, max_depth=10, random_state=42, class_weight='balanced')
    clf.fit(X_train, y_train)
    
    # Evaluation
    preds = clf.predict(X_test)
    accuracy = accuracy_score(y_test, preds)
    
    print("\n--- Model Evaluation ---")
    print(f"Accuracy: {accuracy * 100:.2f}%\n")
    print("Classification Report (WHO/Indonesia Standard):")
    print(classification_report(y_test, preds, labels=[0, 1], target_names=["Non-Hipertensi", "Hipertensi"], zero_division=0))
    
    print("Confusion Matrix:")
    cm = confusion_matrix(y_test, preds, labels=[0, 1])
    print(f"                  Prediksi")
    print(f"                  Non-HT  HT")
    print(f"  Aktual Non-HT  [{cm[0][0]:4d}  {cm[0][1]:4d}]")
    print(f"  Aktual HT      [{cm[1][0]:4d}  {cm[1][1]:4d}]")
    
    # Cross-Validation (anti-overfitting check)
    print("\n--- Cross-Validation (5-Fold) ---")
    cv_scores = cross_val_score(clf, X, y, cv=5, scoring='accuracy')
    print(f"Skor per lipatan: {[f'{s*100:.1f}%' for s in cv_scores]}")
    print(f"Rata-rata CV: {cv_scores.mean()*100:.2f}% (+/- {cv_scores.std()*100:.2f}%)")
    
    # Feature Importance
    print("\n--- Feature Importance ---")
    for feat, imp in sorted(zip(FEATURES, clf.feature_importances_), key=lambda x: -x[1]):
        bar = '#' * int(imp * 40)
        print(f"  {feat:10s}: {imp:.3f} {bar}")
    
    # Generate C++ Code Using micromlgen
    print("\nExtracting and porting model to C++ ...")
    try:
        # Port model
        # Target representation allows you to specify class map, etc if desired
        cpp_code = port(clf)
        
        # Save to header file
        with open(MODEL_OUTPUT_PATH, 'w') as f:
            f.write(cpp_code)
            
        # Get file size in KB
        size_kb = os.path.getsize(MODEL_OUTPUT_PATH) / 1024
        
        print(f"\n[INFO] Random forest successfully ported to C++!")
        print(f"[INFO] Header File: {MODEL_OUTPUT_PATH}")
        print(f"[INFO] Sketch size estimate: {size_kb:.1f} KB (Suitable for ESP32)")
        
    except Exception as e:
        print(f"\n[ERROR] Failed to export to C++: {e}")

if __name__ == "__main__":
    train_and_export()
