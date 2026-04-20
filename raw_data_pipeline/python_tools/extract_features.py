import os
import glob
import pandas as pd
import numpy as np
import scipy.signal as signal

# ==========================================
# CONFIGURATION
# ==========================================
RAW_DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset')
OUTPUT_FILE = os.path.join(RAW_DATA_DIR, 'extracted_features.csv')
SAMPLING_RATE_HZ = 100  # Based on 10ms delta in raw data

def calculate_hr_spo2(ir_raw, red_raw, fs=100):
    """
    Simplex feature extraction for Heart Rate and SpO2 from raw PPG.
    In a real TinyML setup, you want this to mimic the ESP32 algorithm as closely as possible.
    """
    # Detrend signals
    ir_detrended = signal.detrend(ir_raw)
    
    # Bandpass filter for Heart Rate (0.5 - 5 Hz corresponds to 30 - 300 bpm)
    nyq = 0.5 * fs
    b, a = signal.butter(2, [0.5 / nyq, 5.0 / nyq], btype='band')
    ir_filtered = signal.filtfilt(b, a, ir_detrended)
    
    # Find peaks for Heart Rate
    # Distance of fs/(220/60) means max HR of 220 bpm
    min_dist = fs / (220/60.0) 
    peaks, _ = signal.find_peaks(ir_filtered, distance=min_dist, prominence=np.std(ir_filtered)*0.5)
    
    heart_rate = 0
    if len(peaks) > 1:
        # Calculate mean peak-to-peak interval in seconds
        rr_intervals = np.diff(peaks) / fs
        mean_rr = np.mean(rr_intervals)
        heart_rate = 60.0 / mean_rr
        
    # Calculate SpO2 using simplified AC/DC ratio
    ac_red = np.std(red_raw)
    dc_red = np.mean(red_raw)
    ac_ir = np.std(ir_raw)
    dc_ir = np.mean(ir_raw)
    
    spo2 = 0
    if dc_red > 0 and dc_ir > 0 and ac_ir > 0:
        ratio = (ac_red / dc_red) / (ac_ir / dc_ir)
        # Empirical SpO2 formula (often tuned per sensor)
        spo2 = 110.0 - 25.0 * ratio
        # Cap SpO2 to realistic values
        spo2 = np.clip(spo2, 0, 100)
        
    return heart_rate, spo2

def load_and_extract_features():
    print("=" * 70)
    print("RAW DATA FEATURE EXTRACTION (1 File = 1 Window/Sample)")
    print("=" * 70)
    
    csv_files = glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv'))
    if not csv_files:
        print(f"No raw data files found in {RAW_DATA_DIR}")
        return
        
    print(f"Found {len(csv_files)} files. Extracting features...\n")
    
    features_list = []
    
    for file in csv_files:
        try:
            df = pd.read_csv(file)
            if len(df) < 500: # Need at least 5 seconds of data at 100Hz
                print(f"Skipping {os.path.basename(file)}: Not enough samples ({len(df)}).")
                continue
                
            # Process the whole 25-second window
            ir_raw = df['ir_raw'].values
            red_raw = df['red_raw'].values
            
            # Apply Detrending to ensure accurate AC fluctuation readings
            ir_detrended = signal.detrend(ir_raw)
            red_detrended = signal.detrend(red_raw)
            
            # Ground truth is duplicated on every row, just take the first
            systolic = df['systolic_label'].iloc[0]
            diastolic = df['diastolic_label'].iloc[0]
            
            # 1. Statistical Features
            # Mean is taken from RAW to preserve the baseline DC volume offset
            ir_mean = int(np.round(np.mean(ir_raw)))
            red_mean = int(np.round(np.mean(red_raw)))
            # Standard Deviation is taken from DETRENDED data to measure pure AC pulse without wander slope
            ir_std = int(np.round(np.std(ir_detrended)))
            red_std = int(np.round(np.std(red_detrended)))
            
            # 2. Physiological Features
            hr, spo2 = calculate_hr_spo2(ir_raw, red_raw, fs=SAMPLING_RATE_HZ)
            hr = int(np.round(hr))
            spo2 = int(np.round(spo2))
            
            if hr < 40 or hr > 200:
                print(f"Warning in {os.path.basename(file)}: HR {hr:.1f} seems invalid. Still retaining.")
            
            notes = "extracted_raw"
            
            # Determine classification class
            # 0: Hypotension, 1: Normal, 2: Hypertension
            if systolic >= 130 or diastolic >= 85:
                bp_class = 2  # Hypertension
            elif systolic <= 90 or diastolic <= 60:
                bp_class = 0  # Hypotension
            else:
                bp_class = 1  # Normal
            
            features_list.append({
                'timestamp': int(df['time_ms'].iloc[0]), # Start timestamp
                'ir_mean': ir_mean,
                'red_mean': red_mean,
                'ir_std': ir_std,
                'red_std': red_std,
                'heart_rate': hr,
                'spo2': spo2,
                'valid': 1,
                'systolic_bp': systolic,
                'diastolic_bp': diastolic,
                'bp_class': bp_class,
                'notes': notes
            })
            
            print(f"[OK] Processed {os.path.basename(file)} -> BP: {systolic}/{diastolic} (Class {bp_class}), HR: {hr:.1f}")
            
        except Exception as e:
            print(f"[ERROR] Failed to process {file}: {e}")
            
    if not features_list:
        print("No features extracted.")
        return
        
    out_df = pd.DataFrame(features_list)
    
    # Save to the training_data folder so it can be merged with existing datasets
    os.makedirs(os.path.dirname(OUTPUT_FILE), exist_ok=True)
    out_df.to_csv(OUTPUT_FILE, index=False)
    
    print("\n" + "=" * 70)
    print(f"Extraction Successful! Processed {len(out_df)} 25-second windows.")
    print(f"Saved extracted features to: {OUTPUT_FILE}")
    print("=" * 70)

if __name__ == "__main__":
    load_and_extract_features()
