"""
PIPELINE V1: Ekstraksi Fitur (BASELINE - TANPA PREPROCESSING, TANPA SQA)
========================================================================
Kondisi:
  - TIDAK ada Band-Pass Filter
  - TIDAK ada Linear Detrending
  - TIDAK ada Signal Quality Assessment (SQA)
  - TIDAK ada Pelabelan Subjek (subject_id)
  - Sinyal MENTAH langsung digunakan untuk deteksi puncak dan hitung fitur
  - Semua window diterima tanpa seleksi
  
Fitur: ir_ratio, red_ratio, crest_time_ratio, heart_rate
Sumber data: raw_data_pipeline_v2/dataset_v2 (raw_data_*.csv)
"""
import os
import glob
import pandas as pd
import numpy as np

RAW_DATA_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v2', 'dataset_v2'))
OUTPUT_DIR   = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_v1'))
OUTPUT_FILE  = os.path.join(OUTPUT_DIR, 'extracted_features_v1.csv')

SAMPLING_RATE = 100  # Hz

os.makedirs(OUTPUT_DIR, exist_ok=True)

def find_peaks_simple(signal, min_dist=35, prominence_factor=0.5):
    """Deteksi puncak sederhana tanpa scipy (agar konsisten dengan V1 tanpa filter)."""
    threshold = np.std(signal) * prominence_factor
    peaks = []
    last_peak = -min_dist
    for i in range(1, len(signal) - 1):
        if signal[i] > signal[i-1] and signal[i] > signal[i+1]:
            if signal[i] > threshold and (i - last_peak) >= min_dist:
                peaks.append(i)
                last_peak = i
    return np.array(peaks)

def extract():
    print("=" * 65)
    print("PIPELINE V1: Baseline (Tanpa Preprocessing, Tanpa SQA)")
    print("=" * 65)

    csv_files = sorted(glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv')))
    print(f"Ditemukan {len(csv_files)} file raw data\n")

    features_list = []

    for file in csv_files:
        parts = os.path.basename(file).split('_')
        # Pastikan file format raw_data_XXXX_*.csv dan bukan angka
        if len(parts) < 3 or parts[0] != 'raw' or parts[1] != 'data':
            continue
        if parts[2].isdigit():
            continue

        df = pd.read_csv(file)
        if len(df) < 500:
            continue

        systolic  = df['systolic_label'].iloc[0]
        diastolic = df['diastolic_label'].iloc[0]
        bp_class  = 1 if (systolic >= 140 or diastolic >= 90) else 0

        window_size = 1000
        num_windows = 2 if len(df) >= 2000 else (1 if len(df) >= 1000 else 0)

        accepted = 0
        for w in range(num_windows):
            s = w * window_size
            e = s + window_size

            # =============================================
            # V1: Gunakan sinyal MENTAH tanpa filter apapun
            # =============================================
            ir_raw  = df['ir_raw'].values[s:e].astype(float)
            red_raw = df['red_raw'].values[s:e].astype(float)

            # Deteksi puncak langsung pada sinyal mentah
            # (di-normalisasi dulu agar deteksi puncak lebih mudah)
            ir_centered = ir_raw - np.mean(ir_raw)
            peaks = find_peaks_simple(ir_centered, min_dist=35)

            if len(peaks) < 2:
                print(f"  -> [SKIP] {os.path.basename(file)} w{w+1}: Puncak tidak terdeteksi")
                continue

            rr = np.diff(peaks)
            mean_rr = np.mean(rr)
            if mean_rr == 0:
                continue

            heart_rate = 60.0 / (mean_rr / SAMPLING_RATE)

            # Hitung crest time dari sinyal mentah (centered)
            rise_times = []
            for p in peaks:
                trough_idx = p
                for j in range(p - 1, max(0, p - 45) - 1, -1):
                    if ir_centered[j] <= ir_centered[trough_idx]:
                        trough_idx = j
                    else:
                        break
                if p > trough_idx:
                    rise_times.append((p - trough_idx) * 10.0)

            if not rise_times:
                continue

            mean_rise = np.mean(rise_times)
            ctr = mean_rise / (60000.0 / heart_rate) if heart_rate > 0 else 0

            # Fitur AC/DC dari sinyal mentah (TANPA filter)
            ir_mean  = np.mean(ir_raw)
            red_mean = np.mean(red_raw)
            ir_std   = np.std(ir_centered)   # std dari sinyal centered sebagai proxy AC
            red_std  = np.std(red_raw - np.mean(red_raw))

            ir_ratio  = (ir_std  / ir_mean)  * 100.0 if ir_mean  > 0 else 0
            red_ratio = (red_std / red_mean) * 100.0 if red_mean > 0 else 0

            features_list.append({
                'ir_ratio':         round(ir_ratio,  4),
                'red_ratio':        round(red_ratio, 4),
                'crest_time_ratio': round(ctr,       4),
                'heart_rate':       round(heart_rate, 2),
                'systolic_bp':      systolic,
                'diastolic_bp':     diastolic,
                'bp_class':         bp_class,
                'notes':            f"win_{w+1}_of_{num_windows}"
            })
            accepted += 1

        print(f"[OK] {os.path.basename(file)} -> {accepted}/{num_windows} windows diterima")

    df_out = pd.DataFrame(features_list)
    if df_out.empty:
        print("\n[ERROR] Tidak ada data yang berhasil diekstraksi!")
        return

    total_n = (df_out['bp_class'] == 0).sum()
    total_h = (df_out['bp_class'] == 1).sum()
    print(f"\nTotal: {len(df_out)} windows ({total_n}N / {total_h}H) [TANPA pemisahan subjek]")

    df_out.to_csv(OUTPUT_FILE, index=False)
    print(f"\n[SAVED] {OUTPUT_FILE}")
    print("\nCATPATAN: V1 TIDAK memiliki SQA maupun pemisahan subjek.")
    print("Evaluasi model menggunakan StratifiedKFold biasa (RAWAN data leakage).")
    print("=" * 65 + "\n")

if __name__ == '__main__':
    extract()
