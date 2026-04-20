import serial
import serial.tools.list_ports
import time
import os
import csv
from datetime import datetime

# ==========================================
# CONFIGURATION
# ==========================================
BAUD_RATE = 115200
RECORDING_TIME_SECONDS = 25  # Waktu menyesuaikan alat tensi (sekitar 35 detik)
DATASET_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset')

if not os.path.exists(DATASET_DIR):
    os.makedirs(DATASET_DIR)

def find_esp32_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if "CP210x" in port.description or "CH340" in port.description or "Serial" in port.description:
            return port.device
    
    selected_port = 'COM3' if os.name == 'nt' else '/dev/ttyUSB0'
    print(f"Warning: ESP32 auto-detect failed. Defaulting to {selected_port}")
    return selected_port

def record_raw_session():
    port = find_esp32_port()
    print(f"Connecting to ESP32 on port {port}...")
    
    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=1)
        # Flush initial junk
        ser.reset_input_buffer()
        print("\n" + "="*50)
        print(f"📡 CONNECTED! Please stay still.")
        print(f"⏱️ Recording {RECORDING_TIME_SECONDS} seconds of raw PPG data...")
        print("="*50 + "\n")
        
        start_time = time.time()
        raw_rows = []
        
        while (time.time() - start_time) < RECORDING_TIME_SECONDS:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                # Cek jika format T,IR,RED cocok
                parts = line.split(',')
                if len(parts) == 3:
                    try:
                        timestamp = int(parts[0])
                        ir_raw = int(parts[1])
                        red_raw = int(parts[2])
                        raw_rows.append([timestamp, ir_raw, red_raw])
                    except ValueError:
                        pass # Ignore corrupted lines
            
            # Print progress bar
            elapsed = time.time() - start_time
            progress = int((elapsed / RECORDING_TIME_SECONDS) * 20)
            print(f"\rRecording: [{'█' * progress}{'.' * (20 - progress)}] {elapsed:.1f}s", end="")
            
        print("\n\n✅ Recording Complete!")
        ser.close()
        
        if len(raw_rows) <= 100:
            print("ERROR: Too few samples recorded. Was finger on the sensor? (Need >100 samples due to warmup discard)")
            return
            
        print(f"Captured {len(raw_rows)} raw samples (approx. {len(raw_rows)/RECORDING_TIME_SECONDS:.1f} Hz)")
        print("Discarding the first 100 samples for sensor warmup stability...")
        raw_rows = raw_rows[100:]
        print(f"Remaining valid samples: {len(raw_rows)}")
        
        # --- LABELING (GROUND TRUTH INJECTION) ---
        print("\n" + "="*50)
        print("🩺 MANUAL LABELING REQUIRED")
        print("What was your blood pressure reading from the medical monitor?")
        print("Format: Sistolik/Diastolik (example: 120/80)")
        print("Type X to discard this test.")
        print("="*50)
        
        while True:
            label_in = input("\nEnter BP (or 'X' to discard): ").strip()
            
            if label_in.upper() == 'X':
                print("Action discarded. Perekaman data dibatalkan dan tidak disimpan.")
                return
                
            try:
                if '/' not in label_in:
                    raise ValueError("Tidak ada pemisah '/'")
                sys_str, dia_str = label_in.split('/')
                systolic = int(sys_str)
                diastolic = int(dia_str)
                break  # Input valid, keluar dari loop
                
            except ValueError:
                print("❌ Format salah! Harap masukkan dengan format Sistolik/Diastolik (contoh: 120/80).")
                print("💡 Atau ketik 'X' jika Anda benar-benar batal menyimpan datanya.")
            
        filename = f"raw_data_{datetime.now().strftime('%Y%m%d_%H%M%S')}_bp{systolic}_{diastolic}.csv"
        filepath = os.path.join(DATASET_DIR, filename)
        
        # Tulis ke file. Karena raw file bisa sangat panjang, kita akan menyimpan systolik dan diastolik di setiap baris.
        with open(filepath, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(["time_ms", "ir_raw", "red_raw", "systolic_label", "diastolic_label"])
            for row in raw_rows:
                writer.writerow([row[0], row[1], row[2], systolic, diastolic])
                
        print(f"\n🎉 SUCCESS: Data securely saved to {filepath}")
        
    except FileNotFoundError:
        print(f"ERROR: Cannot connect to {port}. Is the ESP32 plugged in?")
    except KeyboardInterrupt:
        print("\nInterrupted by user.")

if __name__ == "__main__":
    while True:
        print("\n[MAIN MENU]")
        print("1. Start New Recording Session")
        print("2. Exit")
        choice = input("Select: ").strip()
        
        if choice == '1':
            record_raw_session()
        elif choice == '2':
            break
        else:
            print("Invalid choice.")
