import os
import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
from micromlgen import port
from collections import Counter

# ==========================================
# CONFIGURATION
# ==========================================
DATASET_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset', 'extracted_features.csv')
OUTPUT_HEADER = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'bp_model.h')

# Dosen's request: Exactly 3 features.
# User feedback: Focus purely on optical features (mean and std).
FEATURES = ['ir_mean', 'red_mean', 'ir_std']

def evaluate_models():
    print("======================================================================")
    print("TINYML ALGORITHM BENCHMARK: RANDOM FOREST vs SVM (3 FEATURES)")
    print("======================================================================\n")

    if not os.path.exists(DATASET_FILE):
        print(f"ERROR: Dataset {DATASET_FILE} not found!")
        return

    # 1. Load Data
    df = pd.read_csv(DATASET_FILE)
    print(f"Loaded {len(df)} samples from dataset.")
    
    missing_cols = [f for f in FEATURES if f not in df.columns]
    if missing_cols:
        print(f"ERROR: Missing columns: {missing_cols}")
        return
        
    if 'bp_class' not in df.columns:
        print("ERROR: Label column 'bp_class' not found in dataset!")
        return

    print("Using exactly 3 features requested:", FEATURES)

    X = df[FEATURES].values
    y = df['bp_class'].values

    # Check class distribution
    class_counts = Counter(y)
    print("\nClass Distribution:")
    for cls_label, count in class_counts.items():
        print(f" - Class {cls_label}: {count} samples")

    # 2. Split Data (Fallback to No-Split if extremely small dataset)
    if len(df) < 10:
        print("\nWARNING: Dataset is extremely small. Using entire dataset for both training and testing!")
        X_train, X_test = X, X
        y_train, y_test = y, y
    else:
        try:
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)
        except ValueError:
            print("\nWARNING: Not enough samples per class for stratification. Splitting without stratify.")
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

    # 3. Train Both Models
    print("\n--- Training Phase ---")
    
    # Model 1: Random Forest
    rf_clf = RandomForestClassifier(n_estimators=30, max_depth=5, random_state=42)
    rf_clf.fit(X_train, y_train)
    rf_preds = rf_clf.predict(X_test)
    rf_acc = accuracy_score(y_test, rf_preds)
    print(f"[Model 1] Random Forest Accuracy: {rf_acc * 100:.2f}%")

    # Model 2: Support Vector Machine (Linear Kernel works best with micromlgen)
    svm_clf = SVC(kernel='linear', C=1.0, random_state=42)
    svm_clf.fit(X_train, y_train)
    svm_preds = svm_clf.predict(X_test)
    svm_acc = accuracy_score(y_test, svm_preds)
    print(f"[Model 2] SVM (Linear) Accuracy : {svm_acc * 100:.2f}%")

    # 4. Compare and Select Winner
    print("\n--- Battle Result ---")
    winner_name = ""
    winner_clf = None
    winner_acc = 0.0

    if svm_acc > rf_acc:
        winner_name = "SVM (Linear)"
        winner_clf = svm_clf
        winner_acc = svm_acc
        print(f"[Winner] {winner_name} wins by {((svm_acc - rf_acc) * 100):.2f}% margin!")
    else:
        winner_name = "Random Forest"
        winner_clf = rf_clf
        winner_acc = rf_acc
        if rf_acc == svm_acc:
            print("[Tie-Breaker] Both have equal accuracy. Selecting Random Forest by default.")
        else:
            print(f"[Winner] {winner_name} wins by {((rf_acc - svm_acc) * 100):.2f}% margin!")

    # 5. Export to C++ Header
    print("\n--- ESP32 Model Porting ---")
    print(f"Exporting the winning model ({winner_name}) to C++...")
    
    try:
        # Generate C++ code
        c_code = port(winner_clf, classmap={0: "Hypotension", 1: "Normal", 2: "Hypertension"})
        
        # Write to bp_model.h
        with open(OUTPUT_HEADER, 'w') as f:
            f.write(c_code)
            
        print("[SUCCESS] Model ported to C++!")
        print(f"[SUCCESS] Header File Saved: {OUTPUT_HEADER}")
        
    except Exception as e:
        print(f"[ERROR] Failed to export model: {e}")

if __name__ == '__main__':
    evaluate_models()
