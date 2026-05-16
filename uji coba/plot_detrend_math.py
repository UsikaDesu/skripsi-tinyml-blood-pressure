import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal
import os

def plot_detrending_math():
    # Menggunakan file yang mirip karena file 173041 tidak ditemukan di folder
    file_path = '../dataset/raw_data_20260421_172913_bp132_81.csv'
    if not os.path.exists(file_path):
        import glob
        files = glob.glob('../dataset/raw_data_*.csv')
        file_path = files[0]
        
    df = pd.read_csv(file_path)
    
    # Ambil 1000 titik (10 detik)
    y_raw = df['ir_raw'].values[0:1000]
    x = np.arange(len(y_raw))
    
    # 1. Matematika di Balik Detrend: Linear Least Squares (Garis regresi terbaik)
    # y = m*x + c
    m, c = np.polyfit(x, y_raw, 1) # derajat 1 (garis lurus)
    trend_line = (m * x) + c
    
    # 2. Sinyal yang sudah di-detrend = Sinyal Mentah dikurangi Garis Trend
    y_detrended = y_raw - trend_line
    # Ini sama persis hasilnya dengan scipy.signal.detrend(y_raw)
    
    # Plotting
    fig, axes = plt.subplots(2, 1, figsize=(12, 8))
    
    # Plot 1: Raw Signal & Trend Line
    axes[0].plot(x, y_raw, label='Sinyal Mentah (Raw)', color='dodgerblue')
    axes[0].plot(x, trend_line, label=f'Garis Lurus Terbaik (y = {m:.2f}x + {c:.0f})', color='red', linestyle='--', linewidth=2)
    axes[0].set_title('1. Sinyal Mentah & Garis Lurus (Trend Line)')
    axes[0].set_ylabel('Amplitudo')
    axes[0].legend()
    axes[0].grid(True)
    
    # Plot 2: Detrended Signal
    axes[1].plot(x, y_detrended, label='Hasil Sinyal - Garis Merah', color='orange')
    axes[1].axhline(0, color='red', linestyle='--', linewidth=2, label='Titik 0 (Garis Merah Didatarkan)')
    axes[1].set_title('2. Setelah Detrending')
    axes[1].set_xlabel('Sample Index (10ms)')
    axes[1].set_ylabel('Amplitudo')
    axes[1].legend()
    axes[1].grid(True)
    
    plt.tight_layout()
    plt.savefig('detrend_math.png')
    print("Plot saved to detrend_math.png")

if __name__ == "__main__":
    plot_detrending_math()
