"""
Blood Pressure Data Logger - BULK SAVER VERSION
Collects sensor data from ESP32, waits for 30 stable samples, 
and saves ALL 30 of them (instead of just 1) into the CSV file for faster dataset generation.
"""

import serial
import csv
import time
from datetime import datetime
import os

# Configuration
SERIAL_PORT = 'COM6'  # Change this to your ESP32's port (COM3, COM4, etc. on Windows)
BAUD_RATE = 115200
DATA_FOLDER = 'training_data'

def find_esp32_port():
    """Try to find ESP32 automatically"""
    import serial.tools.list_ports
    ports = serial.tools.list_ports.comports()
    
    print("\nAvailable serial ports:")
    for i, port in enumerate(ports):
        print(f"  {i+1}. {port.device} - {port.description}")
    
    if not ports:
        print("No serial ports found!")
        return None
    
    choice = input(f"\nSelect port number (1-{len(ports)}) or press Enter for {SERIAL_PORT}: ").strip()
    
    if choice:
        try:
            return ports[int(choice)-1].device
        except:
            print("Invalid selection, using default")
    
    return SERIAL_PORT

def collect_data_session():
    """Main data collection function"""
    
    print("=" * 60)
    print("BP Training Data Collector (BULK 30x MULTIPLIER)")
    print("=" * 60)
    
    # Create data folder
    if not os.path.exists(DATA_FOLDER):
        os.makedirs(DATA_FOLDER)
        print(f"Created data folder: {DATA_FOLDER}")
    
    # Find serial port
    port = find_esp32_port()
    
    # Connect to ESP32
    print(f"\nConnecting to ESP32 on {port}...")
    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=1)
        time.sleep(2)  # Wait for connection
        print("✓ Connected!")
    except Exception as e:
        print(f"✗ Error connecting: {e}")
        return
    
    # Clear buffer
    ser.reset_input_buffer()
    
    # Wait for header
    print("\nWaiting for sensor data...")
    header_found = False
    while not header_found:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if line.startswith('timestamp'):
            print(f"✓ Header received: {line}")
            header_found = True
    
    print("\n" + "=" * 60)
    print("BULK DATA COLLECTION INSTRUCTIONS")
    print("=" * 60)
    print("1. Place finger on MAX30102 sensor")
    print("2. Wait for stable readings (valid=1)")
    print("3. Take BP measurement with your reference device")
    print("4. Enter the BP values when prompted")
    print("5. ⚡ This version will save ALL 30 consecutive samples instantly!")
    print("\nPress Ctrl+C to stop collection\n")
    print("=" * 60)
    
    session_id = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = os.path.join(DATA_FOLDER, f'bp_data_{session_id}.csv')
    
    total_rows_saved = 0
    sample_count = 0
    
    try:
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            # Write header with BP labels
            writer.writerow(['timestamp', 'ir_mean', 'red_mean', 'ir_std', 'red_std', 
                           'heart_rate', 'spo2', 'valid', 'systolic_bp', 'diastolic_bp', 'notes'])
            
            print("\nCollecting data... (showing every 10th sample)")
            print("-" * 60)
            
            stable_count = 0
            last_prompt_time = time.time()
            sample_buffer = []  # Array to precisely hold the 30 samples
            
            while True:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line and not line.startswith('timestamp'):
                    try:
                        # Parse sensor data
                        parts = line.split(',')
                        if len(parts) >= 8:
                            valid = parts[7] == '1'
                            
                            # Count continuous readings and fill buffer (no streak required)
                            sample_buffer.append(parts)
                            stable_count = len(sample_buffer)

                            # Display EVERY sample in real-time
                            sample_count += 1
                            hr = float(parts[5])
                            spo2 = float(parts[6])
                            status = "✓ VALID" if valid else "✗ Invalid"
                            buffer_info = f" (Buffer: {stable_count}/30)"
                            
                            print(f"Sample {sample_count:4d} | HR: {hr:6.1f} bpm | SpO2: {spo2:5.1f}% | {status}{buffer_info}")
                            
                            # Prompt for BP reading after 30 samples accumulated
                            if stable_count >= 30 and (time.time() - last_prompt_time) > 10:
                                print("\n" + "!" * 60)
                                print("📊 30 SAMPLES COLLECTED - READY FOR BP!")
                                print(f"⚡ I have stored all {len(sample_buffer)} continuous samples for you.")
                                print("!" * 60)
                                
                                # Get BP measurement
                                systolic = input("Enter SYSTOLIC BP (or 's' to skip): ").strip()
                                if systolic.lower() == 's':
                                    print("Skipped reading.")
                                    stable_count = 0
                                    sample_buffer.clear()
                                    last_prompt_time = time.time()
                                    print("-" * 60)
                                    ser.reset_input_buffer()
                                    continue
                                
                                diastolic = input("Enter DIASTOLIC BP: ").strip()
                                notes = input("Notes (resting/active/post-exercise/etc): ").strip()
                                
                                try:
                                    systolic = int(systolic)
                                    diastolic = int(diastolic)
                                    
                                    # Validate BP values
                                    if 70 <= systolic <= 200 and 40 <= diastolic <= 130:
                                        
                                        # BULK WRITE! Iterate over everything in the buffer and save them all.
                                        written_this_round = 0
                                        for buffered_parts in sample_buffer:
                                            writer.writerow(buffered_parts + [systolic, diastolic, notes])
                                            written_this_round += 1
                                            
                                        csvfile.flush()
                                        total_rows_saved += written_this_round
                                        print(f"✓ Saved {written_this_round} rows successfully! (BP: {systolic}/{diastolic} mmHg)")
                                        print(f"✓ Total total dataset size: {total_rows_saved} rows")
                                    else:
                                        print("✗ BP values out of range, skipped")
                                        
                                except ValueError:
                                    print("✗ Invalid BP values, skipped")
                                
                                # Reset everything to prepare for the next 30-sample period
                                stable_count = 0
                                sample_buffer.clear()
                                last_prompt_time = time.time()
                                print("-" * 60)
                                ser.reset_input_buffer()  # Purge the serial queue from when you were typing
                            
                    except Exception as e:
                        print(f"Error parsing line: {e}")
                        continue
                
    except KeyboardInterrupt:
        print("\n\n" + "=" * 60)
        print("Collection stopped by user")
    except Exception as e:
        print(f"\nError during collection: {e}")
    finally:
        ser.close()
        print(f"\n✓ Data saved to: {filename}")
        print(f"✓ Total golden samples explicitly saved: {total_rows_saved}")
        print("=" * 60)

if __name__ == "__main__":
    collect_data_session()
