import os
import glob
import pandas as pd
import numpy as np
import scipy.signal as signal

RAW_DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset')

def main():
    raw_files = glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv'))
    
    results = []
    
    for file in sorted(raw_files):
        try:
            df = pd.read_csv(file)
            if len(df) < 500:
                continue
                
            window_size = 1000
            num_windows = len(df) // window_size
            
            for w in range(num_windows):
                start_idx = w * window_size
                end_idx = start_idx + window_size
                
                ir_raw = df['ir_raw'].values[start_idx:end_idx]
                ir_detrended = signal.detrend(ir_raw)
                
                nyq = 0.5 * 100
                b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
                ir_filtered = signal.filtfilt(b, a, ir_detrended)
                
                peaks, _ = signal.find_peaks(ir_filtered, distance=50, prominence=np.std(ir_filtered)*0.5)
                
                if len(peaks) < 2:
                    cv = -1.0 # Flatline
                else:
                    rr_intervals = np.diff(peaks)
                    mean_rr = np.mean(rr_intervals)
                    if mean_rr == 0:
                        cv = -1.0
                    else:
                        cv = np.std(rr_intervals) / mean_rr
                
                status = "REJECTED (Flatline)" if cv == -1.0 else ("REJECTED (CV > 12%)" if cv > 0.12 else "ACCEPTED")
                
                results.append({
                    'File': os.path.basename(file),
                    'Window': w + 1,
                    'CV (%)': "Flatline" if cv == -1.0 else f"{cv*100:.2f}%",
                    'Status': status
                })
        except Exception as e:
            print(f"Error processing {file}: {e}")
            
    df_results = pd.DataFrame(results)
    output_path = os.path.join(RAW_DATA_DIR, 'cv_distribution.csv')
    df_results.to_csv(output_path, index=False)
    
    # Also print summary
    print(f"Total Windows: {len(df_results)}")
    print(f"Accepted: {len(df_results[df_results['Status'] == 'ACCEPTED'])}")
    print(f"Rejected (CV > 12%): {len(df_results[df_results['Status'] == 'REJECTED (CV > 12%)'])}")
    print(f"Rejected (Flatline): {len(df_results[df_results['Status'] == 'REJECTED (Flatline)'])}")
    
    # Print markdown table for the AI to read easily
    print("\n--- ALL WINDOWS ---")
    print(df_results.to_string(index=False))

if __name__ == "__main__":
    main()
