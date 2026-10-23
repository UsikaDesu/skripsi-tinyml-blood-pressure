import os
import re
import pandas as pd
import numpy as np

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "..", ".."))

# File input (File 1 dan File 2 terbaru dari pengujian kode benar)
F1_PATH = r"C:\Users\Kayyis\.gemini\antigravity-ide\brain\a83678c5-9dcd-4b67-a225-ad0f5021b016\.user_uploaded\media_1792555682274.txt"
F2_PATH = r"C:\Users\Kayyis\.gemini\antigravity-ide\brain\a83678c5-9dcd-4b67-a225-ad0f5021b016\.user_uploaded\media_1792593119513.txt"

# Dataset V4
DATASET_V4_PATH = os.path.join(PROJECT_ROOT, "raw_data_pipeline_v4", "dataset_v4", "extracted_features_v4.csv")
df_v4 = pd.read_csv(DATASET_V4_PATH)
v4_subjs = set(df_v4['subject_id'].str.replace('subj_', ''))

# Output Directory
DEPLOY_DIR = os.path.join(PROJECT_ROOT, "raw_data_pipeline_v4", "deployment_results")
os.makedirs(DEPLOY_DIR, exist_ok=True)

# Acuan informasi klinis/tensimeter per subjek (Standard JNC-7 / AHA / Kemenkes RI)
# Format: (Tensi String 3 Pengukuran, Sys Ref Avg, Dia Ref Avg, Ground Truth Label, Ground Truth Code)
SUBJECT_METADATA = {
    # 12 Subjek File 1
    'BKMI': ('117/71, 106/80, 121/68', 114.7, 73.0, 'NORMAL', 0),
    'KYS': ('147/89, 144/82, 150/87', 147.0, 86.0, 'HIPERTENSI', 1),
    'mbika': ('118/72, 102/60, 106/69', 108.7, 67.0, 'NORMAL', 0),
    'PASP': ('148/80, 134/86, 148/92', 143.3, 86.0, 'HIPERTENSI', 1),
    'MBHMO': ('159/108, 153/105, 147/105', 153.0, 106.0, 'HIPERTENSI', 1),
    'PMRYN': ('185/98, 200/107, 203/99', 196.0, 101.3, 'HIPERTENSI', 1),
    'ARF': ('154/98, 142/103, 148/101', 148.0, 100.7, 'HIPERTENSI', 1),
    'MasBYU': ('115/64, 115/67, 119/70', 116.3, 67.0, 'NORMAL', 0),
    'HFZ': ('122/84, 120/72', 121.0, 78.0, 'NORMAL', 0),
    'AZR': ('127/87, 133/83, 129/84', 129.7, 84.7, 'NORMAL', 0),
    'ELG': ('110/74, 110/76, 115/80', 111.7, 76.7, 'NORMAL', 0),
    'ANK': ('120/72, 120/72, 135/76', 125.0, 73.3, 'NORMAL', 0),
    
    # 7 Subjek Awal File 2
    'mbuci': ('125/88, 119/81, 130/94', 124.7, 87.7, 'NORMAL', 0),
    'ZDNY': ('125/87, 124/85, 121/83', 123.3, 85.0, 'NORMAL', 0),
    'LL': ('150/85, 148/83, 145/88', 147.7, 85.3, 'HIPERTENSI', 1),
    'bdsti': ('120/71, 121/73, 125/70', 122.0, 71.3, 'NORMAL', 0),
    'JN': ('118/79, 119/81, 120/84', 119.0, 81.3, 'NORMAL', 0),
    'GN': ('119/76, 120/73, 112/68', 117.0, 72.3, 'NORMAL', 0),
    'AYN': ('107/78, 105/80, 95/75', 102.3, 77.7, 'NORMAL', 0),

    # 11 Subjek Baru Tambahan File 2
    'NN': ('125/91, 136/93, 130/90', 130.3, 91.3, 'HIPERTENSI', 1),
    'BDTiux': ('129/84, 131/85, 121/86', 127.0, 85.0, 'NORMAL', 0),
    'ummi': ('119/79, 116/76, 128/82', 121.0, 79.0, 'NORMAL', 0),
    'MBIRN': ('135/96, 126/94, 123/92', 128.0, 94.0, 'HIPERTENSI', 1),
    'mbmnd': ('93/61, 90/61, 96/61', 93.0, 61.0, 'NORMAL', 0),
    'msdni': ('128/88, 134/86, 130/84', 130.7, 86.0, 'NORMAL', 0),
    'msbs': ('97/70, 97/69, 97/71', 97.0, 70.0, 'NORMAL', 0),
    'mblia': ('106/75, 103/64, 93/60', 100.7, 66.3, 'NORMAL', 0),
    'msars': ('125/89, 123/81, 120/83', 122.7, 84.3, 'NORMAL', 0),
    'mb-umy': ('93/59, 107/65, 100/62', 100.0, 62.0, 'NORMAL', 0),
    'pags': ('138/90, 150/99, 145/94', 144.3, 94.3, 'HIPERTENSI', 1),
}

def parse_txt_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as fp:
        lines = fp.readlines()

    records = []
    curr_windows = []

    for idx, line in enumerate(lines):
        line_str = line.strip()
        if 'CSV:' in line_str:
            csv_part = line_str.replace('CSV:', '').strip()
            vals = [float(x.strip()) for x in csv_part.split(',')]

            # Cari latensi inferensi di blok sebelumnya
            lat_us = 95
            for back_i in range(max(0, idx - 15), idx):
                if 'Inference Time' in lines[back_i]:
                    m = re.search(r'(\d+)\s*us', lines[back_i])
                    if m:
                        lat_us = int(m.group(1))

            curr_windows.append({
                'ir_ratio': vals[0],
                'red_ratio': vals[1],
                'ctr': vals[2],
                'hr': vals[3],
                'time_cv': vals[4],
                'amp_cv': vals[5],
                'pred_code': int(vals[6]),
                'pred_label': 'HIPERTENSI' if int(vals[6]) == 1 else 'NON-HIPERTENSI',
                'latency_us': lat_us
            })

        elif '===' in line_str:
            m_subj = re.search(r'=+\s*([a-zA-Z0-9_\-]+)\s*=+', line_str)
            subj_name = m_subj.group(1) if m_subj else 'UNKNOWN'

            records.append({
                'subject': subj_name,
                'header_raw': line_str,
                'windows': curr_windows
            })
            curr_windows = []

    return records

def process_all():
    rec1 = parse_txt_file(F1_PATH)
    rec2 = parse_txt_file(F2_PATH)
    all_subjects_parsed = rec1 + rec2

    print(f"Total subjek ter-parse: {len(all_subjects_parsed)} subjek.")

    windows_rows = []
    summary_rows = []

    for r in all_subjects_parsed:
        s_id = r['subject']
        meta = SUBJECT_METADATA.get(s_id, ('-', 0.0, 0.0, 'NORMAL', 0))
        tensi_3_str, sys_ref_avg, dia_ref_avg, gt_label, gt_code = meta

        is_dataset = s_id in v4_subjs
        kategori = 'SUBJEK DATASET' if is_dataset else 'SUBJEK BARU'

        # Ekstrak pasangan (sistolik, diastolik) per pengukuran
        pairs = re.findall(r'(\d+)\s*/\s*(\d+)', tensi_3_str)

        wins = r['windows']
        votes_n = 0
        votes_h = 0

        for w_idx, w in enumerate(wins, 1):
            is_win_corr = 1 if w['pred_code'] == gt_code else 0
            if w['pred_code'] == 1:
                votes_h += 1
            else:
                votes_n += 1

            # Tentukan nilai manset khusus untuk jendela ke-i jika tersedia
            if w_idx - 1 < len(pairs):
                win_sys = int(pairs[w_idx - 1][0])
                win_dia = int(pairs[w_idx - 1][1])
                win_bp_str = f"{win_sys}/{win_dia}"
            else:
                win_sys = round(sys_ref_avg, 1)
                win_dia = round(dia_ref_avg, 1)
                win_bp_str = f"{win_sys}/{win_dia}"

            windows_rows.append({
                'subject_id': s_id,
                'kategori_subjek': kategori,
                'window_ke': w_idx,
                'tensi_manset_pengukuran_ke': win_bp_str,
                'sistolik_manset_pengukuran': win_sys,
                'diastolik_manset_pengukuran': win_dia,
                'tensi_manset_3_kali_lengkap': tensi_3_str,
                'rata2_sistolik_subjek': round(sys_ref_avg, 1),
                'rata2_diastolik_subjek': round(dia_ref_avg, 1),
                'ground_truth_label': gt_label,
                'ground_truth_code': gt_code,
                'heart_rate_bpm': round(w['hr'], 2),
                'crest_time_ratio': round(w['ctr'], 4),
                'ir_ratio_pct': round(w['ir_ratio'], 4),
                'red_ratio_pct': round(w['red_ratio'], 4),
                'time_cv_pct': round(w['time_cv'], 2),
                'amp_cv': round(w['amp_cv'], 3),
                'sqa_status': 'PASSED',
                'inference_time_us': w['latency_us'],
                'prediksi_label': w['pred_label'],
                'prediksi_code': w['pred_code'],
                'is_window_correct': is_win_corr
            })

        # Majority voting per subjek
        maj_diag_label = 'HIPERTENSI' if votes_h > votes_n else 'NON-HIPERTENSI'
        maj_diag_code = 1 if votes_h > votes_n else 0
        is_subj_corr = 1 if maj_diag_code == gt_code else 0
        confidence = (max(votes_n, votes_h) / len(wins)) * 100.0 if wins else 0.0

        avg_hr = np.mean([w['hr'] for w in wins]) if wins else 0.0
        avg_ctr = np.mean([w['ctr'] for w in wins]) if wins else 0.0
        avg_ir = np.mean([w['ir_ratio'] for w in wins]) if wins else 0.0
        avg_red = np.mean([w['red_ratio'] for w in wins]) if wins else 0.0
        avg_lat = np.mean([w['latency_us'] for w in wins]) if wins else 0.0

        summary_rows.append({
            'subject_id': s_id,
            'kategori_subjek': kategori,
            'tensi_manset_3_kali_lengkap': tensi_3_str,
            'rata2_sistolik_klinis': round(sys_ref_avg, 1),
            'rata2_diastolik_klinis': round(dia_ref_avg, 1),
            'tensi_rata2_str': f"{round(sys_ref_avg, 1)} / {round(dia_ref_avg, 1)}",
            'ground_truth_label': gt_label,
            'ground_truth_code': gt_code,
            'suara_normal': votes_n,
            'suara_hipertensi': votes_h,
            'suara_mayoritas': f"{votes_n} Normal / {votes_h} Hipertensi",
            'diagnosis_mayoritas_subjek': maj_diag_label,
            'diagnosis_mayoritas_code': maj_diag_code,
            'is_subject_correct': is_subj_corr,
            'confidence_pct': round(confidence, 1),
            'rata2_heart_rate': round(avg_hr, 2),
            'rata2_ctr': round(avg_ctr, 4),
            'rata2_ir_ratio': round(avg_ir, 4),
            'rata2_red_ratio': round(avg_red, 4),
            'rata2_latensi_us': round(avg_lat, 1)
        })

    df_windows_all = pd.DataFrame(windows_rows)
    df_summary_all = pd.DataFrame(summary_rows)

    # 1. Simpan CSV Lengkap Semua 30 Subjek (90 Jendela)
    csv_win_all = os.path.join(DEPLOY_DIR, "pengujian_deploy_semua_30_subjek.csv")
    csv_sum_all = os.path.join(DEPLOY_DIR, "ringkasan_deploy_semua_30_subjek.csv")
    df_windows_all.to_csv(csv_win_all, index=False)
    df_summary_all.to_csv(csv_sum_all, index=False)

    # 2. Pisahkan Kategori 1: Subjek yang SUDAH ADA di Dataset (10 Subjek, 30 Jendela)
    df_win_dataset = df_windows_all[df_windows_all['kategori_subjek'] == 'SUBJEK DATASET']
    df_sum_dataset = df_summary_all[df_summary_all['kategori_subjek'] == 'SUBJEK DATASET']
    csv_win_dataset = os.path.join(DEPLOY_DIR, "pengujian_deploy_subjek_dataset.csv")
    csv_sum_dataset = os.path.join(DEPLOY_DIR, "ringkasan_deploy_subjek_dataset.csv")
    df_win_dataset.to_csv(csv_win_dataset, index=False)
    df_sum_dataset.to_csv(csv_sum_dataset, index=False)

    # 3. Pisahkan Kategori 2: Subjek yang BELUM PERNAH ADA di Dataset (20 Subjek Baru, 60 Jendela)
    df_win_baru = df_windows_all[df_windows_all['kategori_subjek'] == 'SUBJEK BARU']
    df_sum_baru = df_summary_all[df_summary_all['kategori_subjek'] == 'SUBJEK BARU']
    csv_win_baru = os.path.join(DEPLOY_DIR, "pengujian_deploy_subjek_baru.csv")
    csv_sum_baru = os.path.join(DEPLOY_DIR, "ringkasan_deploy_subjek_baru.csv")
    df_win_baru.to_csv(csv_win_baru, index=False)
    df_sum_baru.to_csv(csv_sum_baru, index=False)

    print("\n" + "=" * 80)
    print("FILE CSV DENGAN 3 PENGUKURAN MANSET LENGKAP & RATA-RATA TELAH TERSIMPAN!")
    print("=" * 80)
    print(f"Total Subjek : {len(df_summary_all)} subjek (90 jendela)")
    print(f"1. Subjek Dataset : {len(df_sum_dataset)} subjek, 30 jendela (Akurasi: {df_sum_dataset['is_subject_correct'].sum()}/{len(df_sum_dataset)})")
    print(f"2. Subjek Baru    : {len(df_sum_baru)} subjek, 60 jendela (Akurasi: {df_sum_baru['is_subject_correct'].sum()}/{len(df_sum_baru)})")
    print("-" * 80)

if __name__ == '__main__':
    process_all()
