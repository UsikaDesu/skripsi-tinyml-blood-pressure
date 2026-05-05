# Sistem Deteksi Hipertensi Berbasis TinyML (ESP32)

Proyek skripsi ini membangun alat pendeteksi tekanan darah tinggi (hipertensi) menggunakan sensor cahaya MAX30102 yang ditanamkan pada mikrokontroler ESP32. Sistem mengklasifikasikan status tekanan darah seseorang menjadi **Non-Hipertensi** atau **Hipertensi** berdasarkan standar WHO/Indonesia (≥140/90 mmHg).

## ⚠️ Peringatan Medis

**Proyek ini dibuat untuk keperluan pendidikan dan penelitian (Skripsi).** Alat ini bukan pengganti tensimeter medis. Jangan gunakan hasil alat ini untuk keputusan medis. Selalu konsultasikan dengan tenaga kesehatan profesional.

## 🔧 Alat dan Bahan

### Perangkat Keras
| Komponen | Fungsi |
|----------|--------|
| ESP32 | Mikrokontroler utama (otak sistem) |
| MAX30102 | Sensor cahaya inframerah & merah untuk membaca denyut nadi |
| OLED SSD1306 | Layar kecil untuk menampilkan hasil diagnosis |
| Tensimeter Digital | Alat ukur tekanan darah pembanding (untuk pengambilan data latih) |
| Kabel USB | Menghubungkan ESP32 ke komputer |
| Kabel Jumper | Menghubungkan sensor ke ESP32 |

### Rangkaian Kabel
```
MAX30102  →  ESP32
───────────────────
VCC       →  3.3V
GND       →  GND
SDA       →  GPIO21
SCL       →  GPIO22
```

### Perangkat Lunak
| Software | Kegunaan |
|----------|----------|
| Arduino IDE | Menulis dan mengunggah program ke ESP32 |
| Python 3.7+ | Menjalankan skrip pengolahan data dan pelatihan AI |
| Library Arduino: `SparkFun MAX3010x` | Berkomunikasi dengan sensor MAX30102 |
| Library Arduino: `micromlgen` | Mengubah model AI menjadi kode C++ |

Instal dependensi Python:
```bash
pip install -r requirements.txt
```

## 📁 Struktur Proyek

```
koding/
├── esp32_data_collector/          ← Program ESP32 untuk merekam data mentah
│   └── esp32_data_collector.ino
│
├── raw_data_pipeline/             ← Pusat pengolahan data & pelatihan AI
│   ├── dataset/                   ← Tempat penyimpanan seluruh data
│   │   ├── raw_data_*.csv         ← File rekaman mentah dari sensor
│   │   └── extracted_features.csv ← Hasil ekstraksi fitur (siap latih)
│   └── python_tools/              ← Skrip-skrip Python
│       ├── raw_data_logger.py     ← Merekam data mentah dari ESP32
│       ├── extract_features.py    ← Mengekstrak fitur dari data mentah
│       ├── train_rf_tinyml.py     ← Melatih model Random Forest
│       ├── plot_raw_data.py       ← Melihat grafik gelombang sinyal
│       └── plot_raw_correlation.py← Melihat korelasi antar fitur
│
├── esp32_bp_inference_classification/ ← Program ESP32 untuk diagnosis
│   └── esp32_bp_inference_classification.ino
│
├── rf_model.h                     ← Model AI dalam format C++ (hasil pelatihan)
└── requirements.txt               ← Daftar library Python yang dibutuhkan
```

## 🚀 Cara Kerja Singkat

### 1. Pengambilan Data
- Pasang sensor MAX30102 di ujung jari
- Ukur tekanan darah dengan tensimeter secara bersamaan
- ESP32 merekam sinyal cahaya inframerah (IR) dan merah (RED) selama 25 detik
- Data mentah disimpan otomatis ke file CSV

### 2. Pengolahan Data
- Setiap file 25 detik dipotong menjadi 2 jendela (masing-masing 10 detik)
- Dari setiap jendela, diekstrak 4 fitur utama:
  - `ir_mean` → Rata-rata cahaya inframerah (ketebalan darah)
  - `red_mean` → Rata-rata cahaya merah (kadar oksigen)
  - `ir_std` → Variasi denyut inframerah (kekuatan detak)
  - `red_std` → Variasi denyut merah
- Jendela yang tidak stabil (jitter > 12%) otomatis dibuang

### 3. Pelatihan AI
- Model Random Forest dan SVM dilatih menggunakan 4 fitur di atas
- Sistem membandingkan kinerja kedua model (Studi Komparasi)
- Model terbaik diekspor menjadi kode C++ untuk ESP32

### 4. Diagnosis di ESP32
- Pasien meletakkan jari di sensor selama 10 detik
- ESP32 mengekstrak fitur dan menjalankan model AI
- Layar OLED menampilkan hasil: **"NON-HIPERTENSI"** atau **"HIPERTENSI"**

## 📊 Standar Klasifikasi (WHO/Indonesia)

| Status | Tekanan Sistolik | Tekanan Diastolik |
|--------|:---:|:---:|
| **Non-Hipertensi** | < 140 mmHg | DAN < 90 mmHg |
| **Hipertensi** | ≥ 140 mmHg | ATAU ≥ 90 mmHg |

## 🐛 Solusi Masalah Umum

| Masalah | Solusi |
|---------|--------|
| Sensor tidak terdeteksi | Periksa kabel, pastikan tegangan 3.3V (bukan 5V) |
| Semua data menunjukkan "Invalid" | Jari tidak menempel sempurna, atau jari terlalu dingin |
| Detak jantung tidak stabil | Tahan jari diam, jangan bergerak, tunggu 10 detik |
| COM Port tidak ditemukan | Instal driver USB (CP2102 / CH340) |
| Akurasi model rendah | Tambah data latih, terutama data hipertensi |

## 📚 Referensi

- [MAX30102 Datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf)
- [PPG-based Blood Pressure Estimation (PubMed)](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC6359303/)
- [Scikit-learn: Random Forest](https://scikit-learn.org/stable/modules/ensemble.html#forests-of-randomized-trees)

---
**Dibuat untuk Skripsi TinyML — Deteksi Dini Hipertensi Berbasis IoT** ❤️
