# 🚀 Classification Quick Start Guide

**Follow these steps to get your Blood Pressure Classification system running!**
*(This guide is specifically for the Classification pipeline: Normal vs Hypertension vs Hypotension)*

## 📌 Prerequisites Checklist
- [x] ESP32 board + USB cable
- [x] MAX30102 sensor module
- [x] Reference BP monitor (digital)
- [x] Arduino IDE installed
- [x] Python 3.7+ installed

## ⚡ 5-Step Classification Quick Start

### Step 1: Hardware Setup
```
Connect MAX30102 to ESP32:
VCC  → 3.3V
GND  → GND
SDA  → GPIO21
SCL  → GPIO22
```

### Step 2: Install Software
**Arduino IDE:**
1. Install ESP32 board support
2. Install libraries: `SparkFun MAX3010x`, `TensorFlowLite_ESP32`, `Adafruit SSD1306`

**Python:**
```bash
pip install -r requirements.txt
```

### Step 3: Collect Data
**If you don't have enough data yet:**
1. Upload `esp32_data_collector/esp32_data_collector.ino` to your ESP32.
2. Run `python data_logger_bulk.py` to collect data rapidly.
3. *Optional (But Highly Recommended):* If you don't have extreme blood pressure patients to test on, run `python augment_dataset.py` to artificially generate Hypertension/Hypotension data so your AI can actively learn the difference!

### Step 4: Train Classification Model
Run these exact classification scripts sequentially:
```bash
python preprocess_data_classification.py
python train_model_classification.py
python convert_to_tflite.py
python generate_normalization.py
```

**IMPORTANT:** Copy the mean/std outputs from `generate_normalization.py` specifically!

### Step 5: Deploy & Test
1. Open the Classification Sketch: `esp32_bp_inference_classification/esp32_bp_inference_classification.ino`
2. Update normalization values (lines 40-41) using the numbers from Step 4.
3. Ensure the newly generated `model_data.h` is dragged into the `esp32_bp_inference_classification` folder.
4. Upload to ESP32.
5. Place your finger on the sensor.

The OLED screen will now boldly display your physiological state: **NORMAL**, **HYPERTENSION**, or **HYPOTENSION** along with the AI's confidence score! 

---
**Ready? Start building your Skripsi! 🚀**
