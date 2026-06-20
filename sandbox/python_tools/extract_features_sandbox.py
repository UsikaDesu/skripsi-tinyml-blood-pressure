"""
SANDBOX: Ekstraksi Fitur V3
Fitur: ir_ratio, red_ratio, crest_time_ratio, heart_rate
Sumber data: dataset_sandbox (raw_data_*.csv)
"""
import os
import glob
import pandas as pd
import numpy as np
import scipy.signal as signal

RAW_DATA_DIR  = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v2', 'dataset_v2'))
OUTPUT_DIR    = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_sandbox'))
OUTPUT_FILE   = os.path.join(OUTPUT_DIR, 'extracted_features_sandbox.csv')
SAMPLING_RATE = 100  # Hz

def get_subject_id(filename):
    parts = os.path.basename(filename).split('_')
    if len(parts) >= 3 and parts[0] == 'raw' and parts[1] == 'data':
        initial = parts[2]
        return None if initial.isdigit() else f"subj_{initial}"
    return None

def get_session_key(filename):
    parts = os.path.basename(filename).split('_')
    return f"{parts[3]}_{parts[4]}" if len(parts) >= 5 else 'unknown'

def extract():
    print("=" * 65)
    print("SANDBOX: Ekstraksi Fitur V3 (CTR + HR + IR/Red Ratio)")
    print("=" * 65)

    csv_files = sorted(glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv')))
    print(f"Ditemukan {len(csv_files)} file raw data\n")

    features_list = []

    for file in csv_files:
        subject_id = get_subject_id(file)
        if subject_id is None:
            continue

        df = pd.read_csv(file)
        if len(df) < 500:
            print(f"Skipping {os.path.basename(file)}: terlalu pendek ({len(df)} sampel).")
            continue

        systolic  = df['systolic_label'].iloc[0]
        diastolic = df['diastolic_label'].iloc[0]
        bp_class  = 1 if (systolic >= 140 or diastolic >= 90) else 0
        session   = get_session_key(file)

        window_size  = 1000
        # EKSPERIMEN: Hanya ambil 10 detik pertama (1 window)
        num_windows  = 1 if len(df) >= window_size else 0
        count_before = len(features_list)

        for w in range(num_windows):
            s = w * window_size
            e = s + window_size
            ir_raw  = df['ir_raw'].values[s:e]
            red_raw = df['red_raw'].values[s:e]

            # Preprocessing
            nyq = 0.5 * SAMPLING_RATE
            b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
            ir_flt  = -signal.filtfilt(b, a, signal.detrend(ir_raw))
            red_flt = -signal.filtfilt(b, a, signal.detrend(red_raw))

            # Peak detection
            peaks, _ = signal.find_peaks(ir_flt, distance=35, prominence=np.std(ir_flt)*0.5)
            if len(peaks) < 2:
                continue

            rr = np.diff(peaks)
            mean_rr = np.mean(rr)
            if mean_rr == 0:
                continue

            # SQA Gate 1: Time CV (Menggunakan Threshold IQR 10.0%)
            time_cv = (np.std(rr) / mean_rr) * 100
            if time_cv > 10.0:
                print(f"  -> [REJECT] {os.path.basename(file)} w{w+1}: Time CV {time_cv:.1f}% > 10.0%")
                continue

            # SQA Gate 2: Amplitude CV (Menggunakan Threshold IQR 37.0%)
            amps = ir_flt[peaks]
            mean_amp = np.mean(amps)
            if mean_amp <= 0:
                continue
            amp_cv = np.std(amps) / mean_amp
            if amp_cv > 0.37:
                print(f"  -> [REJECT] {os.path.basename(file)} w{w+1}: Amp CV {amp_cv*100:.1f}% > 37.0%")
                continue

            # Crest Time (Rise Time per puncak)
            rise_times = []
            for p in peaks:
                trough_idx = p
                for j in range(p - 1, max(0, p - 45) - 1, -1):
                    if ir_flt[j] <= ir_flt[trough_idx]:
                        trough_idx = j
                    else:
                        break
                if p > trough_idx:
                    rise_times.append((p - trough_idx) * 10.0)
            if not rise_times:
                continue

            mean_rise = np.mean(rise_times)
            heart_rate = 60.0 / (mean_rr / SAMPLING_RATE)

            # Fitur V3
            ir_ratio  = (np.std(ir_flt)  / np.mean(ir_raw))  * 100.0 if np.mean(ir_raw)  > 0 else 0
            red_ratio = (np.std(red_flt) / np.mean(red_raw)) * 100.0 if np.mean(red_raw) > 0 else 0
            ctr       = mean_rise / (60000.0 / heart_rate)    if heart_rate > 0 else 0

            features_list.append({
                'subject_id':       subject_id,
                'session':          session,
                'ir_ratio':         round(ir_ratio,  4),
                'red_ratio':        round(red_ratio, 4),
                'crest_time_ratio': round(ctr,       4),
                'heart_rate':       round(heart_rate, 2),
                'systolic_bp':      systolic,
                'diastolic_bp':     diastolic,
                'bp_class':         bp_class,
                'notes':            f"win_{w+1}_of_{num_windows}"
            })

        accepted = len(features_list) - count_before
        print(f"[OK] {os.path.basename(file)} -> {accepted}/{num_windows} windows diterima")

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

    # ── Undersampling Round-Robin ──
    print("\n" + "=" * 65)
    print("UNDERSAMPLING (Dinonaktifkan untuk tes ini)")
    print("=" * 65)
    MAX_PER_SUBJECT = 999
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

if __name__ == '__main__':
    extract()
