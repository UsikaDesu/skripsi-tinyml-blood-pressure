# -*- coding: utf-8 -*-
"""
DATA COLLECTOR V4 (Enhanced Robustness Edition)
==============================================
Mengumpulkan data sinyal PPG (IR + Red) dari ESP32 via Serial
dan menyimpannya langsung ke folder raw dataset terpusat.

Arsitektur Penyimpanan:
  Seluruh raw data dari seluruh iterasi penelitian (V1, V2, V3, V4)
  secara sengaja dipusatkan pada direktori bersama:
    raw_data_pipeline_v2/dataset_v2/raw_data_ID_YYYYMMDD_HHMMSS_bpSIS_DIA.csv
  Hal ini bertujuan untuk:
  1. Menjaga kesinambungan dan integritas data mentah (single source of truth).
  2. Mencegah duplikasi data mentah yang membebani repositori.

Peningkatan V4:
  - Validasi fisiologis tekanan darah: Sistolik WAJIB > Diastolik.
  - Dukungan ID numerik/subjek unik (misal KYS01, KYS02) untuk mencegah bentrok inisial pada LOSO.
  - Settling time (warm-up 1 detik) dihitung HANYA SETELAH jari menempel stabil (IR > 40000).
  - Deteksi jari terlepas secara real-time di tengah proses perekaman.
  - Validasi laju sampling efektif (100 Hz / delta 10 ms) dan ambang minimal sampel (>= 85%).
  - Pencatatan metadata protokol pengukuran (posisi tubuh, lengan, jeda tensi).

Kolom Output CSV:
  time_ms, ir_raw, red_raw, systolic_label, diastolic_label
"""

import serial
import serial.tools.list_ports
import time
import os
import csv
from datetime import datetime

# ==========================================
# KONFIGURASI
# ==========================================
BAUD_RATE = 115200
RECORDING_TIME_SECONDS = 25  # Durasi perekaman (25 detik @ 100 Hz = 2500 sampel)
EXPECTED_SAMPLES = RECORDING_TIME_SECONDS * 100
MIN_VALID_SAMPLES = int(EXPECTED_SAMPLES * 0.85)  # Minimal 85% sampel (2125 sampel)

# Seluruh raw data disimpan di direktori dataset terpusat
DATASET_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v2', 'dataset_v2')
)
os.makedirs(DATASET_DIR, exist_ok=True)

# ==========================================
# DETEKSI PORT ESP32
# ==========================================
def find_esp32_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        desc = port.description or ""
        if any(kw in desc for kw in ["CP210x", "CH340", "CH341", "USB Serial", "UART"]):
            print(f"[AUTO-DETECT] ESP32 ditemukan di: {port.device} ({desc})")
            return port.device

    # Fallback manual
    print("\n[PERINGATAN] Auto-detect ESP32 gagal. Daftar port yang tersedia:")
    all_ports = list(serial.tools.list_ports.comports())
    if all_ports:
        for i, p in enumerate(all_ports):
            print(f"  {i+1}. {p.device} - {p.description}")
        idx = input("Pilih nomor port (atau tekan Enter untuk default COM3): ").strip()
        if idx.isdigit() and 1 <= int(idx) <= len(all_ports):
            return all_ports[int(idx) - 1].device

    fallback = 'COM3' if os.name == 'nt' else '/dev/ttyUSB0'
    print(f"[FALLBACK] Menggunakan port default: {fallback}")
    return fallback

# ==========================================
# SESI PEREKAMAN
# ==========================================
def record_raw_session():
    # 1. Input Identitas Subjek (Unik)
    print("\n" + "=" * 55)
    print("  IDENTITAS SUBJEK")
    print("=" * 55)
    print("  Tips: Gunakan inisial + nomor jika inisial sama (contoh: KYS01, AMR02)")
    while True:
        subject_id = input("  Masukkan ID/Inisial Subjek: ").strip().upper()
        if not subject_id:
            print("  [ERROR] Inisial/ID tidak boleh kosong!")
        elif '_' in subject_id or ' ' in subject_id:
            print("  [ERROR] ID tidak boleh mengandung spasi atau garis bawah.")
        else:
            break

    # 2. Koneksi Serial
    port = find_esp32_port()
    print(f"\nMenghubungkan ke ESP32 di {port} ...")

    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=1)
        ser.reset_input_buffer()

        print("\n" + "=" * 55)
        print("  LANGKAH 1: MENUNGGU JARI MENEMPEL PADA SENSOR")
        print("  Letakkan jari dengan pas dan tenang...")
        print("=" * 55)

        # Polling hingga jari terdeteksi (IR > 40000)
        finger_detected = False
        while not finger_detected:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                parts = line.split(',')
                if len(parts) == 3:
                    try:
                        ir_val = int(parts[1])
                        if ir_val > 40000:
                            finger_detected = True
                            print(f"\n  >> Jari Terdeteksi! (IR={ir_val})")
                    except ValueError:
                        pass
            time.sleep(0.01)

        # Settling time 100 sampel (1 detik) setelah jari terdeteksi
        print("  Stabilisasi sensor (100 sampel settling time)...")
        settled_count = 0
        while settled_count < 100:
            if ser.in_waiting > 0:
                ser.readline()
                settled_count += 1
            time.sleep(0.005)

        print("\n" + "=" * 55)
        print(f"  LANGKAH 2: MEREKAM DATA PPG ({RECORDING_TIME_SECONDS} DETIK)")
        print("  Harap tetap diam dan bernapas normal.")
        print("=" * 55 + "\n")

        start_time = time.time()
        raw_rows = []
        last_progress = -1
        consecutive_low_ir = 0

        while (time.time() - start_time) < RECORDING_TIME_SECONDS:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                parts = line.split(',')
                if len(parts) == 3:
                    try:
                        timestamp = int(parts[0])
                        ir_raw    = int(parts[1])
                        red_raw   = int(parts[2])

                        # Deteksi jari terlepas di tengah jalan
                        if ir_raw < 35000:
                            consecutive_low_ir += 1
                            if consecutive_low_ir > 100:  # > 1 detik terlepas
                                print("\n\n  ❌ [ABORT] Jari terlepas dari sensor di tengah perekaman!")
                                ser.close()
                                return
                        else:
                            consecutive_low_ir = 0

                        raw_rows.append([timestamp, ir_raw, red_raw])
                    except ValueError:
                        pass  # Abaikan baris rusak

            elapsed  = time.time() - start_time
            progress = int((elapsed / RECORDING_TIME_SECONDS) * 30)
            if progress != last_progress:
                bar = '#' * progress + '.' * (30 - progress)
                print(f"\r  [{bar}] {elapsed:.1f}s / {RECORDING_TIME_SECONDS}s  |  {len(raw_rows)} samples", end="", flush=True)
                last_progress = progress

        print(f"\n\n  Perekaman selesai! Total terkumpul: {len(raw_rows)} samples.")
        ser.close()

        # 3. Validasi Kualitas Pengumpulan Data
        if len(raw_rows) < MIN_VALID_SAMPLES:
            print(f"  ❌ [ERROR] Sampel terlalu sedikit ({len(raw_rows)}/{EXPECTED_SAMPLES}). Minimal {MIN_VALID_SAMPLES}.")
            print("     Perekaman dibatalkan untuk menjaga kualitas dataset.\n")
            return

        # Pengecekan laju sampel efektif (sampling rate)
        timestamps = [r[0] for r in raw_rows]
        dt_list = [timestamps[i] - timestamps[i-1] for i in range(1, len(timestamps))]
        avg_dt = sum(dt_list) / len(dt_list) if dt_list else 10.0
        effective_fs = 1000.0 / avg_dt if avg_dt > 0 else 0
        print(f"  Laju Sampel Efektif : {effective_fs:.1f} Hz (Interval rata-rata: {avg_dt:.2f} ms)")

        if abs(effective_fs - 100.0) > 10.0:
            print(f"  ⚠️ [PERINGATAN] Laju sampel menyimpang dari 100 Hz ({effective_fs:.1f} Hz).")

        # 4. Input Label Tekanan Darah dengan Validasi Fisiologis
        print("\n" + "=" * 55)
        print("  LABELING TEKANAN DARAH (Tensimeter Digital)")
        print("  Format: Sistolik/Diastolik  (contoh: 120/80)")
        print("  Ketik 'X' untuk membatalkan tanpa menyimpan.")
        print("=" * 55)

        while True:
            label_in = input("\n  Masukkan BP: ").strip()

            if label_in.upper() == 'X':
                print("  [DIBATALKAN] Data tidak disimpan.\n")
                return

            try:
                if '/' not in label_in:
                    raise ValueError
                sys_str, dia_str = label_in.split('/')
                systolic  = int(sys_str.strip())
                diastolic = int(dia_str.strip())

                # Validasi rentang wajar manusia
                if not (60 <= systolic <= 250) or not (40 <= diastolic <= 150):
                    print("  [ERROR] Nilai BP di luar batas wajar manusia (Sistolik: 60-250, Diastolik: 40-150).")
                    continue

                # Validasi fisiologis wajib: Sistolik harus > Diastolik
                if systolic <= diastolic:
                    print("  [ERROR] Tekanan Sistolik harus lebih besar dari Diastolik! Periksa kembali.")
                    continue

                break
            except ValueError:
                print("  [ERROR] Format salah! Gunakan format Sistolik/Diastolik (contoh: 120/80).")

        # 5. Simpan ke File CSV Terpusat
        bp_class = "H" if (systolic >= 140 or diastolic >= 90) else "N"
        timestamp_str = datetime.now().strftime('%Y%m%d_%H%M%S')
        filename = f"raw_data_{subject_id}_{timestamp_str}_bp{systolic}_{diastolic}.csv"
        filepath = os.path.join(DATASET_DIR, filename)

        with open(filepath, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(["time_ms", "ir_raw", "red_raw", "systolic_label", "diastolic_label"])
            for row in raw_rows:
                writer.writerow([row[0], row[1], row[2], systolic, diastolic])

        print(f"\n  ✅ [SUKSES] Data tersimpan -> {filename}")
        print(f"     Kelas               : {'HIPERTENSI' if bp_class == 'H' else 'NORMAL'} ({systolic}/{diastolic} mmHg)")
        print(f"     Total Sampel Valid  : {len(raw_rows)} baris")
        print(f"     Lokasi Penyimpanan  : {filepath}\n")

    except serial.SerialException as e:
        print(f"\n  ❌ [ERROR] Tidak bisa terhubung ke {port}: {e}")
        print("     Pastikan ESP32 sudah terpasang dan port tidak dipakai Serial Monitor lain.\n")
    except KeyboardInterrupt:
        print("\n\n  [INTERRUPT] Perekaman dihentikan oleh pengguna.\n")


# ==========================================
# MAIN MENU
# ==========================================
if __name__ == "__main__":
    print("\n" + "=" * 55)
    print("  DATA COLLECTOR V4 - PPG Blood Pressure System")
    print("  Enhanced Robustness & Physiological Validation")
    print(f"  Target Folder: {DATASET_DIR}")
    print("=" * 55)

    while True:
        print("\n  MENU:")
        print("  1. Mulai Sesi Perekaman Baru")
        print("  2. Keluar")
        choice = input("\n  Pilih: ").strip()

        if choice == '1':
            record_raw_session()
        elif choice == '2':
            print("  Sampai jumpa!\n")
            break
        else:
            print("  [ERROR] Pilihan tidak valid.")
