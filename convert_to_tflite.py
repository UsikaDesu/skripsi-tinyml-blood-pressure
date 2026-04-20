"""
Convert trained model to TensorFlow Lite for ESP32 deployment
Generates C header file for use with TFLite Micro
"""

import tensorflow as tf
import numpy as np
import pickle
import os

MODEL_FILE = 'bp_model.h5'
TFLITE_MODEL_FILE = 'bp_model.tflite'
TFLITE_QUANT_MODEL_FILE = 'bp_model_quantized.tflite'
HEADER_FILE = 'model_data.h'
PROCESSED_DATA_FILE = 'processed_data.pkl'

def convert_to_tflite(model_path):
    """Convert Keras model to TFLite format"""
    print("=" * 70)
    print("Converting model to TensorFlow Lite")
    print("=" * 70)
    
    if not os.path.exists(model_path):
        print(f"ERROR: Model file not found: {model_path}")
        print("Please run train_model.py first!")
        return None
    
    # Load model
    print(f"\nLoading model from {model_path}...")
    model = tf.keras.models.load_model(model_path)
    print("✓ Model loaded")
    
    # Convert to TFLite (float32)
    print("\nConverting to TFLite (float32)...")
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    tflite_model = converter.convert()
    
    # Save float model
    with open(TFLITE_MODEL_FILE, 'wb') as f:
        f.write(tflite_model)
    
    print(f"✓ Float32 model saved: {TFLITE_MODEL_FILE}")
    print(f"  Size: {len(tflite_model) / 1024:.2f} KB")
    
    return model, tflite_model

def quantize_model(model):
    """Apply post-training quantization to reduce model size"""
    print("\nApplying post-training quantization (int8)...")
    
    # Load representative dataset for calibration
    if os.path.exists(PROCESSED_DATA_FILE):
        with open(PROCESSED_DATA_FILE, 'rb') as f:
            data = pickle.load(f)
        X_train = data['X_train']
        
        def representative_dataset():
            for i in range(min(100, len(X_train))):
                yield [X_train[i:i+1].astype(np.float32)]
        
        # Convert with quantization
        converter = tf.lite.TFLiteConverter.from_keras_model(model)
        converter.optimizations = [tf.lite.Optimize.DEFAULT]
        converter.representative_dataset = representative_dataset
        converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
        converter.inference_input_type = tf.float32
        converter.inference_output_type = tf.float32
        
        tflite_quant_model = converter.convert()
        
        # Save quantized model
        with open(TFLITE_QUANT_MODEL_FILE, 'wb') as f:
            f.write(tflite_quant_model)
        
        print(f"✓ Quantized model saved: {TFLITE_QUANT_MODEL_FILE}")
        print(f"  Size: {len(tflite_quant_model) / 1024:.2f} KB")
        print(f"  Compression: {len(tflite_quant_model) / len(tflite_model) * 100:.1f}%")
        
        return tflite_quant_model
    else:
        print("⚠ Warning: No training data found, skipping quantization")
        return None

def validate_tflite_model(tflite_model):
    """Validate TFLite model accuracy"""
    print("\nValidating TFLite model...")
    
    if not os.path.exists(PROCESSED_DATA_FILE):
        print("⚠ Warning: No test data available for validation")
        return
    
    # Load test data
    with open(PROCESSED_DATA_FILE, 'rb') as f:
        data = pickle.load(f)
    X_test = data['X_test'][:10]  # Use first 10 samples
    y_test = data['y_test'][:10]
    
    # Load original model for comparison
    model = tf.keras.models.load_model(MODEL_FILE)
    
    # Get original predictions
    y_pred_original = model.predict(X_test, verbose=0)
    
    # Create TFLite interpreter
    interpreter = tf.lite.Interpreter(model_content=tflite_model)
    interpreter.allocate_tensors()
    
    # Get input/output details
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    
    # Run inference
    y_pred_tflite = []
    for i in range(len(X_test)):
        interpreter.set_tensor(input_details[0]['index'], X_test[i:i+1].astype(np.float32))
        interpreter.invoke()
        output = interpreter.get_tensor(output_details[0]['index'])
        y_pred_tflite.append(output[0])
    
    y_pred_tflite = np.array(y_pred_tflite)
    
    # Compare predictions
    print("\nComparison (first 5 samples):")
    print("  Actual    | Original  | TFLite    | Diff")
    print("-" * 55)
    for i in range(min(5, len(y_test))):
        actual = y_test[i]
        orig = y_pred_original[i]
        tfl = y_pred_tflite[i]
        diff_sys = abs(orig[0] - tfl[0])
        diff_dia = abs(orig[1] - tfl[1])
        print(f"  {actual[0]:.0f}/{actual[1]:.0f}  | {orig[0]:.0f}/{orig[1]:.0f}  | {tfl[0]:.0f}/{tfl[1]:.0f}  | {diff_sys:.1f}/{diff_dia:.1f}")
    
    # Calculate mean difference
    diff = np.abs(y_pred_original - y_pred_tflite).mean()
    print(f"\nMean absolute difference: {diff:.4f} mmHg")
    
    if diff < 1.0:
        print("✓ TFLite model validated successfully!")
    else:
        print("⚠ Warning: TFLite model has significant differences from original")

def generate_c_header(tflite_model, header_path):
    """Generate C header file for Arduino"""
    print(f"\nGenerating C header file: {header_path}...")
    
    # Convert model to C array
    model_bytes = bytes(tflite_model)
    model_size = len(model_bytes)
    
    # Create header content
    header = f"""// Auto-generated TensorFlow Lite model
// Generated on {np.datetime64('now')}
// Model size: {model_size} bytes ({model_size/1024:.2f} KB)

#ifndef MODEL_DATA_H
#define MODEL_DATA_H

const unsigned int model_len = {model_size};
const unsigned char model_data[] = {{
"""
    
    # Add model bytes (16 per line)
    for i in range(0, model_size, 16):
        chunk = model_bytes[i:i+16]
        hex_values = ', '.join([f'0x{b:02x}' for b in chunk])
        header += f"  {hex_values}"
        if i + 16 < model_size:
            header += ","
        header += "\n"
    
    header += "};\n\n#endif  // MODEL_DATA_H\n"
    
    # Write to file
    with open(header_path, 'w') as f:
        f.write(header)
    
    print(f"✓ Header file generated: {header_path}")
    print(f"  Model size: {model_size} bytes ({model_size/1024:.2f} KB)")

def main():
    """Main conversion pipeline"""
    
    # Convert to TFLite
    model, tflite_model = convert_to_tflite(MODEL_FILE)
    if tflite_model is None:
        return False
    
    # Quantize model
    tflite_quant_model = quantize_model(model)
    
    # Use quantized model if available, otherwise float
    final_model = tflite_quant_model if tflite_quant_model is not None else tflite_model
    
    # Validate model
    validate_tflite_model(final_model)
    
    # Generate C header
    generate_c_header(final_model, HEADER_FILE)
    
    print("\n" + "=" * 70)
    print("✓ Conversion complete!")
    print("=" * 70)
    print("\nNext steps:")
    print("  1. Copy model_data.h to your Arduino project folder")
    print("  2. Upload esp32_bp_inference.ino to your ESP32")
    print("  3. Test real-time blood pressure estimation!")
    print("=" * 70)
    
    return True

if __name__ == "__main__":
    success = main()
    if not success:
        exit(1)
