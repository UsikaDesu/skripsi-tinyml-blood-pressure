import os
import glob
import pandas as pd
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

# ==========================================
# KONFIGURASI
# ==========================================
DATASET_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset')
SAMPLING_RATE = 100  # Hz
WINDOW_SIZE = 1000   # 10 detik

def main():
    # Cari semua file data mentah
    csv_files = sorted(glob.glob(os.path.join(DATASET_DIR, 'raw_data_*.csv')))
    
    if not csv_files:
        print("Tidak ada file data mentah ditemukan!")
        return
    
    print("=" * 60)
    print("VISUALISASI TAHAPAN PREPROCESSING")
    print("=" * 60)
    print()
    
    for i, file in enumerate(csv_files):
        print(f"[{i+1}] {os.path.basename(file)}")
    
    print()
    choice = int(input(f"Pilih file [1-{len(csv_files)}]: ")) - 1
    
    if choice < 0 or choice >= len(csv_files):
        print("Pilihan tidak valid!")
        return
    
    df = pd.read_csv(csv_files[choice])
    basename = os.path.basename(csv_files[choice])
    
    # Ambil window pertama (10 detik = 1000 sampel)
    ir_raw = df['ir_raw'].values[:WINDOW_SIZE]
    time_s = np.arange(len(ir_raw)) / SAMPLING_RATE  # Waktu dalam detik
    
    # ==========================================
    # TAHAP PREPROCESSING
    # ==========================================
    
    # 1. Data Mentah (Raw)
    raw = ir_raw.copy()
    
    # 2. Detrending
    detrended = signal.detrend(ir_raw)
    
    # 3. Bandpass Filter saja (tanpa detrending)
    nyq = 0.5 * SAMPLING_RATE
    b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
    bandpass_only = signal.filtfilt(b, a, ir_raw.astype(float))
    
    # 4. Detrending + Bandpass Filter (proses lengkap)
    detrend_then_bandpass = signal.filtfilt(b, a, detrended)
    
    # ==========================================
    # PLOT GRAFIK
    # ==========================================
    fig, axes = plt.subplots(4, 1, figsize=(14, 10), sharex=True)
    fig.suptitle(f'Tahapan Preprocessing Sinyal PPG\n{basename}', 
                 fontsize=14, fontweight='bold')
    
    # Warna
    colors = ['#2196F3', '#FF9800', '#4CAF50', '#E91E63']
    titles = [
        '1. Sinyal Mentah (Raw)',
        '2. Setelah Detrending',
        '3. Setelah Bandpass Filter Saja (0.5-5 Hz)',
        '4. Detrending + Bandpass Filter (Proses Lengkap)'
    ]
    data_list = [raw, detrended, bandpass_only, detrend_then_bandpass]
    
    for idx, (ax, title, data, color) in enumerate(zip(axes, titles, data_list, colors)):
        ax.plot(time_s, data, color=color, linewidth=0.8)
        ax.set_title(title, fontsize=11, fontweight='bold', loc='left')
        ax.set_ylabel('Amplitudo')
        ax.grid(True, alpha=0.3)
        
        # Tambahkan info statistik
        ax.text(0.98, 0.92, f'Mean: {np.mean(data):.1f}  |  STD: {np.std(data):.1f}', 
                transform=ax.transAxes, fontsize=9, ha='right', va='top',
                bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.8))
    
    axes[-1].set_xlabel('Waktu (detik)', fontsize=11)
    
    plt.tight_layout()
    
    # Simpan
    save_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'preprocessing_stages.png')
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"\nGrafik disimpan di: {save_path}")
    
    plt.show()

if __name__ == '__main__':
    main()
