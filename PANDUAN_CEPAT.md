# 🚀 Panduan Cepat — Sistem Deteksi Hipertensi TinyML

**Ikuti langkah-langkah ini untuk menjalankan seluruh sistem dari awal sampai akhir!**

## 📌 Persiapan Awal
- ✅ Board ESP32 + Kabel USB
- ✅ Sensor MAX30102
- ✅ Tensimeter digital (untuk referensi)
- ✅ Arduino IDE sudah terinstal
- ✅ Python 3.7+ sudah terinstal

---

## Langkah 1: Rangkai Hardware (5 menit)

Hubungkan sensor MAX30102 ke ESP32 dengan kabel jumper:
```
VCC  →  3.3V
GND  →  GND
SDA  →  GPIO21
SCL  →  GPIO22
```

## Langkah 2: Instal Software (10 menit)

**Arduino IDE:**
1. Instal board ESP32 di Board Manager
2. Instal library: `SparkFun MAX3010x`

**Python:**
```bash
pip install -r requirements.txt
```

## Langkah 3: Kumpulkan Data (1-2 jam per sesi)

**Upload firmware perekam:**
1. Buka `esp32_data_collector/esp32_data_collector.ino` di Arduino IDE
2. Klik Upload ke ESP32

**Jalankan perekam data di komputer:**
```bash
cd raw_data_pipeline/python_tools
python raw_data_logger.py
```

**Cara merekam:**
1. Ukur tekanan darah pasien dengan tensimeter → catat hasilnya
2. Letakkan jari pasien di sensor MAX30102
3. Masukkan angka sistol dan diastol di terminal
4. Tunggu 25 detik hingga perekaman selesai
5. Ulangi 3-5 kali per pasien

**Target data:**
- Minimal **50 data Non-Hipertensi** (orang sehat, tekanan < 140/90)
- Minimal **30 data Hipertensi** (orang darah tinggi, tekanan ≥ 140/90)
- Semakin banyak dan beragam pasien, semakin pintar AI-nya!

## Langkah 4: Ekstrak Fitur (1 menit)

```bash
cd raw_data_pipeline/python_tools
python extract_features.py
```

Skrip ini akan:
- Memotong setiap rekaman 25 detik menjadi 2 jendela (10 detik)
- Mengekstrak 4 fitur (ir_mean, red_mean, ir_std, red_std)
- Membuang data yang tidak stabil (jitter > 12%)
- Menyimpan hasilnya di `dataset/extracted_features.csv`

## Langkah 5: Latih Model AI (2 menit)

```bash
python train_rf_tinyml.py
```

Skrip ini akan:
- Melatih model Random Forest menggunakan data yang sudah diekstrak
- Menampilkan hasil evaluasi (Accuracy, Precision, Recall, F1-Score)
- Menampilkan Confusion Matrix
- Mengekspor model ke file `rf_model.h` (kode C++ untuk ESP32)

**Hasil yang harus diperhatikan:**
- Accuracy > 80% → ✅ Bagus
- Recall Hipertensi > 60% → ✅ AI berhasil mendeteksi orang sakit
- Precision Hipertensi > 50% → ✅ Tebakan AI bisa dipercaya

## Langkah 6: Pasang di ESP32 (5 menit)

1. Pastikan file `rf_model.h` sudah ada di folder utama
2. Buka `esp32_bp_inference_classification/esp32_bp_inference_classification.ino`
3. Upload ke ESP32
4. Letakkan jari di sensor
5. Tunggu 10 detik → layar OLED akan menampilkan diagnosis!

---

## 🔍 Skrip Tambahan (Opsional)

| Skrip | Fungsi |
|-------|--------|
| `python plot_raw_data.py` | Melihat grafik gelombang sinyal mentah |
| `python plot_raw_correlation.py` | Melihat korelasi antar fitur dengan tekanan darah |

## ⚡ Tips Penting

1. **Jari harus diam** saat perekaman — gerakan sedikit saja bisa merusak data
2. **Selalu ukur tensimeter** sebelum setiap perekaman
3. **Jeda 1-2 menit** antar rekaman agar jari "istirahat"
4. **Cari pasien beragam** — usia, jenis kelamin, dan tingkat tekanan darah berbeda
5. **Jalankan extract_features.py** setiap kali selesai menambah data baru

---

**Siap? Mulai dari Langkah 1! 🚀**
