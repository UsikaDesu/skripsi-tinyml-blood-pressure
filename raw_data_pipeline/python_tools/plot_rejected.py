import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal
import os

def plot_rejected_window():
    file_path = '../dataset/raw_data_20260424_055954_bp130_78.csv'
    df = pd.read_csv(file_path)
    
    # We want Window 1 (indices 0 to 1000)
    ir_raw = df['ir_raw'].values[0:1000]
    
    # Detrend & Filter
    ir_detrended = signal.detrend(ir_raw)
    nyq = 0.5 * 100
    b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
    ir_filtered = signal.filtfilt(b, a, ir_detrended)
    
    # Find Peaks
    peaks, _ = signal.find_peaks(ir_filtered, distance=50, prominence=np.std(ir_filtered)*0.5)
    amplitudes = ir_filtered[peaks]
    
    # Plot
    plt.figure(figsize=(12, 5))
    plt.plot(ir_filtered, label='IR Filtered (0.5-5Hz)', color='blue')
    plt.plot(peaks, ir_filtered[peaks], "rx", markersize=10, label='Detected Peaks')
    
    # Annotate peak heights
    for p, amp in zip(peaks, amplitudes):
        plt.annotate(f"{amp:.0f}", (p, amp), textcoords="offset points", xytext=(0,10), ha='center', fontsize=9, color='red')
        
    cv_amp = np.std(amplitudes) / np.mean(amplitudes)
    plt.title(f'REJECTED: Amplitude Shift Detected\nAmp CV: {cv_amp*100:.1f}% (Limit: 25%)')
    plt.xlabel('Sample Index (10ms)')
    plt.ylabel('Amplitude')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('rejected_plot.png')
    print("Plot saved to rejected_plot.png")

if __name__ == "__main__":
    plot_rejected_window()
