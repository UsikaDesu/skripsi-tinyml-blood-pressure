import os
import glob
import pandas as pd
import numpy as np
import scipy.signal as signal

# ==========================================
# CONFIGURATION
# ==========================================
RAW_DATA_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_v2'))
OUTPUT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_v2'))
OUTPUT_FILE = os.path.join(OUTPUT_DIR, 'extracted_features_v2.csv')
SAMPLING_RATE_HZ = 100  # Based on 10ms delta in raw data

# ==========================================
# SUBJECT IDENTIFICATION
# ==========================================
def get_subject_id(filename):
    """Extracts subject ID directly from the filename (e.g., raw_data_KYS_2026...csv)"""
    basename = os.path.basename(filename)
    parts = basename.split('_')
    
    # Expected format: raw_data_INISIAL_...
    if len(parts) >= 3 and parts[0] == "raw" and parts[1] == "data":
        initial = parts[2]
        if initial.isdigit():
            print(f"Warning: File {basename} tidak memiliki inisial, dilewati.")
            return None
        return f"subj_{initial}"
    else:
        print(f"Warning: Filename format invalid, skipping: {basename}")
        return None

def get_record_date(filename):
    """Extracts recording date (YYYYMMDD) from filename."""
    basename = os.path.basename(filename)
    parts = basename.split('_')
    # Expected format: raw_data_INISIAL_YYYYMMDD_HHMMSS_bpXXX_YY.csv
    if len(parts) >= 4:
        return parts[3]  # e.g., '20260421'
    return 'unknown'

def get_record_time(filename):
    """Extracts recording time (HHMMSS) from filename."""
    basename = os.path.basename(filename)
    parts = basename.split('_')
    # Expected format: raw_data_INISIAL_YYYYMMDD_HHMMSS_bpXXX_YY.csv
    if len(parts) >= 5:
        return parts[4]  # e.g., '172913'
    return '000000'

def load_and_extract_features():
    print("=" * 70)
    print("V2.1 FEATURE EXTRACTION (Patched: Inversion, HR Limit, True Trough)")
    print("=" * 70)
    
    csv_files = glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv'))
    if not csv_files:
        print(f"No raw data files found in {RAW_DATA_DIR}")
        return
        
    print(f"Found {len(csv_files)} files in primary dataset. Extracting features...\n")
    
    features_list = []
    
    for file in csv_files:
        subject_id = get_subject_id(file)
        if subject_id is None:
            continue
            
        try:
            df = pd.read_csv(file)
            if len(df) < 500: # Need at least 5 seconds of data at 100Hz
                print(f"Skipping {os.path.basename(file)}: Not enough samples ({len(df)}).")
                continue
                
            window_size = 1000
            num_windows = len(df) // window_size
            
            if num_windows == 0:
                print(f"Skipping {os.path.basename(file)}: Length ({len(df)}) is too short.")
                continue
                
            count_before = len(features_list)
            for w in range(num_windows):
                start_idx = w * window_size
                end_idx = start_idx + window_size
                
                ir_raw = df['ir_raw'].values[start_idx:end_idx]
                red_raw = df['red_raw'].values[start_idx:end_idx]
                
                # Apply Detrending
                ir_detrended = signal.detrend(ir_raw)
                red_detrended = signal.detrend(red_raw)
                
                # Zero-phase Bandpass filter
                nyq = 0.5 * SAMPLING_RATE_HZ
                b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
                ir_filtered = signal.filtfilt(b, a, ir_detrended)
                red_filtered = signal.filtfilt(b, a, red_detrended)
                
                # ==========================================
                # PATCH 1: INVERSI SINYAL FISIKA MAX30102
                # ==========================================
                # Sinyal pantulan cahaya menurun saat darah penuh (sistolik). 
                # Wajib dikali -1 agar puncak sistolik menghadap ke ATAS!
                ir_filtered = -ir_filtered
                red_filtered = -red_filtered
                
                # ==========================================
                # PATCH 2: BATAS DETAK JANTUNG (distance=35)
                # ==========================================
                # Find peaks on inverted (correct) IR signal
                peaks, _ = signal.find_peaks(ir_filtered, distance=35, prominence=np.std(ir_filtered)*0.5)
                
                if len(peaks) < 2:
                    print(f"  -> [REJECTED] Window {w+1}/{num_windows} in {os.path.basename(file)}: Flatline (No Peaks).")
                    continue
                    
                rr_intervals = np.diff(peaks)
                mean_rr = np.mean(rr_intervals)
                
                if mean_rr == 0:
                    continue
                    
                time_cv = (np.std(rr_intervals) / mean_rr) * 100
                
                # 1. SQA Jitter Gate: Time CV <= 15%
                if time_cv > 15.0:
                    print(f"  -> [REJECTED] Window {w+1}/{num_windows} in {os.path.basename(file)}: Time CV {time_cv:.1f}% > 15% Limit (Jitter Detected).")
                    continue
                    
                # 2. SQA Amplitude Gate: Amp CV <= 25%
                amplitudes = ir_filtered[peaks]
                mean_amp = np.mean(amplitudes)
                if mean_amp == 0:
                    continue
                    
                cv_amp = np.std(amplitudes) / mean_amp
                if cv_amp > 0.25:
                    print(f"  -> [REJECTED] Window {w+1}/{num_windows} in {os.path.basename(file)}: Amp CV {cv_amp*100:.1f}% > 25% Limit (Pressure Shift Detected).")
                    continue
                
                # ==========================================
                # PATCH 3: PENCARI LEMBAH (TROUGH) YANG BENAR
                # ==========================================
                rise_times = []
                for p in peaks:
                    trough_idx = p
                    start_search = max(0, p - 45) # Batas mundur maksimal 450ms
                    
                    # Berjalan mundur dari puncak satu persatu
                    for j in range(p - 1, start_search - 1, -1):
                        if ir_filtered[j] <= ir_filtered[trough_idx]:
                            trough_idx = j
                        elif ir_filtered[j] > ir_filtered[trough_idx]:
                            break # Kita sudah menemukan lembah murni
                            
                    if p > trough_idx:
                        # Rise time in ms (1 sample = 10 ms)
                        rise_time_ms = (p - trough_idx) * 10.0
                        rise_times.append(rise_time_ms)
                
                if len(rise_times) == 0:
                    continue
                    
                mean_rise_time = np.mean(rise_times)
                
                # Calculate Heart Rate (BPM)
                mean_rr_seconds = mean_rr / SAMPLING_RATE_HZ
                heart_rate = 60.0 / mean_rr_seconds
                
                # Calculate Standard Deviation
                ir_std = np.std(ir_filtered)
                red_std = np.std(red_filtered)
                
                # Calculate DC baseline (Mean of raw signal)
                ir_mean = np.mean(ir_raw)
                red_mean = np.mean(red_raw)
                
                # Calculate V2 UPGRADED FEATURES: AC/DC Ratio (Perfusion Index %)
                ir_ratio = (ir_std / ir_mean) * 100.0 if ir_mean > 0 else 0
                red_ratio = (red_std / red_mean) * 100.0 if red_mean > 0 else 0
                
                systolic = df['systolic_label'].iloc[0]
                diastolic = df['diastolic_label'].iloc[0]
                
                if systolic >= 140 or diastolic >= 90:
                    bp_class = 1  # Hipertensi
                else:
                    bp_class = 0  # Non-Hipertensi
                
                notes = f"win_{w+1}_of_{num_windows}"
                record_date = get_record_date(file)
                record_time = get_record_time(file)
                
                features_list.append({
                    'timestamp': int(df['time_ms'].iloc[start_idx]),
                    'subject_id': subject_id,
                    'record_date': record_date,
                    'record_time': record_time,
                    'total_cv': time_cv + (cv_amp * 100),
                    'ir_ratio': round(ir_ratio, 4),
                    'red_ratio': round(red_ratio, 4),
                    'rise_time_ms': round(mean_rise_time, 2),
                    'heart_rate': round(heart_rate, 2),
                    'systolic_bp': systolic,
                    'diastolic_bp': diastolic,
                    'bp_class': bp_class,
                    'notes': notes
                })
            
            accepted = len(features_list) - count_before
            rejected = num_windows - accepted
            
            if rejected > 0:
                print(f"[OK] Processed {os.path.basename(file)} -> Accepted {accepted}/{num_windows} windows, Rejected {rejected}")
            else:
                print(f"[OK] Processed {os.path.basename(file)} -> Accepted {accepted}/{num_windows} windows")
            
        except Exception as e:
            print(f"[ERROR] Failed to process {file}: {e}")
            
    if not features_list:
        print("No features extracted.")
        return
        
    out_df = pd.DataFrame(features_list)
    
    # ============================================================
    # FILTER MINORITY CLASS PER SUBJECT
    # ============================================================
    # Buang data yang kurang dominan dari setiap subjek.
    # Jika subjek A memiliki 10 HT dan 2 Non-HT, maka 2 Non-HT dibuang.
    print("\n" + "=" * 70)
    print("FILTER MINORITY CLASS PER SUBJECT")
    print("=" * 70)
    filtered_rows = []
    for subject, group in out_df.groupby('subject_id'):
        majority_class = group['bp_class'].mode()[0]
        majority_group = group[group['bp_class'] == majority_class]
        filtered_rows.append(majority_group)
        removed = len(group) - len(majority_group)
        if removed > 0:
            print(f"  -> {subject}: Dibuang {removed} window minority class (disisakan {len(majority_group)} window class {majority_class})")
    
    out_df = pd.concat(filtered_rows).reset_index(drop=True)
    
    # ============================================================
    # UNDERSAMPLING ROUND-ROBIN PER SESI (Maks 10 per Subjek)
    # ============================================================
    # Logika: Untuk setiap subjek, kelompokkan data ke dalam "sesi".
    # Sesi baru dimulai jika:
    #   - Tanggal berbeda, ATAU
    #   - Tanggal sama tapi jarak waktu > 4 jam
    # Kemudian ambil 1 window dari setiap sesi secara bergiliran
    # (round-robin) hingga mencapai batas 10 window.
    print("\n" + "=" * 70)
    print("UNDERSAMPLING ROUND-ROBIN PER SESI (Maks 10 per Subjek)")
    print("=" * 70)
    
    SESSION_GAP_HOURS = 4
    MAX_WINDOWS = 10
    rows_to_keep = []
    for subject, group in out_df.groupby('subject_id'):
        if len(group) <= MAX_WINDOWS:
            rows_to_keep.append(group)
        else:
            # Urutkan berdasarkan tanggal dan waktu
            group_sorted = group.sort_values(by=['record_date', 'record_time']).reset_index()
            
            # Buat session ID berdasarkan gap 4 jam
            session_ids = []
            current_session = 0
            prev_date = None
            prev_minutes = None
            
            for _, row in group_sorted.iterrows():
                date = row['record_date']
                time_str = row['record_time']
                curr_minutes = int(time_str[:2]) * 60 + int(time_str[2:4])
                
                if prev_date is None:
                    pass  # Sesi pertama
                elif date != prev_date:
                    current_session += 1  # Hari berbeda = sesi baru
                elif abs(curr_minutes - prev_minutes) >= SESSION_GAP_HOURS * 60:
                    current_session += 1  # Hari sama tapi gap > 4 jam = sesi baru
                
                session_ids.append(current_session)
                prev_date = date
                prev_minutes = curr_minutes
            
            group_sorted['session_id'] = session_ids
            
            # Bangun antrian per sesi (urutkan berdasarkan total_cv terendah)
            session_queues = {}
            session_order = []
            for sess_id, sess_group in group_sorted.groupby('session_id'):
                # SQA: Ambil window yang paling "mulus" (total_cv paling kecil) lebih dulu
                sess_group_sorted = sess_group.sort_values(by='total_cv')
                session_queues[sess_id] = list(sess_group_sorted['index'].values)  # index asli
                session_order.append(sess_id)
            
            # Round-Robin antar sesi
            selected_indices = []
            round_num = 0
            while len(selected_indices) < MAX_WINDOWS:
                added_this_round = False
                for sess_id in session_order:
                    if len(selected_indices) >= MAX_WINDOWS:
                        break
                    queue = session_queues[sess_id]
                    if round_num < len(queue):
                        selected_indices.append(queue[round_num])
                        added_this_round = True
                if not added_this_round:
                    break  # Semua sesi sudah habis
                round_num += 1
            
            pruned_group = out_df.loc[selected_indices]
            rows_to_keep.append(pruned_group)
            
            # Log detail pemilihan per sesi
            n_sessions = len(session_order)
            sess_detail = []
            for sess_id in session_order:
                sess_rows = group_sorted[group_sorted['session_id'] == sess_id]
                date_val = sess_rows['record_date'].iloc[0]
                time_val = sess_rows['record_time'].iloc[0]
                date_fmt = f"{date_val[:4]}-{date_val[4:6]}-{date_val[6:8]}"
                time_fmt = f"{time_val[:2]}:{time_val[2:4]}"
                count_in_pruned = len([i for i in selected_indices if i in list(sess_rows['index'].values)])
                sess_detail.append(f"{date_fmt} {time_fmt}={count_in_pruned}")
            detail = ', '.join(sess_detail)
            print(f"  -> {subject}: Dipangkas {len(group)} -> {len(pruned_group)} windows ({n_sessions} sesi: {detail})")
                
    out_df = pd.concat(rows_to_keep).reset_index(drop=True)
    
    # Urutkan berdasarkan subjek agar CSV rapi
    out_df = out_df.sort_values(by=['subject_id', 'timestamp']).reset_index(drop=True)
    
    # Hapus kolom meta yang tidak diperlukan untuk training
    out_df = out_df.drop(columns=['record_date', 'record_time', 'total_cv'])
    
    os.makedirs(os.path.dirname(OUTPUT_FILE), exist_ok=True)
    out_df.to_csv(OUTPUT_FILE, index=False)
    
    # Hancurkan arsip lama jika ada
    archive_path = os.path.join(os.path.dirname(OUTPUT_FILE), 'raw_data_archive.csv')
    if os.path.exists(archive_path):
        os.remove(archive_path)
    
    print("\n" + "=" * 70)
    print("DISTRIBUSI SUBJEK AKHIR (SETELAH UNDERSAMPLING)")
    print("=" * 70)
    subj_counts = out_df.groupby('subject_id').agg(
        windows=('bp_class', 'count'),
        classes=('bp_class', lambda x: f"{(x==0).sum()}N/{(x==1).sum()}H")
    )
    for subj, row in subj_counts.iterrows():
        print(f"  {subj}: {row['windows']} windows ({row['classes']})")
    print(f"\nTotal: {len(out_df)} windows dari {out_df['subject_id'].nunique()} subjek unik")
    
    print("\n" + "=" * 70)
    print(f"Extraction Successful! Processed {len(out_df)} windows.")
    print(f"Saved extracted features v2 to: {OUTPUT_FILE}")
    print("=" * 70)

if __name__ == "__main__":
    load_and_extract_features()