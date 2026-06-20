# Review Proposal Skripsi vs Implementasi Nyata

Berikut adalah hasil review terhadap file `Proposal Kayyis Real.docx` dibandingkan dengan sistem yang sudah kita bangun bersama.

---

## ✅ Bagian yang Sudah BENAR dan Sesuai

1. **Judul** — "Sistem Klasifikasi Tingkat Tekanan Darah Berbasis Sinyal PPG Menggunakan TinyML pada ESP32" → Sudah tepat.
2. **4 Fitur (ir_mean, red_mean, ir_std, red_std)** — Disebutkan dengan benar di Intisari dan Bab III.
3. **Bandpass Filter Butterworth Orde 2 (0.5 - 5 Hz)** — Disebutkan dengan benar di Bab III.
4. **Windowing 10 detik (1000 sampel)** — Disebutkan dengan benar.
5. **Klasifikasi Biner (Normal vs Hipertensi)** — Sesuai dengan kode kita (Kelas 0 dan 1).
6. **Pelabelan WHO (Sistolik ≥140 atau Diastolik ≥90)** — Sesuai persis dengan `extract_features.py`.
7. **Penggunaan micromlgen untuk porting ke C++** — Disebutkan di Bab III bagian Optimasi.
8. **Sensor MAX30102 + ESP32 + OLED** — Sesuai dengan hardware yang kita gunakan.
9. **SQA menggunakan CV dari interval antar-puncak** — Disebutkan di Bab III bagian Akuisisi Data.
10. **Posisi jari berlawanan dengan manset** — Disebutkan dan direferensi dengan baik (Elgendi et al., 2019).

---

## ⚠️ Bagian yang PERLU DIPERBAIKI / Tidak Sesuai

### 1. Istilah "TensorFlow Lite" — SALAH
**Lokasi:** Intisari (paragraf 65) dan Abstract (paragraf 73)
**Tertulis:** *"...dikonversi menjadi TensorFlow Lite for Microcontrollers (TinyML)..."*
**Kenyataan:** Kita **TIDAK** menggunakan TensorFlow Lite sama sekali. Kita menggunakan pustaka `micromlgen` yang mengonversi model scikit-learn menjadi kode C++ murni (file `.h`). Ini adalah dua hal yang sangat berbeda. TensorFlow Lite membutuhkan runtime interpreter yang berat, sedangkan micromlgen menghasilkan kode statis tanpa runtime.
**Saran:** Ganti menjadi: *"...dikonversi menjadi representasi kode C++ murni menggunakan pustaka micromlgen agar dapat ditanamkan langsung ke dalam memori ESP32."*

> Catatan: Di Bab III paragraf 165, kamu sudah menyebut micromlgen dengan benar. Jadi ada **inkonsistensi** antara Intisari dan Bab III.

### 2. SQA Hanya Menyebut 1 Gate (Time CV) — KURANG LENGKAP
**Lokasi:** Bab III, paragraf 161
**Tertulis:** Hanya menyebut CV dari jarak antar-puncak (Time CV) saja.
**Kenyataan:** Sistem kita menggunakan **DUA gate SQA**:
- **Gate 1: Time CV (≤15%)** — Mengukur konsistensi jeda waktu antar-denyut.
- **Gate 2: Amplitude CV (≤25%)** — Mengukur konsistensi tinggi puncak gelombang.
**Saran:** Tambahkan penjelasan Gate 2 (Amplitude CV) di paragraf tersebut.

### 3. Istilah "Dtrend" — SALAH KETIK
**Lokasi:** Beberapa tempat di Bab III (paragraf 132, 140)
**Tertulis:** "Dtrend"
**Seharusnya:** "Detrend" atau "Linear Detrending" (istilah resmi di dunia DSP).

### 4. Metode FiltFilt (Zero-Phase) Tidak Disebutkan
**Lokasi:** Bab III, paragraf 132
**Tertulis:** Hanya menyebut "IIR Butterworth Band-Pass Filter orde-2"
**Kenyataan:** Kita menggunakan metode **FiltFilt** (filter maju-mundur) yang menghasilkan Zero-Phase Distortion. Ini adalah detail teknis penting yang membedakan sistem kita dari filter biasa (lfilter) karena filtfilt menjaga posisi asli puncak gelombang tanpa pergeseran.
**Saran:** Tambahkan: *"...diimplementasikan menggunakan metode zero-phase filtering (filtfilt) untuk memastikan tidak terjadi pergeseran fase pada posisi puncak gelombang asli."*

### 5. "Membuang 100 Sampel Pertama" — PERLU DICEK
**Lokasi:** Bab III, paragraf 160
**Tertulis:** *"...membuang (drop) 100 sampel data mentah pertama..."*
**Kenyataan di ESP32:** Kode ESP32 saat ini **TIDAK** membuang 100 sampel pertama. Ia langsung merekam 1000 sampel dan menggunakannya semua. 
**Kenyataan di Python:** Di `extract_features.py`, windowing dimulai dari indeks 0 (tidak ada pembuangan 100 sampel pertama). Yang ada adalah pembagian file 25 detik menjadi 2 jendela @ 10 detik, dan 5 detik terakhir dibuang.
**Saran:** Sesuaikan penjelasan di proposal dengan implementasi nyata, atau tambahkan logika pembuangan 100 sampel ke kode jika memang diinginkan.

### 6. SQA di ESP32 Tidak Disebutkan
**Lokasi:** Bab III, paragraf 170
**Tertulis:** Hanya menyebut proses inferensi RF dan SVM di ESP32.
**Kenyataan:** Kita baru saja menambahkan **Amplitude Bounding SQI** di ESP32 (`if irStd < 100 || irStd > 500`). Fitur ini membuat ESP32 secara otomatis menolak sinyal yang cacat sebelum melakukan prediksi.
**Saran:** Tambahkan penjelasan bahwa ESP32 juga memiliki mekanisme penolakan sinyal secara real-time.

### 7. Intisari/Abstract Menggunakan Kata "Estimasi" — AMBIGU
**Lokasi:** Intisari (paragraf 64) dan Abstract (paragraf 72)
**Tertulis:** *"...sistem estimasi tekanan darah..."*
**Kenyataan:** Sistem kita melakukan **KLASIFIKASI** (menentukan kelas: Normal atau Hipertensi), bukan **ESTIMASI** (menebak angka sistolik/diastolik). Judul skripsimu sendiri sudah benar menggunakan kata "Klasifikasi".
**Saran:** Ganti kata "estimasi" menjadi "klasifikasi tingkat" di seluruh Intisari dan Abstract agar konsisten dengan judul.

---

## 🔴 Bagian yang BELUM Diimplementasi (Masih Janji di Proposal)

### SVM (Support Vector Machine) Belum Dibuat
**Lokasi:** Hampir di seluruh proposal (Intisari, Bab I, Bab III)
**Status:** Kita baru mengimplementasikan **Random Forest saja**. SVM belum dilatih dan belum di-deploy ke ESP32.
**Dampak:** Ini adalah janji terbesar di proposal. Dosen penguji pasti akan bertanya tentang perbandingan RF vs SVM.
**Saran:** Ini harus menjadi prioritas utama setelah ujian proposal disetujui (tahap implementasi skripsi).

---

## Rangkuman Prioritas Perbaikan

| No | Item | Urgensi | Alasan |
|:--:|:-----|:-------:|:-------|
| 1 | Ganti "TensorFlow Lite" → "micromlgen/kode C++" di Intisari/Abstract | 🔴 Tinggi | Bisa dianggap tidak paham tools sendiri |
| 2 | Ganti "estimasi" → "klasifikasi" di Intisari/Abstract | 🔴 Tinggi | Inkonsisten dengan judul |
| 3 | Tambahkan Gate 2 SQA (Amplitude CV) | 🟡 Sedang | Agar sesuai implementasi |
| 4 | Tambahkan penjelasan FiltFilt (Zero-Phase) | 🟡 Sedang | Detail teknis penting |
| 5 | Perbaiki "Dtrend" → "Detrend" | 🟢 Rendah | Typo kecil |
| 6 | Sesuaikan penjelasan "100 sampel dibuang" | 🟡 Sedang | Harus cocok dengan kode |
| 7 | Siapkan implementasi SVM | 🔴 Tinggi | Janji utama proposal |
