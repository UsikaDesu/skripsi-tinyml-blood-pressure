# TinyML Blood Pressure Estimation System

A complete end-to-end system for collecting physiological data, training a neural network, and deploying it to ESP32 for real-time blood pressure estimation using TensorFlow Lite Micro.

## ⚠️ Medical Disclaimer

**This project is for educational and research purposes only.** TinyML-based blood pressure estimation is not a substitute for medical-grade devices. Results should not be used for medical diagnosis or treatment decisions. Always consult healthcare professionals and use FDA-approved devices for medical measurements.

## 📋 Hardware Requirements

- **ESP32 Development Board** (any variant with I2C support)
- **MAX30102 PPG Sensor Module**
  - Heart rate and SpO2 sensor
  - Available from SparkFun, Adafruit, or generic Chinese modules
- **Reference Blood Pressure Monitor** (for collecting training data)
  - Any digital BP monitor (arm cuff type recommended)
- **USB Cable** (for ESP32 programming and serial communication)
- **Jumper Wires** (for connecting MAX30102 to ESP32)

### Wiring Diagram

```
MAX30102    →    ESP32
────────────────────────
VCC (3.3V)  →    3.3V
GND         →    GND
SDA         →    GPIO21
SCL         →    GPIO22
INT         →    GPIO19 (optional)
```

## 💻 Software Requirements

### Arduino IDE Setup
1. Install Arduino IDE (1.8.x or 2.x)
2. Install ESP32 board support:
   - Add to Board Manager URLs: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Install "ESP32 by Espressif Systems"

3. Install required libraries (via Library Manager):
   - `SparkFun MAX3010x Pulse and Proximity Sensor`
   - `TensorFlowLite_ESP32`

### Python Setup
```bash
# Install Python dependencies
pip install -r requirements.txt
```

Required Python packages:
- `pyserial` - Serial communication with ESP32
- `numpy` - Numerical operations
- `pandas` - Data manipulation
- `matplotlib` - Visualization
- `scikit-learn` - Data preprocessing
- `tensorflow` - Model training and conversion

## 🚀 Complete Workflow (Step by Step)

### Phase 1: Data Collection

#### Step 1.1: Upload Data Collector Firmware
1. Open `esp32_data_collector.ino` in Arduino IDE
2. Select your ESP32 board (Tools → Board)
3. Select the correct COM port (Tools → Port)
4. Click Upload (→)
5. Open Serial Monitor (115200 baud) to verify it's working

#### Step 1.2: Collect Training Data
```bash
python data_logger.py
```

**Instructions:**
- The script will auto-detect your ESP32's COM port
- Place finger firmly on MAX30102 sensor (don't press too hard)
- Wait for stable readings (`valid=1`)
- When prompted, measure your BP with the reference device
- Enter the systolic and diastolic values
- Add notes (e.g., "resting", "after walking", "post-exercise")

**Tips for good data:**
- Collect at least **50-100 samples** for decent accuracy
- Vary your physical state:
  - Resting (sitting calmly for 5+ minutes)
  - After light activity (walking around)
  - After moderate activity (climbing stairs)
  - Different times of day
- Keep finger placement consistent
- Avoid motion during measurement

Data is saved in `training_data/bp_data_TIMESTAMP.csv`

### Phase 2: Model Training

#### Step 2.1: Preprocess Data
```bash
python preprocess_data.py
```

This script:
- Loads all CSV files from `training_data/`
- Removes invalid readings
- Filters outliers
- Extracts features
- Normalizes data
- Creates train/test split
- Saves to `processed_data.pkl`

#### Step 2.2: Train Neural Network
```bash
python train_model.py
```

Training process:
- Creates a small neural network (optimized for ESP32)
- Trains for up to 200 epochs with early stopping
- Evaluates on test set
- Saves model to `bp_model.h5`
- Generates `training_history.png` (check this to verify convergence!)

**Expected Performance:**
- MAE: 5-15 mmHg (depends on data quality and quantity)
- Better results with more diverse training data

#### Step 2.3: Convert to TensorFlow Lite
```bash
python convert_to_tflite.py
```

Conversion steps:
- Converts Keras model to TFLite format
- Applies int8 quantization (reduces size by ~75%)
- Validates converted model
- Generates `model_data.h` C header file
- This header contains your model as a byte array

#### Step 2.4: Generate Normalization Parameters
```bash
python generate_normalization.py
```

This outputs code to paste into the ESP32 firmware.
**IMPORTANT:** You must update the normalization parameters in `esp32_bp_inference.ino`!

### Phase 3: Deployment

#### Step 3.1: Update Inference Firmware
1. Open `esp32_bp_inference.ino` in Arduino IDE
2. Replace normalization parameters (lines 34-35) with values from `generate_normalization.py`
3. Verify `model_data.h` is in the same folder as the `.ino` file

#### Step 3.2: Upload to ESP32
1. Select Board and Port in Arduino IDE
2. Click Upload
3. Open Serial Monitor (115200 baud)

#### Step 3.3: Test Real-time Inference
- Place finger on sensor
- Wait a few seconds for data collection
- BP estimate will be displayed every 2 seconds
- Also shows heart rate, SpO2, and inference time

### Phase 4: Calibration & Validation

**Personal Calibration (Recommended):**

Due to individual variation, the model works best when calibrated per person:

1. Take 5-10 reference BP measurements with your digital monitor
2. At the same time, note the ESP32 estimates
3. Calculate the average offset
4. Apply correction in code or mentally

**Example:**
```
Reference: 120/80, ESP32: 115/75 → Offset: +5/+5
Reference: 135/85, ESP32: 130/82 → Offset: +5/+3
Average offset: +5/+4 mmHg
```

## 📊 File Structure

```
koding/
├── esp32_data_collector.ino      # Data collection firmware
├── data_logger.py                 # PC-side data collection
├── preprocess_data.py             # Data preprocessing
├── train_model.py                 # Model training
├── convert_to_tflite.py          # TFLite conversion
├── generate_normalization.py     # Helper for normalization params
├── esp32_bp_inference.ino        # Inference firmware
├── model_data.h                  # Generated TFLite model (auto-generated)
├── requirements.txt              # Python dependencies
├── README.md                     # This file
└── training_data/                # Collected data (created automatically)
    └── bp_data_*.csv
```

## 🔧 Troubleshooting

### ESP32 Issues

**"MAX30102 not found!"**
- Check wiring (especially SDA/SCL)
- Verify sensor has power (3.3V, not 5V!)
- Try a different I2C address scanner sketch

**COM port not detected**
- Install CP2102 or CH340 USB drivers
- Try different USB cable (needs data lines)
- Check Device Manager (Windows)

**Model header file missing**
- Make sure to run `convert_to_tflite.py` first
- Copy `model_data.h` to same folder as `.ino`

### Data Collection Issues

**All readings show "Invalid"**
- Finger not making good contact
- Finger too cold (warm it up)
- Pressing too hard (cuts off circulation)
- Sensor faulty

**Unstable heart rate values**
- Keep finger still
- Adjust pressure slightly
- Wait 10-15 seconds for stabilization

### Model Training Issues

**"Not enough samples for training"**
- Need at least 20-30 labeled samples
- Collect more data using `data_logger.py`

**Poor accuracy (MAE > 20 mmHg)**
- Collect more diverse training data
- Ensure reference BP measurements are accurate
- Check for labeling errors in CSV files
- Try collecting data at different times/states

**Model doesn't converge (loss not decreasing)**
- Check training_history.png
- May need more data
- Verify data isn't corrupted

## 📈 Improving Accuracy

1. **Collect More Data**: 100+ samples is better than 30
2. **Diverse Conditions**: Different BP states give better generalization
3. **Consistent Measurement**: Same finger, same pressure, same position
4. **Quality Reference**: Use a reliable BP monitor
5. **Personal Calibration**: Tune the model to your physiology
6. **Feature Engineering**: Advanced users can add more features (HRV metrics, waveform analysis)

## 🎓 Understanding the Model

**Input Features (6):**
- IR LED mean intensity
- Red LED mean intensity
- IR LED standard deviation
- Red LED standard deviation
- Heart rate (BPM)
- SpO2 (%)

**Model Architecture:**
```
Input (6) → Dense(16) → Dense(12) → Dense(8) → Output(2)
                ↓           ↓          ↓
              ReLU       ReLU       ReLU
```

**Outputs:**
- Systolic BP (mmHg)
- Diastolic BP (mmHg)

**Why this works:**
- PPG waveform characteristics correlate with blood pressure
- Heart rate variability provides additional information
- Machine learning finds patterns in these relationships

## 🔬 Advanced Usage

### Custom Features
Edit `preprocess_data.py` to add your own features:
- HRV time-domain metrics (SDNN, RMSSD)
- Frequency-domain features (LF/HF ratio)
- Waveform shape characteristics

### Hyperparameter Tuning
Edit `train_model.py`:
- Change network architecture (more/fewer layers)
- Adjust learning rate
- Try different batch sizes

### Real-time Display
Add an OLED display to ESP32:
- SSD1306 128x64 OLED recommended
- Use U8g2 library
- Display BP estimate, HR, timestamp

## 📚 References & Further Reading

- [TensorFlow Lite for Microcontrollers](https://www.tensorflow.org/lite/microcontrollers)
- [MAX30102 Datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf)
- [PPG-based Blood Pressure Estimation](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC6359303/)
- [Edge Impulse TinyML Resources](https://www.edgeimpulse.com/)

## 📝 License

This project is provided as-is for educational purposes. Use at your own risk.

## 🤝 Contributing

Found a bug? Have an improvement? Feel free to modify and extend this codebase!

---

**Built with ❤️ for TinyML enthusiasts**
