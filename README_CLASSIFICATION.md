# TinyML Blood Pressure Classification System (Categorical Target)

A complete end-to-end system for collecting physiological data, training a **Categorical classification neural network**, and deploying it to an ESP32 for real-time Blood Pressure state estimation (Normal vs Hypertension vs Hypotension) using TensorFlow Lite Micro.

## ⚠️ Medical Disclaimer

**This project is for educational and research purposes only.** TinyML-based blood pressure estimation is not a substitute for medical-grade devices. Results should not be used for medical diagnosis.

## 📊 Why Classification over Regression?
This specific branch of the repository throws away continuous measurement guessing (`122/81 mmHg`) in favor of medical state **Classification**. This is vastly safer for Extreme Outlier patients. 
If a Neural Network's maximum training data ceiling is `140` systolic, a regression model cannot accurately predict a patient with `190` systolic. However, a Classification model will simply verify that their sensor readings severely breach the "Normal" threshold and confidently assign them to the **HYPERTENSION** class.

## 💻 Software Workflow

### Phase 1: Data Gathering (or Synthesizing)
1. **Physical Gathering:** Use `data_logger_bulk.py` alongside the `esp32_data_collector.ino` sketch to take baseline measurements. 
2. **Synthetic Outlier Generation:** Use `augment_dataset.py` to artificially morph your normal baselines into Hypertension (`>130 Sys`) and Hypotension (`<90 Sys`) profiles. This is strictly required if you cannot physically find extreme outlier humans to train the AI on!

### Phase 2: Classification Training Pipeline
```bash
# 1. Clean data and package it into 3 categorical buckets (Class 0, 1, 2)
python preprocess_data_classification.py

# 2. Train the Softmax Neural Network (Tracks categorical Accuracy, not Error Distance)
python train_model_classification.py

# 3. Convert Keras model to a Quantized C++ Array
python convert_to_tflite.py

# 4. Extract sensor data Standard Scaling limits
python generate_normalization.py
```

### Phase 3: Hardware Inference
Navigate into the `esp32_bp_inference_classification` directory.
1. Drag the newly generated `model_data.h` into this directory.
2. Open `esp32_bp_inference_classification.ino`.
3. Update `feature_mean` and `feature_std` variables at the top of the file using the terminal output from Phase 2.
4. Upload to the ESP32. 

The OLED screen will dynamically render the target Class ("NORMAL", "HYPOTENSION", "HYPERTENSION") and display the AI's internal decimal confidence score (e.g., `Conf: 96%`). 

---
**Built with ❤️ for TinyML Medical Diagnostics**
