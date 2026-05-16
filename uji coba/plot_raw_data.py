import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
import shutil
import scipy.signal as signal

# ==========================================
# CONFIGURATION
# ==========================================
DATASET_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset')
OUTPUT_IMG = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'raw_signal_plot.png')
ARTIFACT_IMG = r'C:\Users\Kayyis\.gemini\antigravity\brain\0cbcc73c-da4f-4d41-acf5-bc7850b899c2\raw_signal_plot.png'

def plot_raw_data():
    csv_files = glob.glob(os.path.join(DATASET_DIR, 'raw_data_*.csv'))
    if not csv_files:
        print("No raw data files found.")
        return
        
    print("=" * 60)
    print("Available Raw Data Files:")
    for i, file in enumerate(csv_files):
        print(f"[{i+1}] {os.path.basename(file)}")
    print("=" * 60)
    
    choice = input(f"\nSelect a file to plot [1-{len(csv_files)}]: ").strip()
    try:
        idx = int(choice) - 1
        if idx < 0 or idx >= len(csv_files):
            print("Invalid selection.")
            return
        latest_file = csv_files[idx]
    except ValueError:
        print("Action cancelled.")
        return
        
    print(f"\nPlotting data from: {os.path.basename(latest_file)}")
    
    df = pd.read_csv(latest_file)
    
    # Plotting the entire data until the end of the collection
    plot_limit = len(df)
    
    time_sec = (df['time_ms'].iloc[:plot_limit] - df['time_ms'].iloc[0]) / 1000.0
    ir_data = df['ir_raw'].iloc[:plot_limit]
    red_data = df['red_raw'].iloc[:plot_limit]
    
    fs = 100.0
    nyq = 0.5 * fs
    b, a = signal.butter(2, [0.5 / nyq, 5.0 / nyq], btype='band')
    
    ir_detrended = signal.detrend(ir_data)
    ir_filtered = signal.filtfilt(b, a, ir_detrended)
    
    red_detrended = signal.detrend(red_data)
    
    plt.figure(figsize=(12, 12))
    
    # 1. Plot IR Signal (Raw)
    plt.subplot(4, 1, 1)
    plt.plot(time_sec, ir_data, color='blue', label='IR Signal (Pulse) - RAW')
    plt.title(f'Photoplethysmogram (PPG) - {os.path.basename(latest_file)}')
    plt.ylabel('IR Amplitude')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    # 2. Plot IR Signal (Processed)
    plt.subplot(4, 1, 2)
    plt.plot(time_sec, ir_filtered, color='purple', label='IR Signal (Pulse) - DETRENDED + BANDPASS (Ready for AI)')
    plt.ylabel('IR Filtered')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    # 3. Plot RED Signal (Raw)
    plt.subplot(4, 1, 3)
    plt.plot(time_sec, red_data, color='red', label='RED Signal (SpO2) - RAW')
    plt.ylabel('RED Amplitude')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    # 4. Plot RED Signal (Detrended)
    plt.subplot(4, 1, 4)
    plt.plot(time_sec, red_detrended, color='orange', label='RED Signal (SpO2) - DETRENDED ONLY')
    plt.xlabel('Time (Seconds)')
    plt.ylabel('RED Detrended')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    plt.tight_layout()
    plt.savefig(OUTPUT_IMG, dpi=150)
    print(f"Saved plot locally to: {OUTPUT_IMG}")
    
    # Copy to artifact directory for AI to view/embed
    try:
        shutil.copy(OUTPUT_IMG, ARTIFACT_IMG)
        print(f"Saved plot to artifacts: {ARTIFACT_IMG}")
    except Exception as e:
        print(f"Artifact copy error: {e}")

if __name__ == '__main__':
    plot_raw_data()
