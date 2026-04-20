# 🚀 Quick Start Guide - TinyML Blood Pressure System

**Follow these steps to get your system running fast!**

## 📌 Prerequisites Checklist
- [V] ESP32 board + USB cable
- [V] MAX30102 sensor module
- [V] Reference BP monitor (digital)
- [V] Arduino IDE installed
- [V] Python 3.7+ installed

## ⚡ 5-Step Quick Start

### Step 1: Hardware Setup (5 minutes)
```
Connect MAX30102 to ESP32:
VCC  → 3.3V
GND  → GND
SDA  → GPIO21
SCL  → GPIO22
```

### Step 2: Install Software (10 minutes)

**Arduino IDE:**
1. Install ESP32 board support
2. Install libraries: `SparkFun MAX3010x`, `TensorFlowLite_ESP32`

**Python:**
```bash
pip install -r requirements.txt
```

### Step 3: Collect Data (30-60 minutes)

Upload data collector:
```
1. Open esp32_data_collector.ino in Arduino IDE
2. Upload to ESP32
```

Run data logger:
```bash
python data_logger.py
```

**Collect at least 50 samples:**
- 20 samples while resting
- 15 samples after walking around
- 15 samples after light exercise

### Step 4: Train Model (5-10 minutes)

Run these commands in order:
```bash
python preprocess_data.py
python train_model.py
python convert_to_tflite.py
python generate_normalization.py
```

**IMPORTANT:** Copy the output from `generate_normalization.py` and paste into `esp32_bp_inference.ino` (lines 34-35)

### Step 5: Deploy & Test (5 minutes)

```
1. Open esp32_bp_inference.ino
2. Update normalization values (from Step 4)
3. Verify model_data.h is in the same folder
4. Upload to ESP32
5. Open Serial Monitor (115200 baud)
6. Place finger on sensor
```

You should see BP estimates every 2 seconds! 🎉

## 🎯 Expected Timeline
- **First run:** 1-2 hours total
- **Most time:** Data collection (quality matters!)
- **Fastest parts:** Training and deployment

## 🐛 Common Issues

**Issue:** Sensor not detected
- **Fix:** Check wiring, use 3.3V not 5V

**Issue:** Invalid readings
- **Fix:** Better finger contact, warm hands, adjust pressure

**Issue:** Poor accuracy (>20 mmHg error)
- **Fix:** Collect more diverse data, check reference device

**Issue:** COM port not found
- **Fix:** Install USB drivers, check Device Manager

## 📖 For Detailed Info
See [README.md](file:///c:/Users/Kayyis/OneDrive/Documents/skripsi%20pantek/koding/README.md) for complete documentation.

## 💡 Pro Tips
1. **More data = better accuracy** (aim for 100+ samples)
2. **Vary conditions** (different times, different BP states)
3. **Keep finger still** during measurement
4. **Personal calibration** improves results significantly
5. **Check training_history.png** to verify model learned properly

---

**Ready? Start with Step 1! 🚀**
