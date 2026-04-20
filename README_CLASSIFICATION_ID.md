# Sistem Klasifikasi Tekanan Darah Berbasis TinyML

Sistem end-to-end lengkap untuk mengumpulkan data fisiologis, melatih **Jaringan Saraf Tiruan (Neural Network) Klasifikasi Kelas**, dan menanamkannya ke dalam mikrokontroler ESP32 untuk estimasi status Tekanan Darah secara real-time (Normal vs Hipertensi vs Hipotensi) menggunakan TensorFlow Lite Micro.

## ⚠️ Penafian Medis (Disclaimer)

**Proyek ini secara eksklusif hanya untuk tujuan pendidikan dan penelitian (Skripsi).** Estimasi tekanan darah berbasis TinyML pada sensor optik ini bukanlah alat pengganti perangkat medis komersial. Hasil dari sensor ini tidak boleh digunakan untuk mengambil keputusan medis.

## 📊 Mengapa Memilih Klasifikasi Daripada Regresi?
Versi sistem / *branch* ini sepenuhnya membuang sistem prediksi angka eksak (contoh: menebak `122/81 mmHg`) dan menggantinya dengan sistem **Klasifikasi Kelas/Status** diagnosis. Ini adalah pendekatan secara matematis jauh lebih aman, terutama untuk outlier (kasus angka tekanan ekstrem). 
Apabila plafon maksimum pelatihan Neural Network Anda hanyalah data dengan sistolik `140`, model regresi biasanya akan selalu meremehkan pasien dengan sistolik `190` dan memprediksinya sebagai ~145. Sebaliknya, model Klasifikasi secara kuat akan mendeteksi varians ekstrem dari pukulan detak pasien sebagai lonjakan dari titik "Threshold Normal" dan dengan aman memprediksi pasien tersebut masuk ke dalam Kelas **HYPERTENSION** (Hipertensi).

## 💻 Alur Kerja Software (Workflow)

### Tahap 1: Pengumpulan Data (& Sintetis Data Buatan)
1. **Pengumpulan Fisik Asli:** Gunakan `data_logger.py` bersama dengan program Arduino `esp32_data_collector.ino` untuk mengambil data "Normal" Anda sendiri sebagai pengukuran 'baseline' (dasar). 
2. **Menghasilkan Anomali Sintetis:** Gunakan `augment_dataset.py` untuk secara matematis mengubah dan memanipulasi baseline normal Anda ke dalam profil Hipertensi Ekstrem (Sistolik `>135`) dan Hipotensi (`<90`). Tahap ini **diwajibkan** jika Anda tidak bisa menemukan orang/relawan yang benar-benar sakit tekanan darah ekstrem untuk dipindai oleh device Anda!

### Tahap 2: Proses Melatih Kecerdasan Buatan (AI Pipeline)
```bash
# 1. Bersihkan Data, Normalisasi, dan Konversi ke dalam 3 Kelas Kategorikal (Kelas 0, 1, 2)
python preprocess_data_classification.py

# 2. Latih Neural Network Softmax (Melacak Akurasi klasifikasi persentase, bukan lagi Jarak Error numerik)
python train_model_classification.py

# 3. Konversikan model AI Keras yang besar menjadi format C++ Array kuantisasi ringan (TFLite)
python convert_to_tflite.py

# 4. Ekstrak batasan Normalisasi Sensor (Wajib dicopy ke ESP32 nantinya)
python generate_normalization.py
```

### Tahap 3: Penerapan pada Hardware (Inference)
Pindah ke dalam direktori `esp32_bp_inference_classification`.
1. Pindahkan atau *drag* file hasil `model_data.h` yang baru ke dalam folder ini.
2. Buka program `esp32_bp_inference_classification.ino` di Arduino IDE.
3. Perbarui variabel `feature_mean` dan `feature_std` pada barisan kode bagian atas secara persis sesuai urutan angka terminal output yang Anda dapat dari Tahap 2 barusan!
4. Klik Upload ke ESP32. 

LCD OLED Anda akan secara aman dan dinamis mencetak output Status medis Anda ("NORMAL", "HYPOTENSION", "HYPERTENSION") secara teks literal beserta tingkat akurasi kepercayaan (Confidence) persentase desimal dari AI secara langsung (Contoh tampilan: `Conf: 96%`). 

---
**Dibuat dengan ❤️ untuk Riset Medis Skripsi Berbasis IoT / TinyML**
