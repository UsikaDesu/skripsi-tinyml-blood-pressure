# 🚀 Panduan Cepat (Quick Start) - Klasifikasi

**Ikuti langkah-langkah ini untuk menjalankan sistem Klasifikasi Tekanan Darah Anda!**
*(Panduan ini khusus untuk jalur Klasifikasi: Normal vs Hipertensi vs Hipotensi)*

## 📌 Contoh Kebutuhan Persiapan
- [x] Board ESP32 + Kabel USB
- [x] Modul sensor MAX30102
- [x] Alat pengukur tekanan darah digital (untuk referensi)
- [x] Arduino IDE terinstal
- [x] Python 3.7+ terinstal

## ⚡ 5 Langkah Panduan Cepat Klasifikasi

### Langkah 1: Persiapan Perangkat Keras (Hardware)
```
Hubungkan MAX30102 ke ESP32:
VCC  → 3.3V
GND  → GND
SDA  → GPIO21
SCL  → GPIO22
```

### Langkah 2: Instalasi Perangkat Lunak (Software)
**Arduino IDE:**
1. Instal dukungan board ESP32
2. Instal library: `SparkFun MAX3010x`, `TensorFlowLite_ESP32`, `Adafruit SSD1306`

**Python:**
```bash
pip install -r requirements.txt
```

### Langkah 3: Pengambilan Data
**Jika Anda belum memiliki cukup data:**
1. Upload program `esp32_data_collector/esp32_data_collector.ino` ke ESP32 Anda.
2. Jalankan `python data_logger.py` di terminal untuk mulai mengumpulkan data secara otomatis.
3. *Opsional (Tapi Sangat Disarankan):* Jika Anda tidak memiliki data pasien dengan tekanan darah ekstrem (darah tinggi/rendah), jalankan `python augment_dataset.py` untuk mensimulasikan data Hipertensi/Hipotensi buatan agar AI dapat membedakannya!

### Langkah 4: Melatih Model Klasifikasi
Jalankan script klasifikasi Python berikut ini secara berurutan:
```bash
python preprocess_data_classification.py
python train_model_classification.py
python convert_to_tflite.py
python generate_normalization.py
```

**PENTING:** Anda wajib mencatat angka mean dan std deviation yang muncul di terminal setelah menjalankan script `generate_normalization.py`!

### Langkah 5: Penerapan (Deployment) & Pengujian
1. Buka file Sketch Klasifikasi: `esp32_bp_inference_classification/esp32_bp_inference_classification.ino` di Arduino IDE.
2. Perbarui nilai normalisasi (pada baris 40-41) menggunakan angka-angka yang Anda dapatkan di Langkah 4.
3. Pastikan file `model_data.h` yang baru saja di-generate (di Langkah 4) sudah Anda pindahkan ke dalam folder `esp32_bp_inference_classification`.
4. Klik **Upload** ke ESP32.
5. Letakkan jari Anda pada sensor.

Sekarang, layar OLED Anda akan dengan jelas menampilkan status fisiologis Anda: **NORMAL**, **HYPERTENSION** (Hipertensi), atau **HYPOTENSION** (Hipotensi) beserta tingkat kepercayaan (Confidence Score) dari AI tersebut! 

---
**Siap? Mari selesaikan Skripsi Anda! 🚀**
