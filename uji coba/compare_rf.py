import os
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import accuracy_score, f1_score
from micromlgen import port
import warnings
warnings.filterwarnings('ignore')

DATA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset', 'extracted_features.csv')

def get_model_size_kb(clf):
    try:
        cpp_code = port(clf)
        return len(cpp_code) / 1024
    except:
        return 0

def evaluate_model(n_trees, feature_list, X, y):
    clf = RandomForestClassifier(
        n_estimators=n_trees,
        max_depth=5,
        min_samples_leaf=2,
        class_weight='balanced',
        random_state=42
    )
    
    # Cross validation
    cv_scores = cross_val_score(clf, X, y, cv=5, scoring='accuracy')
    cv_acc = cv_scores.mean() * 100
    
    # Train-test split
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)
    clf.fit(X_train, y_train)
    y_pred = clf.predict(X_test)
    
    test_acc = accuracy_score(y_test, y_pred) * 100
    
    # F1 Scores (0: Non-HT, 1: HT)
    f1_non_ht = f1_score(y_test, y_pred, pos_label=0)
    f1_ht = f1_score(y_test, y_pred, pos_label=1)
    
    # Size
    size_kb = get_model_size_kb(clf)
    
    return cv_acc, test_acc, f1_non_ht, f1_ht, size_kb

def main():
    df = pd.read_csv(DATA_PATH)
    
    features = ['ir_mean', 'red_mean', 'ir_std', 'red_std']
    y = df['bp_class'].values
    
    results = []
    
    configs = [
        ("All Features", features),
        ("ir_mean only", ['ir_mean']),
        ("red_mean only", ['red_mean']),
        ("ir_std only", ['ir_std']),
        ("red_std only", ['red_std'])
    ]
    
    trees_to_test = [30, 50]
    
    for feat_name, feat_cols in configs:
        X = df[feat_cols].values
        
        for n_trees in trees_to_test:
            cv_acc, test_acc, f1_non_ht, f1_ht, size_kb = evaluate_model(n_trees, feat_cols, X, y)
            
            results.append({
                'Features': feat_name,
                'Trees': n_trees,
                'CV Acc (%)': f"{cv_acc:.1f}",
                'Test Acc (%)': f"{test_acc:.1f}",
                'F1 Non-HT': f"{f1_non_ht:.2f}",
                'F1 HT': f"{f1_ht:.2f}",
                'Size (KB)': f"{size_kb:.1f}"
            })
            
    df_results = pd.DataFrame(results)
    print(df_results.to_string(index=False))

if __name__ == "__main__":
    main()
