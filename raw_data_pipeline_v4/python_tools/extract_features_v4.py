"""
PIPELINE V4: Ekstraksi Fitur (Smart Round-Robin + Two-Pass SQA)
Fitur: ir_ratio, red_ratio, crest_time_ratio, heart_rate
Sumber data: raw_data_pipeline_v2/dataset_v2 (raw_data_*.csv)

Alur:
  1. Kumpulkan semua raw windows (komputasi SQA metrics)
  2. Smart Round-Robin per subjek:
       - Kluster rekaman dalam satu hari berdasarkan jarak waktu (gap >= SESSION_GAP_HOURS)
       - Dalam tiap kluster, urutkan dari kualitas terbaik (time_cv, amp_cv terendah)
       - Round-robin lintas kluster sampai maks MAX_PER_SUBJECT window
  3. Two-Pass SQA pada candidate_windows saja:
       - Pass 1: buang outlier ekstrem (mean + 2*std)
       - Pass 2: Q3 + 1.5*IQR dari distribusi bersih -> threshold final
  4. Filter candidate_windows berdasarkan threshold -> ekstraksi fitur
  5. Filter minority class per subjek
  6. Simpan CSV + header ESP32
"""
import os
import glob
from collections import defaultdict
from datetime import datetime

import pandas as pd
import numpy as np
import scipy.signal as signal

# ==========================================
# KONFIGURASI
# ==========================================
RAW_DATA_DIR  = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v2', 'dataset_v2'))
OUTPUT_DIR    = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'dataset_v4'))
OUTPUT_FILE   = os.path.join(OUTPUT_DIR, 'extracted_features_v4.csv')

ESP32_DIRS = [
    os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_rf_v4')),
    os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_svm_v4')),
    os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_rf_v4_no_oled')),
    os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'esp32_bp_inference_svm_v4_no_oled'))
]

SAMPLING_RATE     = 100    # Hz
MAX_PER_SUBJECT   = 10     # Maks window per subjek
SESSION_GAP_HOURS = 4.0    # Gap >= nilai ini = sesi berbeda dalam satu hari

# Subjek yang dikecualikan dari pipeline (data tetap ada di folder sebagai cadangan)
EXCLUDED_SUBJECTS = {'ELG', 'TMI', 'NN', 'LL', 'BKMI', 'PASP'}  # Tambahkan inisial subjek di sini jika perlu dikecualikan

# ==========================================
# HELPERS
# ==========================================
def get_subject_id(filename):
    parts = os.path.basename(filename).split('_')
    if len(parts) >= 3 and parts[0] == 'raw' and parts[1] == 'data':
        initial = parts[2]
        return None if initial.isdigit() else f"subj_{initial}"
    return None

def get_session_key(filename):
    parts = os.path.basename(filename).split('_')
    return f"{parts[3]}_{parts[4]}" if len(parts) >= 5 else 'unknown'

def smart_round_robin(windows_for_subj, max_windows=10, gap_hours=4.0):
    """
    Pilih hingga max_windows window dari satu subjek menggunakan
    round-robin berbasis kluster waktu.

    Mekanisme:
    - Rekaman dalam hari yang sama dikelompokkan menjadi kluster
      berdasarkan jarak waktu. Gap >= gap_hours = kluster baru.
    - Dalam satu kluster, window diurutkan dari kualitas terbaik
      (time_cv terendah, amp_cv terendah).
    - Round-robin lintas kluster: ambil 1 window terbaik dari tiap
      kluster per putaran hingga mencapai max_windows.
    - Jika satu hari hanya memiliki rekaman berdekatan (1 kluster),
      hanya diambil 1 window terlebih dahulu; putaran berikutnya
      baru mengambil window ke-2 dari kluster yang sama.
    """
    # Parse datetime
    for w in windows_for_subj:
        w['_dt'] = datetime.strptime(w['session'], "%Y%m%d_%H%M%S")

    # Urutkan semua window secara kronologis
    sorted_w = sorted(windows_for_subj, key=lambda x: x['_dt'])

    # Buat kluster berdasarkan gap waktu
    clusters = []
    cur = [sorted_w[0]]
    for i in range(1, len(sorted_w)):
        gap = (sorted_w[i]['_dt'] - sorted_w[i-1]['_dt']).total_seconds() / 3600.0
        if gap >= gap_hours:
            clusters.append(cur)
            cur = [sorted_w[i]]
        else:
            cur.append(sorted_w[i])
    clusters.append(cur)

    # Urutkan isi tiap kluster: kualitas terbaik di depan
    pools = [sorted(c, key=lambda x: (x['time_cv'], x['amp_cv'])) for c in clusters]

    # Round-robin lintas kluster
    selected = []
    pointers = [0] * len(pools)
    while len(selected) < max_windows:
        added = False
        for i, pool in enumerate(pools):
            if len(selected) >= max_windows:
                break
            if pointers[i] < len(pool):
                selected.append(pool[pointers[i]])
                pointers[i] += 1
                added = True
        if not added:
            break

    return selected, len(clusters)

# ==========================================
# PIPELINE UTAMA
# ==========================================
def extract():
    print("=" * 65)
    print("PIPELINE V4: Smart Round-Robin + Two-Pass SQA")
    print("=" * 65)

    csv_files = sorted(glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv')))
    print(f"Ditemukan {len(csv_files)} file raw data\n")

    # ------------------------------------------
    # STEP 1: Kumpulkan semua raw windows
    # ------------------------------------------
    print("STEP 1: Mengumpulkan seluruh raw windows + SQA metrics...")
    raw_windows = []

    for file in csv_files:
        subject_id = get_subject_id(file)
        if subject_id is None:
            continue
        # Skip subjek yang dikecualikan
        initials = subject_id.replace('subj_', '')
        if initials in EXCLUDED_SUBJECTS:
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
            ir_raw  = df['ir_raw'].values[s:e]
            red_raw = df['red_raw'].values[s:e]

            # Preprocessing
            nyq = 0.5 * SAMPLING_RATE
            b, a = signal.butter(2, [0.5 / nyq, 5.0 / nyq], btype='band')
            ir_flt  = -signal.filtfilt(b, a, signal.detrend(ir_raw))
            red_flt = -signal.filtfilt(b, a, signal.detrend(red_raw))

            # Peak detection
            peaks, _ = signal.find_peaks(ir_flt, distance=35, prominence=np.std(ir_flt) * 0.5)
            if len(peaks) < 2:
                continue

            rr      = np.diff(peaks)
            mean_rr = np.mean(rr)
            if mean_rr == 0:
                continue

            time_cv = (np.std(rr) / mean_rr) * 100.0

            amps     = ir_flt[peaks]
            mean_amp = np.mean(amps)
            if mean_amp == 0:
                continue
            amp_cv = np.std(amps) / mean_amp

            raw_windows.append({
                'file':        file,
                'w':           w,
                'num_windows': num_windows,
                'subject_id':  subject_id,
                'session':     session,
                'bp_class':    bp_class,
                'systolic':    systolic,
                'diastolic':   diastolic,
                'ir_raw':      ir_raw,
                'red_raw':     red_raw,
                'ir_flt':      ir_flt,
                'red_flt':     red_flt,
                'peaks':       peaks,
                'mean_rr':     mean_rr,
                'time_cv':     time_cv,
                'amp_cv':      amp_cv,
            })

    print(f"Terkumpul {len(raw_windows)} raw windows dari semua file.\n")

    # ------------------------------------------
    # STEP 2: Smart Round-Robin per Subjek
    # ------------------------------------------
    print("=" * 65)
    print(f"STEP 2: SMART ROUND-ROBIN (gap sesi >= {SESSION_GAP_HOURS:.0f} jam, maks {MAX_PER_SUBJECT}/subjek)")
    print("=" * 65)

    by_subject = defaultdict(list)
    for w in raw_windows:
        by_subject[w['subject_id']].append(w)

    candidate_windows = []
    for subj in sorted(by_subject.keys()):
        windows = by_subject[subj]
        selected, n_clusters = smart_round_robin(windows, MAX_PER_SUBJECT, SESSION_GAP_HOURS)
        candidate_windows.extend(selected)
        print(f"  {subj}: {len(windows)} raw window, {n_clusters} kluster -> terpilih {len(selected)}")

    print(f"\nTotal kandidat: {len(candidate_windows)} window dari {len(by_subject)} subjek\n")

    # ------------------------------------------
    # STEP 3: Two-Pass SQA Threshold (hanya dari candidate_windows)
    # ------------------------------------------
    print("=" * 65)
    print("STEP 3: TWO-PASS SQA THRESHOLD (dari kandidat saja)")
    print("=" * 65)

    time_cvs_all = np.array([d['time_cv'] for d in candidate_windows])
    amp_cvs_all  = np.array([d['amp_cv']  for d in candidate_windows])

    # Pass 1 — buang outlier ekstrem (mean + 2*std)
    p1_time = np.mean(time_cvs_all) + 2.0 * np.std(time_cvs_all)
    p1_amp  = np.mean(amp_cvs_all)  + 2.0 * np.std(amp_cvs_all)
    time_cvs_clean = time_cvs_all[time_cvs_all <= p1_time]
    amp_cvs_clean  = amp_cvs_all [amp_cvs_all  <= p1_amp]

    # Pass 2 — IQR dari distribusi bersih
    q3_t = np.percentile(time_cvs_clean, 75)
    q1_t = np.percentile(time_cvs_clean, 25)
    max_time_cv = q3_t + 1.5 * (q3_t - q1_t)

    q3_a = np.percentile(amp_cvs_clean, 75)
    q1_a = np.percentile(amp_cvs_clean, 25)
    max_amp_cv = q3_a + 1.5 * (q3_a - q1_a)

    print(f"  Kandidat total             : {len(time_cvs_all)} window")
    print(f"  Pass 1 batas kasar Time CV : mean+2s = {p1_time:.1f}%  -> sisa {len(time_cvs_clean)} window")
    print(f"  Pass 1 batas kasar Amp CV  : mean+2s = {p1_amp:.3f}   -> sisa {len(amp_cvs_clean)} window")
    print(f"  Pass 2 MAX_TIME_CV (IQR)   : {max_time_cv:.1f} %")
    print(f"  Pass 2 MAX_AMP_CV  (IQR)   : {max_amp_cv:.3f}")
    print("=" * 65 + "\n")

    # Generate header ESP32
    header_content = f"""// AUTO-GENERATED BY extract_features_v4.py
// Jangan diedit manual! File ini akan ditimpa setiap kali ekstraksi fitur.
#ifndef SQA_THRESHOLDS_H
#define SQA_THRESHOLDS_H

#define MAX_TIME_CV {max_time_cv:.1f}f
#define MAX_AMP_CV  {max_amp_cv:.3f}f

#endif
"""
    for folder in ESP32_DIRS:
        if os.path.exists(folder):
            h_path = os.path.join(folder, 'sqa_thresholds.h')
            with open(h_path, 'w') as f:
                f.write(header_content)
            print(f"[SAVED] {h_path}")
    print()

    # ------------------------------------------
    # STEP 3.5: SQA-Aware Candidate Replacement
    # ------------------------------------------
    # Setelah batas SQA diketahui, periksa apakah ada kandidat Round-Robin
    # yang PASTI akan gagal SQA (hard reject). Jika ya, ganti dengan
    # window cadangan dari subjek yang sama yang berpeluang lolos SQA.
    TIME_CV_BUFFER = 2.0
    AMP_CV_BUFFER = 0.05

    print("=" * 65)
    print("STEP 3.5: SQA-AWARE CANDIDATE REPLACEMENT")
    print("=" * 65)

    # Kelompokkan kandidat per subjek
    cand_by_subj = defaultdict(list)
    for w in candidate_windows:
        cand_by_subj[w['subject_id']].append(w)

    total_swaps = 0
    for subj in sorted(cand_by_subj.keys()):
        subj_candidates = cand_by_subj[subj]
        all_subj_windows = by_subject[subj]

        # Identifikasi kandidat yang akan HARD REJECT (di luar rescue buffer)
        hard_reject_indices = []
        for idx, w in enumerate(subj_candidates):
            beyond_rescue = (w['time_cv'] > (max_time_cv + TIME_CV_BUFFER) or
                             w['amp_cv'] > (max_amp_cv + AMP_CV_BUFFER))
            if beyond_rescue:
                hard_reject_indices.append(idx)

        if not hard_reject_indices:
            continue

        # Cari window cadangan: semua window subjek yang TIDAK terpilih Round-Robin
        cand_keys = set((w['file'], w['w']) for w in subj_candidates)
        unused = [w for w in all_subj_windows if (w['file'], w['w']) not in cand_keys]

        # Filter cadangan: hanya yang berpeluang lolos SQA (strict pass atau rescuable)
        good_unused = [w for w in unused
                       if w['time_cv'] <= (max_time_cv + TIME_CV_BUFFER) and
                          w['amp_cv'] <= (max_amp_cv + AMP_CV_BUFFER)]

        # Urutkan cadangan dari kualitas terbaik
        good_unused.sort(key=lambda x: (x['time_cv'], x['amp_cv']))

        # Lakukan penggantian
        for idx in hard_reject_indices:
            if not good_unused:
                break
            old_w = subj_candidates[idx]
            new_w = good_unused.pop(0)

            # Ganti di candidate_windows (list global)
            pos = candidate_windows.index(old_w)
            candidate_windows[pos] = new_w
            subj_candidates[idx] = new_w
            total_swaps += 1

            print(f"  [SWAP] {subj}:")
            print(f"    Keluar : {os.path.basename(old_w['file'])} w{old_w['w']+1} "
                  f"(TimeCV={old_w['time_cv']:.1f}%, AmpCV={old_w['amp_cv']:.3f})")
            print(f"    Masuk  : {os.path.basename(new_w['file'])} w{new_w['w']+1} "
                  f"(TimeCV={new_w['time_cv']:.1f}%, AmpCV={new_w['amp_cv']:.3f})")

    if total_swaps == 0:
        print("  Tidak ada penggantian diperlukan. Semua kandidat sudah optimal.")
    else:
        print(f"\n  Total penggantian: {total_swaps} window")
    print()

    # ------------------------------------------
    # STEP 4: Filter SQA + Ekstraksi Fitur (DENGAN RESCUE MECHANISM)
    # ------------------------------------------
    print("STEP 4: Filter SQA & Ekstraksi Fitur (DENGAN RESCUE MECHANISM)...")

    # Kelompokkan per subjek
    subject_groups = defaultdict(list)
    for w in candidate_windows:
        subject_groups[w['subject_id']].append(w)

    features_list = []
    
    for subj in sorted(subject_groups.keys()):
        subj_windows = subject_groups[subj]
        
        strictly_accepted = []
        buffer_candidates = []
        hard_rejected_msgs = []
        
        for d in subj_windows:
            if d['time_cv'] <= max_time_cv and d['amp_cv'] <= max_amp_cv:
                strictly_accepted.append(d)
            elif d['time_cv'] <= (max_time_cv + TIME_CV_BUFFER) and d['amp_cv'] <= (max_amp_cv + AMP_CV_BUFFER):
                buffer_candidates.append(d)
            else:
                hard_rejected_msgs.append(f"  -> [HARD REJECT] ({os.path.basename(d['file'])} w{d['w']+1}): Time CV {d['time_cv']:.1f}%, Amp CV {d['amp_cv']:.3f}")
                
        final_subj_windows = strictly_accepted.copy()
        
        rescued_msgs = []
        soft_reject_msgs = []
        
        if len(final_subj_windows) < MAX_PER_SUBJECT and len(buffer_candidates) > 0:
            needed = MAX_PER_SUBJECT - len(final_subj_windows)
            buffer_candidates = sorted(buffer_candidates, key=lambda x: (x['time_cv'], x['amp_cv']))
            rescued = buffer_candidates[:needed]
            final_subj_windows.extend(rescued)
            for r in rescued:
                rescued_msgs.append(f"  -> [RESCUED] ({os.path.basename(r['file'])} w{r['w']+1}): Time CV {r['time_cv']:.1f}%, Amp CV {r['amp_cv']:.3f}")
                
        for d in buffer_candidates:
            if d not in final_subj_windows:
                soft_reject_msgs.append(f"  -> [SOFT REJECT] ({os.path.basename(d['file'])} w{d['w']+1}): Time CV {d['time_cv']:.1f}%, Amp CV {d['amp_cv']:.3f}")

        # Cetak dengan urutan: nama subjek -> status OK -> detail reject/rescued
        print(f"[OK] {subj} -> {len(final_subj_windows)}/{len(subj_windows)} windows diterima")
        for msg in hard_rejected_msgs:
            print(msg)
        for msg in rescued_msgs:
            print(msg)
        for msg in soft_reject_msgs:
            print(msg)
                
        # Ekstraksi fitur untuk final_subj_windows
        for d in final_subj_windows:
            num_windows = d['num_windows']
            
            # Crest Time Ratio
            rise_times = []
            for p in d['peaks']:
                trough_idx = p
                for j in range(p - 1, max(0, p - 45) - 1, -1):
                    if d['ir_flt'][j] <= d['ir_flt'][trough_idx]:
                        trough_idx = j
                    else:
                        break
                if p > trough_idx:
                    rise_times.append((p - trough_idx) * 10.0)
            if not rise_times:
                continue

            mean_rise  = np.mean(rise_times)
            heart_rate = 60.0 / (d['mean_rr'] / SAMPLING_RATE)

            ir_ratio  = (np.std(d['ir_flt'])  / np.mean(d['ir_raw']))  * 100.0 if np.mean(d['ir_raw'])  > 0 else 0
            red_ratio = (np.std(d['red_flt']) / np.mean(d['red_raw'])) * 100.0 if np.mean(d['red_raw']) > 0 else 0
            ctr       = mean_rise / (60000.0 / heart_rate) if heart_rate > 0 else 0

            features_list.append({
                'subject_id':       d['subject_id'],
                'session':          d['session'],
                'time_cv':          round(d['time_cv'], 2),
                'amp_cv':           round(d['amp_cv'],  3),
                'ir_ratio':         round(ir_ratio,     4),
                'red_ratio':        round(red_ratio,    4),
                'crest_time_ratio': round(ctr,          4),
                'heart_rate':       round(heart_rate,   2),
                'systolic_bp':      d['systolic'],
                'diastolic_bp':     d['diastolic'],
                'bp_class':         d['bp_class'],
                'notes':            f"win_{d['w']+1}_of_{num_windows}",
            })

    df_out = pd.DataFrame(features_list)
    if df_out.empty:
        print("\n[ERROR] Tidak ada data yang berhasil diekstraksi!")
        return

    # ------------------------------------------
    # STEP 5: Filter Minority Class per Subjek
    # ------------------------------------------
    print("\n" + "=" * 65)
    print("STEP 5: FILTER MINORITY CLASS PER SUBJEK")
    print("=" * 65)
    filtered = []
    for subj, grp in df_out.groupby('subject_id'):
        mode_class = grp['bp_class'].mode()[0]
        dropped    = (grp['bp_class'] != mode_class).sum()
        if dropped > 0:
            print(f"  -> {subj}: Dibuang {dropped} window minority class")
        filtered.append(grp[grp['bp_class'] == mode_class])
    df_out = pd.concat(filtered).reset_index(drop=True)

    # ------------------------------------------
    # STEP 6: Distribusi & Simpan
    # ------------------------------------------
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

    os.makedirs(OUTPUT_DIR, exist_ok=True)
    df_out.to_csv(OUTPUT_FILE, index=False)
    print(f"\n[SAVED] {OUTPUT_FILE}")

    print("\n" + "=" * 65)
    print("REKAP BATAS SQA UNTUK ESP32")
    print("=" * 65)
    print(f"MAX_TIME_CV : {max_time_cv:.1f} %")
    print(f"MAX_AMP_CV  : {max_amp_cv:.3f}")
    print("=" * 65 + "\n")


if __name__ == '__main__':
    extract()
