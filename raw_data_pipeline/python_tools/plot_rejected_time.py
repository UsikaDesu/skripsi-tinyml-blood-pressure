import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal
import os

def plot_rejected_time_cv_window():
    file_path = '../dataset/raw_data_20260502_101327_bp116_83.csv'
    df = pd.read_csv(file_path)
    
    # Window 1 (indices 0 to 1000)
    ir_raw = df['ir_raw'].values[0:1000]
    
    # Detrend & Filter
    ir_detrended = signal.detrend(ir_raw)
    nyq = 0.5 * 100
    b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
    ir_filtered = signal.filtfilt(b, a, ir_detrended)
    
    # Find Peaks
    peaks, _ = signal.find_peaks(ir_filtered, distance=50, prominence=np.std(ir_filtered)*0.5)
    rr_intervals = np.diff(peaks)
    
    # Plot
    plt.figure(figsize=(12, 5))
    plt.plot(ir_filtered, label='IR Filtered (0.5-5Hz)', color='blue')
    plt.plot(peaks, ir_filtered[peaks], "rx", markersize=10, label='Detected Peaks')
    
    # Annotate RR intervals (Time between peaks in seconds)
    for i in range(len(peaks)-1):
        mid_x = (peaks[i] + peaks[i+1]) / 2
        # Tepat di tengah-tengah garis hijau
        mid_y = (ir_filtered[peaks[i]] + ir_filtered[peaks[i+1]]) / 2
        interval_sec = rr_intervals[i] * 0.01 # Convert 10ms samples to seconds
        plt.annotate(f"{interval_sec:.2f} s", (mid_x, mid_y), ha='center', va='center', fontsize=11, color='green', bbox=dict(facecolor='white', edgecolor='green', alpha=0.9))
        # Draw line to connect peaks
        plt.plot([peaks[i], peaks[i+1]], [ir_filtered[peaks[i]], ir_filtered[peaks[i+1]]], color='green', linestyle='--', alpha=0.5)
        
    mean_rr = np.mean(rr_intervals)
    cv = np.std(rr_intervals) / mean_rr
    
    plt.title(f'REJECTED: Jitter / Motion Artifact Detected\nTime CV: {cv*100:.1f}% (Limit: 12%)')
    plt.xlabel('Sample Index (10ms)')
    plt.ylabel('Amplitude')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('rejected_time_plot.png')
    print("Plot saved to rejected_time_plot.png")

if __name__ == "__main__":
    plot_rejected_time_cv_window()
