import os
import sys
import glob
import pandas as pd
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

# Konfigurasi
RAW_DATA_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v2', 'dataset_v2'))
SAMPLING_RATE_HZ = 100
WINDOW_SIZE = 2500  # 25 detik

def get_initials(filename):
    basename = os.path.basename(filename)
    parts = basename.split('_')
    if len(parts) >= 3 and parts[0] == "raw" and parts[1] == "data":
        return parts[2]
    return None

def main():
    # Ambil semua file raw data
    csv_files = glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv'))
    if not csv_files:
        print(f"Tidak ada file raw data di {RAW_DATA_DIR}")
        return

    # Dapatkan daftar subjek unik
    subjects = {}
    for f in csv_files:
        init = get_initials(f)
        if init:
            subjects.setdefault(init, []).append(f)

    if len(sys.argv) < 2:
        print("=" * 50)
        print("PLOT GELOMBANG PPG SUBJEK (V3)")
        print("=" * 50)
        print("Cara penggunaan: python plot_subject_wave.py <INISIAL_SUBJEK>")
        print("Contoh         : python plot_subject_wave.py KYS\n")
        print("Daftar Inisial Subjek yang Tersedia:")
        print(", ".join(sorted(subjects.keys())))
        print("=" * 50)
        return

    target_subject = sys.argv[1].upper()
    
    if target_subject not in subjects:
        print(f"Error: Subjek '{target_subject}' tidak ditemukan!")
        print("Tersedia: " + ", ".join(sorted(subjects.keys())))
        return

    # Ambil file pertama dari subjek tersebut
    target_file = subjects[target_subject][0]
    print(f"Membaca file: {os.path.basename(target_file)}")

    df = pd.read_csv(target_file)
    if len(df) < WINDOW_SIZE:
        print(f"Data terlalu pendek untuk 1 window ({WINDOW_SIZE/SAMPLING_RATE_HZ} detik). Hanya ada {len(df)} sampel.")
        # Jika kurang dari 25 detik, kita ambil seadanya
        window_len = len(df)
    else:
        window_len = WINDOW_SIZE

    # Ambil data untuk divisualisasikan
    ir_raw = df['ir_raw'].values[:window_len]

    # 1. Linear Detrending
    ir_detrended = signal.detrend(ir_raw)

    # 2. Bandpass Filter (0.5 - 5.0 Hz)
    nyq = 0.5 * SAMPLING_RATE_HZ
    b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
    ir_filtered_normal = signal.filtfilt(b, a, ir_detrended)

    # 3. Inversi (Karena sifat pantulan cahaya MAX30102, puncak darah = cahaya minimum)
    ir_filtered_inverted = -ir_filtered_normal

    # 4. Deteksi Puncak (Systolic Peaks) pada sinyal yang diinversi
    peaks, _ = signal.find_peaks(ir_filtered_inverted, distance=35, prominence=np.std(ir_filtered_inverted)*0.5)

    # 5. Deteksi Lembah (Troughs) di antara puncak
    troughs = []
    for i in range(len(peaks) - 1):
        segment = ir_filtered_inverted[peaks[i]:peaks[i+1]]
        trough_idx = peaks[i] + np.argmin(segment)
        troughs.append(trough_idx)

    # Tambahkan lembah pertama sebelum puncak pertama (jika ada)
    if peaks[0] > 0:
        segment_start = ir_filtered_inverted[0:peaks[0]]
        trough_idx_start = np.argmin(segment_start)
        troughs.insert(0, trough_idx_start)

    troughs = np.array(troughs)

    # ==========================
    # VISUALISASI
    # ==========================
    time_axis = np.arange(window_len) / SAMPLING_RATE_HZ

    fig, (ax0, ax1, ax2) = plt.subplots(3, 1, figsize=(12, 10), sharex=True)

    # Plot 0: Sinyal Paling Mentah (Sebelum Detrend)
    ax0.plot(time_axis, ir_raw, color='black', label='Completely Raw IR')
    ax0.set_title(f'Sinyal Mentah Asli PPG Subjek [{target_subject}] (Dengan Baseline Drift)')
    ax0.set_ylabel('Amplitudo (ADC)')
    ax0.grid(True, linestyle='--', alpha=0.6)
    ax0.legend()

    # Plot 1: Sinyal Detrended - Puncak Asli menghadap ke BAWAH
    ax1.plot(time_axis, ir_detrended, color='gray', label='Raw IR (Detrended)')
    ax1.set_title('Sinyal Setelah Linear Detrending (Baseline Datar, Belum Difilter & Diinversi)')
    ax1.set_ylabel('Amplitudo')
    ax1.grid(True, linestyle='--', alpha=0.6)
    ax1.legend()

    # Plot 2: Sinyal Filtered & Inverted (Puncak Asli menghadap ke ATAS)
    ax2.plot(time_axis, ir_filtered_inverted, color='red', label='Filtered & Inverted IR (V3)')
    ax2.plot(time_axis[peaks], ir_filtered_inverted[peaks], "v", color='blue', markersize=8, label='Puncak Sistolik')
    ax2.plot(time_axis[troughs], ir_filtered_inverted[troughs], "^", color='green', markersize=8, label='Lembah')
    
    # Beri anotasi Crest Time pada salah satu gelombang
    if len(peaks) > 1 and len(troughs) > 1:
        p_idx = peaks[1]
        t_idx = troughs[1] # Lembah sebelum puncak ke-2
        if t_idx < p_idx:
            ax2.axvspan(time_axis[t_idx], time_axis[p_idx], color='yellow', alpha=0.3, label='Crest Time (Rise Time)')
    
    ax2.set_title('Sinyal Bersih Siap Ekstraksi (Puncak Sistolik & Lembah Terdeteksi)')
    ax2.set_xlabel('Waktu (Detik)')
    ax2.set_ylabel('Amplitudo Ternormalisasi')
    ax2.grid(True, linestyle='--', alpha=0.6)
    ax2.legend()

    plt.tight_layout()
    output_filename = f'wave_plot_{target_subject}.png'
    plt.savefig(output_filename)
    print(f"\nBerhasil! Gambar grafik telah disimpan sebagai '{output_filename}'")

if __name__ == "__main__":
    main()
