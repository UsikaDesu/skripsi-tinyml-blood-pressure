import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, f1_score

def main():
    df = pd.read_csv('../dataset/extracted_features.csv')
    
    # Use all 4 features
    X = df[['ir_mean', 'red_mean', 'ir_std', 'red_std']]
    y = df['bp_class']
    
    print("Testing Random Forest (50 Trees, All Features) 10x with Random Seeds\n")
    print(f"{'Run':<5} | {'Test Acc (%)':<15} | {'F1 Non-HT':<12} | {'F1 HT':<10}")
    print("-" * 50)
    
    acc_list = []
    f1_0_list = []
    f1_1_list = []
    
    for i in range(10):
        # No random_state = truly random split each time
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
        
        # No random_state = truly random forest initialization
        rf = RandomForestClassifier(
            n_estimators=50,
            max_depth=10,
            min_samples_split=2,
            min_samples_leaf=1
        )
        
        rf.fit(X_train, y_train)
        y_pred = rf.predict(X_test)
        
        acc = accuracy_score(y_test, y_pred) * 100
        
        # Calculate F1 for both classes safely
        f1_scores = f1_score(y_test, y_pred, average=None, labels=[0, 1])
        f1_0 = f1_scores[0] if len(f1_scores) > 0 else 0
        f1_1 = f1_scores[1] if len(f1_scores) > 1 else 0
        
        acc_list.append(acc)
        f1_0_list.append(f1_0)
        f1_1_list.append(f1_1)
        
        print(f"{i+1:<5} | {acc:<15.2f} | {f1_0:<12.2f} | {f1_1:<10.2f}")
        
    print("-" * 50)
    print(f"AVG   | {np.mean(acc_list):<15.2f} | {np.mean(f1_0_list):<12.2f} | {np.mean(f1_1_list):<10.2f}")
    print(f"MAX   | {np.max(acc_list):<15.2f} | {np.max(f1_0_list):<12.2f} | {np.max(f1_1_list):<10.2f}")
    print(f"MIN   | {np.min(acc_list):<15.2f} | {np.min(f1_0_list):<12.2f} | {np.min(f1_1_list):<10.2f}")

if __name__ == "__main__":
    main()
