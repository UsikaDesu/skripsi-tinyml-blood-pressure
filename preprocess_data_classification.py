"""
Data Preprocessing for Blood Pressure Estimation
Loads, cleans, and prepares data for model training
"""

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
import os
import glob
import pickle

DATA_FOLDER = 'training_data'
PROCESSED_DATA_FILE = 'processed_data.pkl'

def load_all_data():
    """Load all CSV files from training_data folder"""
    csv_files = glob.glob(os.path.join(DATA_FOLDER, '*.csv'))
    
    if not csv_files:
        print(f"ERROR: No CSV files found in {DATA_FOLDER}/")
        print("Please collect data using data_logger.py first!")
        return None
    
    print(f"Found {len(csv_files)} data file(s)")
    
    all_data = []
    for file in csv_files:
        try:
            df = pd.read_csv(file)
            all_data.append(df)
            print(f"  ✓ Loaded {file}: {len(df)} samples")
        except Exception as e:
            print(f"  ✗ Error loading {file}: {e}")
    
    if not all_data:
        return None
    
    # Combine all data
    combined_df = pd.concat(all_data, ignore_index=True)
    print(f"\nTotal samples: {len(combined_df)}")
    
    return combined_df

def clean_data(df):
    """Remove invalid and outlier samples"""
    print("\nCleaning data...")
    original_count = len(df)
    
    # Remove rows with missing BP values
    df = df.dropna(subset=['systolic_bp', 'diastolic_bp'])
    print(f"  After removing missing BP: {len(df)} samples")
    
    # Remove invalid sensor readings (valid = 0)
    df = df[df['valid'] == 1]
    print(f"  After removing invalid readings: {len(df)} samples")
    
    # Remove physiologically impossible BP values
    df = df[(df['systolic_bp'] >= 70) & (df['systolic_bp'] <= 200)]
    df = df[(df['diastolic_bp'] >= 40) & (df['diastolic_bp'] <= 130)]
    df = df[df['systolic_bp'] > df['diastolic_bp']]
    print(f"  After BP range filtering: {len(df)} samples")
    
    # Remove heart rate outliers
    df = df[(df['heart_rate'] >= 40) & (df['heart_rate'] <= 180)]
    print(f"  After HR filtering: {len(df)} samples")
    
    # Remove SpO2 outliers
    df = df[(df['spo2'] >= 85) & (df['spo2'] <= 100)]
    print(f"  After SpO2 filtering: {len(df)} samples")
    
    removed = original_count - len(df)
    print(f"\nRemoved {removed} invalid samples ({removed/original_count*100:.1f}%)")
    
    return df

def extract_features(df):
    """Extract features for model training"""
    print("\nExtracting features...")
    
    # Select sensor features
    feature_columns = [
        'ir_mean',
        'red_mean', 
        'ir_std',
        'red_std',
        'heart_rate',
        'spo2'
    ]
    
    X = df[feature_columns].values
    
    # Target variables (BP Classification)
    # 0 = Hypotension (<90/<60)
    # 2 = Hypertension (>=130/>=80)
    # 1 = Normal (in between)
    y_categorical = []
    
    y_systolic = df['systolic_bp'].values
    y_diastolic = df['diastolic_bp'].values
    
    for sys, dia in zip(y_systolic, y_diastolic):
        if sys >= 130 or dia >= 80:
            y_categorical.append(2)
        elif sys < 90 or dia < 60:
            y_categorical.append(0)
        else:
            y_categorical.append(1)
            
    y = np.array(y_categorical)
    
    print(f"  Features shape: {X.shape}")
    print(f"  Targets shape: {y.shape}")
    print(f"\nFeature statistics:")
    for i, col in enumerate(feature_columns):
        print(f"  {col:15s}: mean={X[:, i].mean():.2f}, std={X[:, i].std():.2f}")
    
    print(f"\nClass Distribution:")
    unique, counts = np.unique(y, return_counts=True)
    class_names = {0: "Hypotension", 1: "Normal", 2: "Hypertension"}
    for label, count in zip(unique, counts):
        print(f"  {class_names[label]}: {count} samples")
    
    return X, y, feature_columns

def normalize_data(X_train, X_test):
    """Normalize features using StandardScaler"""
    print("\nNormalizing data...")
    
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    
    print(f"  ✓ Data normalized using StandardScaler")
    
    return X_train_scaled, X_test_scaled, scaler

def preprocess_pipeline():
    """Main preprocessing pipeline"""
    print("=" * 70)
    print("Blood Pressure Data Preprocessing Pipeline")
    print("=" * 70)
    
    # Load data
    df = load_all_data()
    if df is None:
        return False
    
    # Clean data
    df_clean = clean_data(df)
    
    if len(df_clean) < 5:
        print("\n✗ ERROR: Not enough samples for training!")
        print("  Need at least 5 labeled samples.")
        print(f"  Currently have: {len(df_clean)} samples")
        print("\n  Recommendation: Collect more data using data_logger.py")
        return False
    
    # Extract features
    X, y, feature_names = extract_features(df_clean)
    
    # Split data (80% train, 20% test)
    test_size = 0.2
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=test_size, random_state=42, shuffle=True
    )
    
    print(f"\nData split:")
    print(f"  Training samples: {len(X_train)}")
    print(f"  Test samples: {len(X_test)}")
    
    # Normalize
    X_train_scaled, X_test_scaled, scaler = normalize_data(X_train, X_test)
    
    # Save processed data
    processed_data = {
        'X_train': X_train_scaled,
        'X_test': X_test_scaled,
        'y_train': y_train,
        'y_test': y_test,
        'scaler': scaler,
        'feature_names': feature_names
    }
    
    with open(PROCESSED_DATA_FILE, 'wb') as f:
        pickle.dump(processed_data, f)
    
    print(f"\n✓ Processed data saved to: {PROCESSED_DATA_FILE}")
    print("=" * 70)
    print("✓ Preprocessing complete! Ready for model training.")
    print("=" * 70)
    
    return True

if __name__ == "__main__":
    success = preprocess_pipeline()
    if not success:
        exit(1)
