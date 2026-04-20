"""
Data Augmentation Tool for Blood Pressure Estimation
Generates artificial synthetic data based on real collected samples
using Gaussian noise jittering.
"""

import pandas as pd
import numpy as np
import glob
import os

DATA_FOLDER = 'training_data'
SYNTHETIC_FILE = os.path.join(DATA_FOLDER, 'bp_data_synthetic.csv')

def load_real_data():
    """Load all REAL CSV files from training_data folder"""
    csv_files = glob.glob(os.path.join(DATA_FOLDER, '*.csv'))
    
    # Exclude any previously generated synthetic files so we don't multiply synthetic on synthetic
    csv_files = [f for f in csv_files if 'synthetic' not in f]
    
    if not csv_files:
        print(f"ERROR: No real data files found in {DATA_FOLDER}/")
        return None
    
    all_data = []
    total_samples = 0
    for file in csv_files:
        try:
            df = pd.read_csv(file)
            all_data.append(df)
            total_samples += len(df)
        except Exception as e:
            pass
            
    if not all_data:
        return None
        
    combined_df = pd.concat(all_data, ignore_index=True)
    return combined_df

def generate_synthetic_data(df, multiplier=5):
    """Generate artificial samples using statistical jittering"""
    # Clean data so we only augment good readings
    df = df.dropna(subset=['systolic_bp', 'diastolic_bp'])
    df = df[df['valid'] == 1].copy()
    
    if len(df) == 0:
        print("ERROR: No valid, labeled real samples found in your CSVs!")
        return None
        
    print(f"\nAnalyzed {len(df)} real valid samples.")
    print(f"Generating {multiplier} artificial variations for each real sample...")
    
    synthetic_dfs = []
    feature_cols = ['ir_mean', 'red_mean', 'ir_std', 'red_std', 'heart_rate', 'spo2']
    
    for _ in range(multiplier):
        # --- 1. Jitter Normal Data (Create realistic variations of your own BP) ---
        synth_df = df.copy()
        for col in feature_cols:
            std_dev = df[col].std() if df[col].std() > 0 else (df[col].mean() * 0.05)
            noise = np.random.normal(0, std_dev * 0.05, size=len(df))
            synth_df[col] = synth_df[col] + noise
            
        synth_df['systolic_bp'] = synth_df['systolic_bp'] + np.random.randint(-3, 4, size=len(df))
        synth_df['diastolic_bp'] = synth_df['diastolic_bp'] + np.random.randint(-2, 3, size=len(df))
        synth_df['notes'] = "synthetic_normal"
        synthetic_dfs.append(synth_df)
        
        # --- 2. Force Inject HYPERTENSION Data (Class 2) ---
        high_df = df.copy()
        # High BP creates much stronger, bouncier pulses (higher Standard Deviation)
        # Sesuai grafik korelasi: Tens naik = std NAIK, mean TURUN, HR NAIK
        high_df['ir_std'] = high_df['ir_std'] * np.random.uniform(1.2, 1.5, size=len(df))
        high_df['red_std'] = high_df['red_std'] * np.random.uniform(1.2, 1.5, size=len(df))
        high_df['ir_mean'] = high_df['ir_mean'] * np.random.uniform(0.80, 0.95, size=len(df))
        high_df['red_mean'] = high_df['red_mean'] * np.random.uniform(0.80, 0.95, size=len(df))
        high_df['heart_rate'] = high_df['heart_rate'] * np.random.uniform(1.1, 1.3, size=len(df))
        
        # Force label them securely into hypertension territory (>130 / >85)
        high_df['systolic_bp'] = np.random.randint(135, 175, size=len(df))
        high_df['diastolic_bp'] = np.random.randint(85, 110, size=len(df))
        high_df['notes'] = "synthetic_hypertension"
        synthetic_dfs.append(high_df)
        
        # --- 3. Force Inject HYPOTENSION Data (Class 0) ---
        low_df = df.copy()
        # Low BP creates very weak, shallow pulses (lower Standard Deviation)
        # Sesuai grafik korelasi: Tensi anjlok = std TURUN, mean NAIK, HR TURUN
        low_df['ir_std'] = low_df['ir_std'] * np.random.uniform(0.6, 0.8, size=len(df))
        low_df['red_std'] = low_df['red_std'] * np.random.uniform(0.6, 0.8, size=len(df))
        low_df['ir_mean'] = low_df['ir_mean'] * np.random.uniform(1.05, 1.20, size=len(df))
        low_df['red_mean'] = low_df['red_mean'] * np.random.uniform(1.05, 1.20, size=len(df))
        low_df['heart_rate'] = low_df['heart_rate'] * np.random.uniform(0.7, 0.9, size=len(df))
        
        # Force label them securely into hypotension territory (<90 / <60)
        low_df['systolic_bp'] = np.random.randint(70, 88, size=len(df))
        low_df['diastolic_bp'] = np.random.randint(45, 58, size=len(df))
        low_df['notes'] = "synthetic_hypotension"
        synthetic_dfs.append(low_df)
        
    # Combine all the synthetic frames
    final_synthetic = pd.concat(synthetic_dfs, ignore_index=True)
    
    # Round the feature columns to 2 decimal places to match the sensor's genuine output
    for col in feature_cols:
        final_synthetic[col] = final_synthetic[col].round(2)
        
    return final_synthetic

def main():
    print("=" * 70)
    print("Synthetic Data Generation Tool")
    print("=" * 70)
    
    df = load_real_data()
    if df is None:
        return
        
    print("This tool takes your REAL medical recordings and automatically generates")
    print("hundreds of mathematically probable artificial recordings to help train")
    print("your AI much faster (Data Augmentation).")
    
    try:
        user_input = input("\nHow many artificial copies do you want per real sample? (Default: 5): ").strip()
        multiplier = int(user_input) if user_input else 5
    except ValueError:
        multiplier = 5
        
    synth_df = generate_synthetic_data(df, multiplier)
    
    if synth_df is not None:
        synth_df.to_csv(SYNTHETIC_FILE, index=False)
        print("\n" + "=" * 70)
        print(f"✓ SUCCESS: Generated {len(synth_df)} artificial rows!")
        print(f"✓ Saved into: {SYNTHETIC_FILE}")
        print("=" * 70)
        print("\nYou can now run `python preprocess_data.py` and `python train_model.py`.")
        print("The pipeline will automatically safely load your real data AND this new")
        print("synthetic dataset to train a smarter AI!")

if __name__ == "__main__":
    main()
