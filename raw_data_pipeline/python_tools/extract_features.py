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
                
            # Windowing logic: 10 seconds per window (1000 samples at 100Hz)
            # This perfectly extracts 2 data points from a 25s file and safely discards the last 5 seconds.
            window_size = 1000
            num_windows = len(df) // window_size
            
            if num_windows == 0:
                print(f"Skipping {os.path.basename(file)}: Length ({len(df)}) is less than 8 seconds.")
                continue
                
            count_before = len(features_list)
            for w in range(num_windows):
                start_idx = w * window_size
                end_idx = start_idx + window_size
                
                ir_raw = df['ir_raw'].values[start_idx:end_idx]
                red_raw = df['red_raw'].values[start_idx:end_idx]
                
                # Apply Detrending to ensure accurate AC fluctuation readings
                ir_detrended = signal.detrend(ir_raw)
                red_detrended = signal.detrend(red_raw)
                
                # -------------------------------------------------------------
                # QUALITY ASSURANCE (QA) GATE: CV < 12%
                # -------------------------------------------------------------
                # We apply a strict bandpass filter to calculate pure AC (Cardiac) Amplitude
                nyq = 0.5 * SAMPLING_RATE_HZ
                b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
                ir_filtered = signal.filtfilt(b, a, ir_detrended)
                red_filtered = signal.filtfilt(b, a, red_detrended)
                
                peaks, _ = signal.find_peaks(ir_filtered, distance=50, prominence=np.std(ir_filtered)*0.5)
                
                if len(peaks) < 2:
                    print(f"  -> [REJECTED] Window {w+1}/{num_windows} in {os.path.basename(file)}: Flatline (No Peaks).")
                    continue
                    
                rr_intervals = np.diff(peaks)
                mean_rr = np.mean(rr_intervals)
                
                if mean_rr == 0:
                    continue
                    
                cv = np.std(rr_intervals) / mean_rr
                if cv > 0.12:
                    print(f"  -> [REJECTED] Window {w+1}/{num_windows} in {os.path.basename(file)}: CV {cv*100:.1f}% > 12% Limit (Jitter Detected).")
                    continue
                # -------------------------------------------------------------
                
                # Ground truth is duplicated on every row, just take the first
                systolic = df['systolic_label'].iloc[0]
                diastolic = df['diastolic_label'].iloc[0]
                
                # 1. Medical-Grade ML Features (Raw for Mean, Bandpass for STD)
                ir_mean = int(np.round(np.mean(ir_raw)))
                red_mean = int(np.round(np.mean(red_raw)))
                ir_std = int(np.round(np.std(ir_filtered)))
                red_std = int(np.round(np.std(red_filtered)))
                
                notes = f"win_{w+1}_of_{num_windows}"
                
                # Determine classification class (WHO/Indonesia Standard)
                # Hipertensi: Systolic >= 140 ATAU Diastolic >= 90
                if systolic >= 140 or diastolic >= 90:
                    bp_class = 1  # Hipertensi
                else:
                    bp_class = 0  # Non-Hipertensi
                
                features_list.append({
                    'timestamp': int(df['time_ms'].iloc[start_idx]), # Start timestamp of window
                    'ir_mean': ir_mean,
                    'red_mean': red_mean,
                    'ir_std': ir_std,
                    'red_std': red_std,
                    'valid': 1,
                    'systolic_bp': systolic,
                    'diastolic_bp': diastolic,
                    'bp_class': bp_class,
                    'notes': notes
                })
            
            # Simple & bulletproof: count by list length difference
            accepted = len(features_list) - count_before
            rejected = num_windows - accepted
            
            if rejected > 0:
                print(f"[OK] Processed {os.path.basename(file)} -> Accepted {accepted}/{num_windows} windows, Rejected {rejected} (BP: {systolic}/{diastolic}, Class {bp_class})")
            else:
                print(f"[OK] Processed {os.path.basename(file)} -> Accepted {accepted}/{num_windows} windows (BP: {systolic}/{diastolic}, Class {bp_class})")
            
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
