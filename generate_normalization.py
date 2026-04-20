"""
Generate normalization parameters from training data
Creates code snippet to paste into esp32_bp_inference.ino
"""

import pickle
import os

PROCESSED_DATA_FILE = 'processed_data.pkl'

def generate_normalization_code():
    """Generate normalization parameters for ESP32"""
    
    if not os.path.exists(PROCESSED_DATA_FILE):
        print("ERROR: processed_data.pkl not found!")
        print("Please run preprocess_data.py and train_model.py first!")
        return
    
    print("=" * 70)
    print("Generating Normalization Parameters")
    print("=" * 70)
    
    # Load scaler
    with open(PROCESSED_DATA_FILE, 'rb') as f:
        data = pickle.load(f)
    
    scaler = data['scaler']
    feature_names = data['feature_names']
    
    mean = scaler.mean_
    std = scaler.scale_
    
    print("\nFeature normalization parameters:")
    print("\nFeature Name       | Mean        | Std Dev")
    print("-" * 50)
    for i, name in enumerate(feature_names):
        print(f"{name:18s} | {mean[i]:10.2f} | {std[i]:10.2f}")
    
    print("\n" + "=" * 70)
    print("CODE TO PASTE INTO esp32_bp_inference.ino")
    print("=" * 70)
    print("\nReplace these lines (around line 34-35):\n")
    
    print("float feature_mean[6] = {", end="")
    print(", ".join([f"{m:.2f}" for m in mean]), end="")
    print("};")
    
    print("float feature_std[6] = {", end="")
    print(", ".join([f"{s:.2f}" for s in std]), end="")
    print("};")
    
    print("\n" + "=" * 70)
    print("\nIMPORTANT:")
    print("1. Copy the two lines above")
    print("2. Open esp32_bp_inference.ino in Arduino IDE")
    print("3. Find lines 34-35 (the placeholder values)")
    print("4. Replace them with the values above")
    print("5. Upload to ESP32")
    print("=" * 70)

if __name__ == "__main__":
    generate_normalization_code()
