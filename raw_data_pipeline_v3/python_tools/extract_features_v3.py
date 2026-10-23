"""
PIPELINE V3: Ekstraksi Fitur (SQA SAJA - TANPA Preprocessing Filter)
======================================================================
Kondisi:
  - TIDAK ada Band-Pass Filter
  - TIDAK ada Linear Detrending
  - ADA Signal Quality Assessment (SQA) berbasis IQR Dinamis
  - ADA Pelabelan Subjek (subject_id) + Undersampling (maks 10/subjek)
  - ADA Filter Minority Class per Subjek
  - Sinyal MENTAH digunakan untuk deteksi puncak dan hitung fitur
  - Sinyal yang bergoyang/noise tetap DITOLAK oleh SQA (meski tanpa filter)

Perbedaan dengan V4:
  - V3: SQA saja (tanpa filter) — menyeleksi segmen terbaik dari sinyal mentah
  - V4: SQA + Preprocessing — sinyal dibersihkan dulu, LALU diseleksi

Fitur: ir_ratio, red_ratio, crest_time_ratio, heart_rate
Sumber data: raw_data_pipeline_v2/dataset_v2 (raw_data_*.csv)
"""
import os
import glob
import pandas as pd
import numpy as np

RAW_DATA_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v2', 'dataset_v2'))
OUTPUT_DIR   = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_v3'))
OUTPUT_FILE  = os.path.join(OUTPUT_DIR, 'extracted_features_v3.csv')

SAMPLING_RATE = 100  # Hz

os.makedirs(OUTPUT_DIR, exist_ok=True)

def get_subject_id(filename):
    parts = os.path.basename(filename).split('_')
    if len(parts) >= 3 and parts[0] == 'raw' and parts[1] == 'data':
        initial = parts[2]
        return None if initial.isdigit() else f"subj_{initial}"
    return None

def get_session_key(filename):
    parts = os.path.basename(filename).split('_')
    return f"{parts[3]}_{parts[4]}" if len(parts) >= 5 else 'unknown'

def find_peaks_simple(sig, min_dist=35, prominence_factor=0.5):
    """Deteksi puncak tanpa filter — digunakan untuk menghitung SQA dari sinyal mentah."""
    threshold = np.std(sig) * prominence_factor
    peaks = []
    last_peak = -min_dist
    for i in range(1, len(sig) - 1):
        if sig[i] > sig[i-1] and sig[i] > sig[i+1]:
            if sig[i] > threshold and (i - last_peak) >= min_dist:
                peaks.append(i)
                last_peak = i
    return np.array(peaks)

def extract():
    print("=" * 65)
    print("PIPELINE V3: SQA Saja (Tanpa Preprocessing Filter)")
    print("=" * 65)

    csv_files = sorted(glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv')))
    print(f"Ditemukan {len(csv_files)} file raw data\n")

    # ==========================================
    # PASS 1: GATHER ALL WINDOWS (Tanpa filter)
    # ==========================================
    print("PASS 1: Menghitung SQA Metrics dari sinyal MENTAH...")
    raw_windows = []

    for file in csv_files:
        subject_id = get_subject_id(file)
        if subject_id is None:
            continue

        df = pd.read_csv(file)
        if len(df) < 500:
            continue

        systolic  = df['systolic_label'].iloc[0]
        diastolic = df['diastolic_label'].iloc[0]
        bp_class  = 1 if (systolic >= 140 or diastolic >= 90) else 0
        session   = get_session_key(file)

        window_size = 1000
        num_windows = 2 if len(df) >= 2000 else (1 if len(df) >= 1000 else 0)

        for w in range(num_windows):
            s = w * window_size
            e = s + window_size
            ir_raw  = df['ir_raw'].values[s:e].astype(float)
            red_raw = df['red_raw'].values[s:e].astype(float)

            # =============================================
            # V3: TIDAK ada filter — sinyal mentah langsung
            # =============================================
            ir_centered = ir_raw - np.mean(ir_raw)
            peaks = find_peaks_simple(ir_centered, min_dist=35)

            if len(peaks) < 2:
                continue

            rr = np.diff(peaks)
            mean_rr = np.mean(rr)
            if mean_rr == 0:
                continue

            # Hitung SQA CV dari sinyal mentah
            time_cv = (np.std(rr) / mean_rr) * 100.0

            amps = ir_centered[peaks]
            mean_amp = np.mean(amps)
            if mean_amp == 0:
                continue
            amp_cv = (np.std(amps) / mean_amp)

            raw_windows.append({
                'file': file,
                'w': w,
                'num_windows': num_windows,
                'subject_id': subject_id,
                'session': session,
                'bp_class': bp_class,
                'systolic': systolic,
                'diastolic': diastolic,
                'ir_raw': ir_raw,
                'red_raw': red_raw,
                'ir_centered': ir_centered,
                'peaks': peaks,
                'mean_rr': mean_rr,
                'time_cv': time_cv,
                'amp_cv': amp_cv
            })

    print(f"Berhasil mengumpulkan {len(raw_windows)} valid windows.\n")

    # ==========================================
    # CALCULATE DYNAMIC IQR THRESHOLDS
    # ==========================================
    time_cvs = [d['time_cv'] for d in raw_windows]
    amp_cvs  = [d['amp_cv'] for d in raw_windows]

    q3_time  = np.percentile(time_cvs, 75)
    iqr_time = q3_time - np.percentile(time_cvs, 25)
    max_time_cv = q3_time + (1.5 * iqr_time)

    q3_amp  = np.percentile(amp_cvs, 75)
    iqr_amp = q3_amp - np.percentile(amp_cvs, 25)
    max_amp_cv = q3_amp + (1.5 * iqr_amp)

    print("=" * 65)
    print("HASIL KALKULASI DYNAMIC IQR SQA THRESHOLDS (dari sinyal mentah)")
    print("=" * 65)
    print(f"MAX_TIME_CV (Batas Atas): {max_time_cv:.1f} %")
    print(f"MAX_AMP_CV  (Batas Atas): {max_amp_cv:.3f}")
    print("=" * 65 + "\n")

    # ==========================================
    # PASS 2: FILTER DENGAN SQA (tanpa filter sinyal)
    # ==========================================
    print("PASS 2: Memfilter data dengan threshold IQR (dari sinyal mentah)...")
    features_list = []

    file_groups = {}
    for w in raw_windows:
        file_groups.setdefault(w['file'], []).append(w)

    for file, windows in file_groups.items():
        num_windows = windows[0]['num_windows']
        accepted_count = 0
        for d in windows:
            if d['time_cv'] > max_time_cv:
                print(f"  -> [REJECT] {os.path.basename(file)} w{d['w']+1}: Time CV {d['time_cv']:.1f}%")
                continue
            if d['amp_cv'] > max_amp_cv:
                print(f"  -> [REJECT] {os.path.basename(file)} w{d['w']+1}: Amp CV {d['amp_cv']:.3f}")
                continue

            # Hitung fitur dari sinyal MENTAH (bukan filtered)
            ir_centered = d['ir_centered']
            peaks       = d['peaks']

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

            mean_rise  = np.mean(rise_times)
            heart_rate = 60.0 / (d['mean_rr'] / SAMPLING_RATE)
            ctr = mean_rise / (60000.0 / heart_rate) if heart_rate > 0 else 0

            ir_raw  = d['ir_raw']
            red_raw = d['red_raw']
            ir_mean  = np.mean(ir_raw)
            red_mean = np.mean(red_raw)
            ir_std   = np.std(ir_centered)
            red_std  = np.std(red_raw - np.mean(red_raw))

            ir_ratio  = (ir_std  / ir_mean)  * 100.0 if ir_mean  > 0 else 0
            red_ratio = (red_std / red_mean) * 100.0 if red_mean > 0 else 0

            features_list.append({
                'subject_id':       d['subject_id'],
                'session':          d['session'],
                'time_cv':          round(d['time_cv'], 2),
                'amp_cv':           round(d['amp_cv'], 3),
                'ir_ratio':         round(ir_ratio,  4),
                'red_ratio':        round(red_ratio, 4),
                'crest_time_ratio': round(ctr,       4),
                'heart_rate':       round(heart_rate, 2),
                'systolic_bp':      d['systolic'],
                'diastolic_bp':     d['diastolic'],
                'bp_class':         d['bp_class'],
                'notes':            f"win_{d['w']+1}_of_{num_windows}"
            })
            accepted_count += 1

        print(f"[OK] {os.path.basename(file)} -> {accepted_count}/{num_windows} windows diterima")

    df_out = pd.DataFrame(features_list)
    if df_out.empty:
        print("\n[ERROR] Tidak ada data yang berhasil diekstraksi!")
        return

    # ── Filter Minority Class per Subjek ──
    print("\n" + "=" * 65)
    print("FILTER MINORITY CLASS PER SUBJEK")
    print("=" * 65)
    filtered = []
    for subj, grp in df_out.groupby('subject_id'):
        mode_class = grp['bp_class'].mode()[0]
        dropped    = (grp['bp_class'] != mode_class).sum()
        if dropped > 0:
            print(f"  -> {subj}: Dibuang {dropped} window minority class")
        filtered.append(grp[grp['bp_class'] == mode_class])
    df_out = pd.concat(filtered).reset_index(drop=True)

    # ── Undersampling (max 10 per subjek) ──
    print("\n" + "=" * 65)
    print("UNDERSAMPLING (Maks 10 window per Subjek)")
    print("=" * 65)
    MAX_PER_SUBJECT = 10
    undersampled = []
    for subj, grp in df_out.groupby('subject_id'):
        if len(grp) > MAX_PER_SUBJECT:
            print(f"  -> {subj}: Dipangkas {len(grp)} -> {MAX_PER_SUBJECT}")
            grp = grp.head(MAX_PER_SUBJECT)
        undersampled.append(grp)
    df_out = pd.concat(undersampled).reset_index(drop=True)

    # ── Distribusi Akhir ──
    print("\n" + "=" * 65)
    print("DISTRIBUSI SUBJEK AKHIR")
    print("=" * 65)
    for subj, grp in df_out.groupby('subject_id'):
        n = (grp['bp_class'] == 0).sum()
        h = (grp['bp_class'] == 1).sum()
        print(f"  {subj}: {len(grp)} windows ({n}N/{h}H)")

    total_n = (df_out['bp_class'] == 0).sum()
    total_h = (df_out['bp_class'] == 1).sum()
    print(f"\nTotal: {len(df_out)} windows dari {df_out['subject_id'].nunique()} subjek ({total_n}N / {total_h}H)")

    df_out.to_csv(OUTPUT_FILE, index=False)
    print(f"\n[SAVED] {OUTPUT_FILE}")
    print("\nCATATAN: V3 menggunakan SQA Dinamis IQR TANPA preprocessing filter.")
    print("Fitur dihitung dari sinyal mentah yang lolos seleksi SQA.")
    print("=" * 65 + "\n")

if __name__ == '__main__':
    extract()
