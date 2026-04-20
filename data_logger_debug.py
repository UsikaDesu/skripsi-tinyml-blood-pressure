"""
Blood Pressure Data Logger - DEBUG VERSION
Automatically connects and continuously prints every single frame of data
so you can debug the sampling rate, SpO2, and Heart Rate behavior in real-time.
This script does NOT require you to enter BP or save to a CSV.
"""

import serial
import time
from datetime import datetime

# Configuration
SERIAL_PORT = 'COM6'  # Change this to your ESP32's port if needed
BAUD_RATE = 115200

def find_esp32_port():
    """Try to find ESP32 automatically"""
    import serial.tools.list_ports
    ports = serial.tools.list_ports.comports()
    
    if not ports:
        return None
        
    print("\nAvailable serial ports:")
    for i, port in enumerate(ports):
        print(f"  {i+1}. {port.device} - {port.description}")
        
    choice = input(f"\nSelect port number (1-{len(ports)}) or press Enter for {SERIAL_PORT}: ").strip()
    
    if choice:
        try:
            return ports[int(choice)-1].device
        except:
            pass
            
    return SERIAL_PORT

def run_debug_logger():
    print("=" * 80)
    print(" 🛠️  ESP32 SENSOR DEBUG LOGGER  🛠️ ")
    print("=" * 80)
    
    port = find_esp32_port()
    print(f"\nConnecting to ESP32 on {port} at {BAUD_RATE} baud...")
    
    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=1)
        time.sleep(2)
        print("✓ Connected successfully!\n")
    except Exception as e:
        print(f"✗ Error connecting: {e}")
        return
        
    ser.reset_input_buffer()
    
    print("Waiting for header from ESP32...")
    header_found = False
    while not header_found:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if line.startswith('timestamp'):
            header_found = True
            
    print("✓ Header detected!")
    print("\n" + "=" * 80)
    print("STREAMING REAL-TIME SENSOR DATA (Press Ctrl+C to exit)")
    print("Notice the time gap between each line!")
    print("=" * 80 + "\n")
    
    print(f"{'Time':<10} | {'Gap(s)':<8} | {'HR (BPM)':<10} | {'SpO2 (%)':<10} | {'IR Mean':<10} | {'Red Mean':<10} | {'Status'}")
    print("-" * 80)
    
    last_receive_time = time.time()
    sample_count = 0
    
    try:
        while True:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            
            if line and not line.startswith('timestamp'):
                current_time = time.time()
                time_gap = current_time - last_receive_time
                last_receive_time = current_time
                
                try:
                    parts = line.split(',')
                    if len(parts) >= 8:
                        ir_mean = float(parts[1])
                        red_mean = float(parts[2])
                        hr = float(parts[5])
                        spo2 = float(parts[6])
                        valid = parts[7] == '1'
                        
                        status = "✓ VALID" if valid else "✗ Invalid"
                        if hr == 0.0:
                            status += " (No Heartbeat Lock)"
                            
                        # Print every single row formatted cleanly
                        current_clock = datetime.now().strftime("%H:%M:%S")
                        
                        print(f"{current_clock:<10} | {time_gap:<8.2f} | {hr:<10.1f} | {spo2:<10.1f} | {ir_mean:<10.0f} | {red_mean:<10.0f} | {status}")
                        sample_count += 1
                        
                except Exception as e:
                    print(f"Error parsing line: {line} -> {e}")
                    
    except KeyboardInterrupt:
        print("\n" + "=" * 80)
        print(f"Debug stopped by user. Received {sample_count} total frames.")
        print("=" * 80)
    finally:
        ser.close()

if __name__ == "__main__":
    run_debug_logger()
