# -*- coding: utf-8 -*-
"""
DEPLOY DATA COLLECTOR V4 (TINYML ESP32 REAL-TIME LOGGER)
=========================================================
Alat pengambil dan pencatat data hasil pengujian implementasi (deployment)
ESP32 Blood Pressure Classifier V4 (Random Forest / SVM).

Fungsi:
1. Menghubungkan PC ke ESP32 secara otomatis via Serial (115200 baud).
2. Mencatat identitas subjek dan tekanan darah acuan (Ground Truth Tensimeter Manset).
3. Menangkap hasil ekstraksi 4 fitur, uji kualitas sinyal (SQA), latensi inferensi,
   dan prediksi kelas secara live dari ESP32.
4. Menghitung evaluasi tingkat jendela (Window-Level) dan tingkat subjek (Majority Voting).
5. Menyimpan hasil pengujian ke format CSV terstruktur untuk analisis Bab 4 Skripsi:
   - deploy_windows_v4.csv          : Detail setiap jendela pengukuran 10 detik
   - deploy_subjects_summary_v4.csv  : Ringkasan diagnosis akhir per subjek uji
"""

import serial
import serial.tools.list_ports
import time
import os
import csv
import re
from datetime import datetime

# ==========================================
# KONFIGURASI DIREKTORI OUTPUT
# ==========================================
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DEPLOY_DIR = os.path.abspath(os.path.join(BASE_DIR, '..', 'deployment_results'))
os.makedirs(DEPLOY_DIR, exist_ok=True)

WINDOWS_CSV_PATH = os.path.join(DEPLOY_DIR, "deploy_windows_v4.csv")
SUMMARY_CSV_PATH = os.path.join(DEPLOY_DIR, "deploy_subjects_summary_v4.csv")
BAUD_RATE = 115200

# ==========================================
# DETEKSI OTOMATIS PORT ESP32
# ==========================================
def find_esp32_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        desc = port.description or ""
        if any(kw in desc for kw in ["CP210x", "CH340", "CH341", "USB Serial", "UART"]):
            print(f"[AUTO-DETECT] ESP32 ditemukan pada: {port.device} ({desc})")
            return port.device

    print("\n[PERINGATAN] Port ESP32 tidak terdeteksi otomatis. Daftar port aktif:")
    all_ports = list(serial.tools.list_ports.comports())
    if all_ports:
        for i, p in enumerate(all_ports):
            print(f"  {i+1}. {p.device} - {p.description}")
        idx = input("Pilih nomor port (tekan Enter untuk COM3): ").strip()
        if idx.isdigit() and 1 <= int(idx) <= len(all_ports):
            return all_ports[int(idx) - 1].device

    fallback = 'COM3' if os.name == 'nt' else '/dev/ttyUSB0'
    print(f"[FALLBACK] Menggunakan port default: {fallback}")
    return fallback

# ==========================================
# INISIALISASI HEADER FILE CSV
# ==========================================
def init_csv_headers():
    if not os.path.exists(WINDOWS_CSV_PATH):
        with open(WINDOWS_CSV_PATH, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow([
                "session_timestamp", "subject_id", "age", "gender", "model_type",
                "systolic_ref", "diastolic_ref", "ground_truth_label", "ground_truth_code",
                "window_index", "ir_ratio", "red_ratio", "crest_time_ratio", "heart_rate",
                "time_cv_sqa", "amp_cv_sqa", "inference_time", "prediction_label",
                "prediction_code", "is_window_correct", "sqa_status"
            ])

    if not os.path.exists(SUMMARY_CSV_PATH):
        with open(SUMMARY_CSV_PATH, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow([
                "session_timestamp", "subject_id", "age", "gender", "model_type",
                "systolic_ref", "diastolic_ref", "ground_truth_label", "ground_truth_code",
                "total_windows", "valid_sqa_windows", "rejected_sqa_windows",
                "votes_normal", "votes_hipertensi", "final_subject_prediction",
                "final_prediction_code", "is_subject_correct", "confidence_pct",
                "avg_heart_rate", "avg_infer_time"
            ])

# ==========================================
# INPUT METADATA PENGUJIAN SUBJEK
# ==========================================
def prompt_subject_metadata():
    print("\n" + "=" * 65)
    print("   INPUT IDENTITAS SUBJEK & DATA ACUAN (GROUND TRUTH)")
    print("=" * 65)

    # 1. Inisial Subjek
    while True:
        sub_id = input("1. Masukkan Inisial / ID Subjek (contoh: KYS, UJI01): ").strip().upper()
        if sub_id:
            break
        print("   [ERROR] Inisial tidak boleh kosong!")

    # 2. Usia & Gender (Opsional)
    age = input("2. Usia (opsional, tekan Enter untuk lewati) : ").strip()
    gender = input("3. Gender (L/P, tekan Enter untuk lewati)   : ").strip().upper()

    # 3. Model yang Digunakan di ESP32
    print("4. Model yang sedang terpasang di firmware ESP32:")
    print("   [1] Random Forest (RF-10t)")
    print("   [2] Support Vector Machine (SVM-RBF)")
    m_choice = input("   Pilih (1/2, default 1): ").strip()
    model_type = "SVM-RBF" if m_choice == "2" else "Random Forest"

    # 4. Nilai Tekanan Darah Acuan dari Tensimeter Dokter / Omron
    while True:
        bp_input = input("5. Tekanan Darah Acuan Tensimeter (Format Sistolik/Diastolik, misal 120/80): ").strip()
        try:
            if '/' not in bp_input:
                raise ValueError
            s_str, d_str = bp_input.split('/')
            sys_val = int(s_str.strip())
            dia_val = int(d_str.strip())
            if 60 <= sys_val <= 250 and 40 <= dia_val <= 150:
                break
            print("   [PERINGATAN] Nilai tensi tidak wajar (Sistolik: 60-250, Diastolik: 40-150).")
        except ValueError:
            print("   [ERROR] Format salah! Contoh yang benar: 125/82 atau 140/90")

    # Klasifikasi Acuan: JNC-7 / WHO (Sistolik >= 140 atau Diastolik >= 90 -> Hipertensi)
    is_ht = (sys_val >= 140 or dia_val >= 90)
    gt_label = "HIPERTENSI" if is_ht else "NON-HIPERTENSI"
    gt_code = 1 if is_ht else 0

    print(f"\n   -> Kategori Acuan Medis: {gt_label} (Kelas {gt_code})")
    confirm = input("   Apakah klasifikasi acuan ini sudah sesuai? (Y/n): ").strip().upper()
    if confirm == 'N':
        gt_code = 1 if gt_code == 0 else 0
        gt_label = "HIPERTENSI" if gt_code == 1 else "NON-HIPERTENSI"
        print(f"   -> Klasifikasi diubah menjadi: {gt_label} (Kelas {gt_code})")

    # 5. Target Jumlah Jendela
    target_win_input = input("6. Target Jumlah Jendela untuk Subjek ini (default 10): ").strip()
    target_windows = int(target_win_input) if target_win_input.isdigit() and int(target_win_input) > 0 else 10

    return {
        "subject_id": sub_id,
        "age": age if age else "-",
        "gender": gender if gender else "-",
        "model_type": model_type,
        "systolic_ref": sys_val,
        "diastolic_ref": dia_val,
        "gt_label": gt_label,
        "gt_code": gt_code,
        "target_windows": target_windows
    }

# ==========================================
# PARSER LOG DARI SERIAL ESP32
# ==========================================
def parse_csv_line(line):
    """
    Format dari ESP32:
    CSV: ir_ratio, red_ratio, crest_time_ratio, heart_rate, time_cv, amp_cv, prediction
    """
    if not line.startswith("CSV:"):
        return None
    raw = line.replace("CSV:", "").strip()
    parts = raw.split(',')
    if len(parts) >= 7:
        try:
            return {
                "ir_ratio": float(parts[0]),
                "red_ratio": float(parts[1]),
                "ctr": float(parts[2]),
                "hr": float(parts[3]),
                "time_cv": float(parts[4]),
                "amp_cv": float(parts[5]),
                "prediction": int(parts[6])
            }
        except ValueError:
            return None
    return None

# ==========================================
# SESI PENGUJIAN UTAMA
# ==========================================
def run_deployment_session():
    init_csv_headers()
    meta = prompt_subject_metadata()

    port = find_esp32_port()
    print(f"\nMenghubungkan ke ESP32 pada {port} ({BAUD_RATE} baud)...")

    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=0.5)
        ser.reset_input_buffer()
    except serial.SerialException as e:
        print(f"[ERROR] Gagal membuka port serial: {e}")
        print("Pastikan Serial Monitor di Arduino/PlatformIO sudah DITUTUP sebelum menjalankan script ini.")
        return

    print("\n" + "=" * 65)
    print(f"  SESI PENGUJIAN AKTIF: SUBJEK {meta['subject_id']}")
    print(f"  Model: {meta['model_type']} | Acuan: {meta['gt_label']} ({meta['systolic_ref']}/{meta['diastolic_ref']} mmHg)")
    print(f"  Target: {meta['target_windows']} Jendela Pengukuran")
    print("  (Letakkan jari subjek pada sensor MAX30102 dan tahan posisi)")
    print("  Tekan 'Ctrl+C' kapan saja untuk mengakhiri sesi lebih awal.")
    print("=" * 65 + "\n")

    windows_data = []
    rejected_count = 0
    current_infer_time = "-"
    window_counter = 0

    try:
        while window_counter < meta['target_windows']:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if not line:
                    continue

                # Tampilkan log informatif dari firmware ESP32
                if "Waiting for finger" in line:
                    print(f"  [ESP32] Menunggu jari subjek menempel pada sensor...")
                elif "Finger detected" in line:
                    print(f"  [ESP32] Jari terdeteksi! Memulai stabilisasi...")
                elif "Collecting 1000 samples" in line:
                    print(f"  [ESP32] Sedang merekam jendela 10 detik (1000 sampel)... Mohon jangan bergerak!")
                elif "Processing DSP" in line:
                    print(f"  [ESP32] Perekaman selesai. Memproses Bandpass Filter & Detrending...")
                elif "ABORTED" in line:
                    print(f"  [ESP32] ⚠️ Perekaman dibatalkan: Jari terlepas sebelum 10 detik!")

                # Tangkap SQA Rejected
                if "SQA REJECTED" in line:
                    rejected_count += 1
                    print(f"  [ESP32] ❌ Sinyal DITOLAK oleh SQA! (Noise/Gerakan terdeteksi) -> {line}")
                    # Rekam ke data jendela sebagai jendela gagal
                    timestamp_now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    with open(WINDOWS_CSV_PATH, 'a', newline='', encoding='utf-8') as f:
                        writer = csv.writer(f)
                        writer.writerow([
                            timestamp_now, meta['subject_id'], meta['age'], meta['gender'],
                            meta['model_type'], meta['systolic_ref'], meta['diastolic_ref'],
                            meta['gt_label'], meta['gt_code'],
                            window_counter + 1, 0, 0, 0, 0, 0, 0, "-",
                            "REJECTED", -1, 0, "REJECTED"
                        ])

                # Tangkap Waktu Inferensi jika tercetak
                if "Inference Time" in line:
                    match = re.search(r'Inference Time\s*:\s*(\d+)\s*(us|ms)', line)
                    if match:
                        current_infer_time = f"{match.group(1)} {match.group(2)}"

                # Tangkap Baris Data CSV Hasil Inferensi
                if line.startswith("CSV:"):
                    parsed = parse_csv_line(line)
                    if parsed:
                        window_counter += 1
                        pred_code = parsed['prediction']
                        pred_label = "HIPERTENSI" if pred_code == 1 else "NON-HIPERTENSI"
                        is_correct = 1 if pred_code == meta['gt_code'] else 0

                        parsed['infer_time'] = current_infer_time
                        parsed['pred_label'] = pred_label
                        parsed['is_correct'] = is_correct
                        parsed['window_idx'] = window_counter
                        windows_data.append(parsed)

                        # Tampilkan Card Ringkasan Jendela
                        status_simbol = "✅ COCOK" if is_correct else "❌ BERBEDA"
                        print("\n" + "┌" + "─" * 63 + "┐")
                        print(f"│ JENDELA KE-{window_counter}/{meta['target_windows']} SELESAI                   {datetime.now().strftime('%H:%M:%S')} │")
                        print("├" + "─" * 63 + "┤")
                        print(f"│  Heart Rate     : {parsed['hr']:>6.1f} BPM   | Crest Time R : {parsed['ctr']:>6.3f}     │")
                        print(f"│  IR Ratio       : {parsed['ir_ratio']:>6.3f} %     | RED Ratio    : {parsed['red_ratio']:>6.3f} %   │")
                        print(f"│  Kualitas (SQA) : TimeCV={parsed['time_cv']:.1f}% | AmpCV={parsed['amp_cv']:.3f} (Lolos)       │")
                        print(f"│  Latensi ESP32  : {current_infer_time:<15}                           │")
                        print(f"│  Hasil Prediksi : {pred_label:<14} [Acuan: {meta['gt_label']:<14}]│")
                        print(f"│  Evaluasi       : {status_simbol:<51} │")
                        print("└" + "─" * 63 + "┘")

                        # Simpan ke CSV Jendela Langsung (Flush ke Disk)
                        timestamp_now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                        with open(WINDOWS_CSV_PATH, 'a', newline='', encoding='utf-8') as f:
                            writer = csv.writer(f)
                            writer.writerow([
                                timestamp_now, meta['subject_id'], meta['age'], meta['gender'],
                                meta['model_type'], meta['systolic_ref'], meta['diastolic_ref'],
                                meta['gt_label'], meta['gt_code'],
                                window_counter, parsed['ir_ratio'], parsed['red_ratio'],
                                parsed['ctr'], parsed['hr'], parsed['time_cv'], parsed['amp_cv'],
                                current_infer_time, pred_label, pred_code, is_correct, "PASSED"
                            ])

                        # Hitung Konsensus Sementara
                        votes_h = sum(1 for w in windows_data if w['prediction'] == 1)
                        votes_n = sum(1 for w in windows_data if w['prediction'] == 0)
                        temp_subj_pred = "HIPERTENSI" if votes_h > votes_n else "NON-HIPERTENSI"
                        print(f"  -> Akumulasi Suara Subjek: {votes_n} Non-Hipertensi vs {votes_h} Hipertensi (Diagnosis Sementara: {temp_subj_pred})\n")

                        # Reset infer time buffer
                        current_infer_time = "-"

            time.sleep(0.01)

    except KeyboardInterrupt:
        print("\n\n[INTERRUPT] Sesi pengujian dihentikan secara manual oleh pengguna.")
    finally:
        ser.close()

    # ==========================================
    # RINGKASAN DIAGNOSIS AKHIR SUBJEK (MAJORITY VOTING)
    # ==========================================
    total_valid = len(windows_data)
    if total_valid == 0:
        print("\n[INFO] Belum ada jendela data yang berhasil direkam.")
        return

    votes_h = sum(1 for w in windows_data if w['prediction'] == 1)
    votes_n = sum(1 for w in windows_data if w['prediction'] == 0)

    # Majority voting diagnosis
    final_pred_code = 1 if votes_h > votes_n else 0
    final_pred_label = "HIPERTENSI" if final_pred_code == 1 else "NON-HIPERTENSI"
    is_subject_correct = 1 if final_pred_code == meta['gt_code'] else 0
    confidence = (max(votes_h, votes_n) / total_valid) * 100.0
    avg_hr = sum(w['hr'] for w in windows_data) / total_valid

    # Ekstrak rata-rata latensi
    latencies = []
    for w in windows_data:
        m = re.search(r'(\d+)', w.get('infer_time', ''))
        if m:
            latencies.append(int(m.group(1)))
    avg_lat_str = f"{sum(latencies)/len(latencies):.0f} us" if latencies else "-"

    # Simpan Ringkasan ke Master Summary CSV
    timestamp_session = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(SUMMARY_CSV_PATH, 'a', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow([
            timestamp_session, meta['subject_id'], meta['age'], meta['gender'],
            meta['model_type'], meta['systolic_ref'], meta['diastolic_ref'],
            meta['gt_label'], meta['gt_code'],
            total_valid + rejected_count, total_valid, rejected_count,
            votes_n, votes_h, final_pred_label, final_pred_code,
            is_subject_correct, round(confidence, 1), round(avg_hr, 1), avg_lat_str
        ])

    print("\n" + "=" * 65)
    print("   KESIMPULAN DIAGNOSIS SUBJEK (SUBJECT-LEVEL DIAGNOSIS)")
    print("=" * 65)
    print(f"  Subjek Uji               : {meta['subject_id']} (Usia: {meta['age']}, Gender: {meta['gender']})")
    print(f"  Tekanan Darah Acuan      : {meta['systolic_ref']}/{meta['diastolic_ref']} mmHg ({meta['gt_label']})")
    print(f"  Total Jendela Diukur     : {total_valid} Jendela Valid ({rejected_count} ditolak SQA)")
    print(f"  Distribusi Voting Jendela: {votes_n} Non-Hipertensi vs {votes_h} Hipertensi")
    print(f"  Konsistensi Model        : {confidence:.1f}% kesepakatan internal")
    print(f"  Rata-rata Denyut Jantung : {avg_hr:.1f} BPM")
    print(f"  Diagnosis Akhir Alat     : {final_pred_label}")
    print(f"  Status Evaluasi Klinis   : {'✅ TEPAT (COCOK DENGAN DOKTER)' if is_subject_correct else '❌ BERBEDA DENGAN DOKTER'}")
    print("=" * 65)
    print(f"  [SUKSES] Data disimpan ke:")
    print(f"    1. Jendela : {WINDOWS_CSV_PATH}")
    print(f"    2. Subjek  : {SUMMARY_CSV_PATH}\n")

# ==========================================
# LIHAT REKAPITULASI PENGUJIAN
# ==========================================
def view_summary_table():
    if not os.path.exists(SUMMARY_CSV_PATH):
        print("\n[INFO] Belum ada data pengujian yang tersimpan.")
        return

    import pandas as pd
    df = pd.read_csv(SUMMARY_CSV_PATH)
    if df.empty:
        print("\n[INFO] File ringkasan masih kosong.")
        return

    print("\n" + "=" * 105)
    print("  REKAPITULASI HASIL PENGUJIAN DEPLOYMENT ESP32 (LEVEL SUBJEK)")
    print("=" * 105)
    print(f"{'No':<3} | {'Waktu':<19} | {'Subjek':<8} | {'Acuan':<15} | {'Prediksi Alat':<15} | {'Voting (N:H)':<13} | {'Hasil':<8}")
    print("-" * 105)
    for i, r in df.iterrows():
        bp_ref = f"{r['systolic_ref']}/{r['diastolic_ref']} ({r['ground_truth_label'][:3]})"
        pred_res = f"{r['final_subject_prediction']} ({r['confidence_pct']:.0f}%)"
        votes = f"{r['votes_normal']} : {r['votes_hipertensi']}"
        stat = "✅ COCOK" if r['is_subject_correct'] == 1 else "❌ SALAH"
        print(f"{i+1:>3} | {r['session_timestamp']:<19} | {r['subject_id']:<8} | {bp_ref:<15} | {pred_res:<15} | {votes:<13} | {stat}")
    print("=" * 105)

    # Hitung Akurasi Deploy Sementara
    acc = (df['is_subject_correct'].sum() / len(df)) * 100.0
    print(f"  Total Subjek Diuji : {len(df)} Orang")
    print(f"  Akurasi Deploy Riil: {acc:.1f}% ({df['is_subject_correct'].sum()}/{len(df)} Subjek Terdiagnosis Benar)\n")

# ==========================================
# MAIN MENU
# ==========================================
if __name__ == "__main__":
    init_csv_headers()
    while True:
        print("\n" + "=" * 55)
        print("  DEPLOY DATA COLLECTOR V4 - TINYML ESP32 LOGGER")
        print("=" * 55)
        print("  1. Mulai Sesi Pengujian Subjek Baru")
        print("  2. Lihat Rekapitulasi Pengujian Seluruh Subjek")
        print("  3. Keluar")
        choice = input("\n  Pilih menu (1/2/3): ").strip()

        if choice == '1':
            run_deployment_session()
        elif choice == '2':
            view_summary_table()
        elif choice == '3':
            print("  Sampai jumpa dan sukses dengan skripsinya!\n")
            break
        else:
            print("  [ERROR] Pilihan tidak valid.")
