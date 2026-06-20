# Rangkuman Metode, Alasan, dan Kebutuhan Referensi

Dokumen ini merangkum seluruh metode teknis yang digunakan dalam sistem klasifikasi tekanan darah berbasis PPG pada ESP32, beserta alasan penggunaannya dan apakah memerlukan kutipan jurnal ilmiah atau tidak.

---

## 1. Linear Detrending (Penghilangan Tren Garis Lurus)

**Apa yang dilakukan:**
Menghitung garis tren lurus (y = mx + c) dari seluruh data dalam jendela 10 detik, lalu mengurangkannya dari sinyal asli. Hasilnya, sinyal bergoyang di sekitar angka 0.

**Mengapa diperlukan:**
Sinyal PPG mentah memiliki *baseline drift* (garis dasar yang perlahan naik atau turun) akibat pergeseran posisi jari, pernapasan, atau perubahan suhu kulit. Jika tren ini tidak dihilangkan, nilai Standar Deviasi (STD) akan membengkak secara palsu karena tren tersebut dihitung sebagai "simpangan", padahal itu bukan denyut jantung.

**Perlu jurnal?** ❌ Tidak wajib. Ini adalah teknik dasar DSP (*Digital Signal Processing*) yang diajarkan di mata kuliah Pengolahan Sinyal. Cukup sebutkan saja di metodologi. Namun jika ingin merujuk, bisa gunakan buku teks:
> Oppenheim, A. V. & Schafer, R. W. (2009) *Discrete-Time Signal Processing*, 3rd Ed., Prentice Hall.

---

## 2. IIR Butterworth Bandpass Filter Orde 2 (0.5 Hz - 5.0 Hz)

**Apa yang dilakukan:**
Menyaring sinyal sehingga hanya frekuensi antara 0.5 Hz sampai 5.0 Hz yang diloloskan. Frekuensi di bawah 0.5 Hz (pernapasan, goyangan lambat) dan di atas 5.0 Hz (getaran listrik, noise sensor) akan dibuang.

**Mengapa diperlukan:**
Detak jantung manusia normal berada di kisaran 30 - 300 bpm, yang setara dengan 0.5 - 5.0 Hz. Filter ini memastikan bahwa hanya sinyal denyut jantung murni yang tersisa untuk dihitung STD-nya.

**Mengapa Butterworth?**
Butterworth memiliki respons frekuensi paling datar (*maximally flat*) di zona lolos, artinya ia tidak akan memperkuat atau melemahkan frekuensi jantung secara tidak merata.

**Mengapa Orde 2?**
Orde 2 adalah titik keseimbangan (*sweet spot*) antara ketajaman pemotongan noise dan stabilitas sinyal. Orde yang lebih tinggi (3, 4) menyebabkan efek *Ringing* (getaran pantulan palsu) yang merusak bentuk gelombang, sementara Orde 1 terlalu tumpul untuk memotong noise secara efektif. (Lihat bukti visual di `filter_analysis.md`)

**Perlu jurnal?** ✅ Ya, disarankan. Referensi yang sudah ada di proposalmu:
> Elgendi, M. et al. (2019) *"The use of photoplethysmography for assessing hypertension"*, npj Digital Medicine.
> Sun, B. et al. (2023) *"The Case for tinyML in Healthcare"*, ACM SAC.

---

## 3. Zero-Phase Filtering (FiltFilt / Filter Maju-Mundur)

**Apa yang dilakukan:**
Menerapkan filter Bandpass sebanyak dua kali: sekali dari depan ke belakang (maju), lalu sekali lagi dari belakang ke depan (mundur). Hasilnya, posisi puncak gelombang tidak bergeser dari posisi aslinya (*zero-phase distortion*).

**Mengapa diperlukan:**
Filter biasa (*lfilter*) menyebabkan puncak gelombang bergeser ke kanan (*phase delay*). Pergeseran ini membuat perhitungan jarak antar-puncak (yang digunakan oleh SQA Time CV) menjadi tidak akurat. Dengan filtfilt, posisi puncak tetap di tempat aslinya sehingga perhitungan SQA tetap valid. (Lihat bukti numerik di `filtfilt_comparison.png`: Time CV berubah dari 2.40% menjadi 6.28% tanpa filtfilt)

**Perlu jurnal?** ❌ Tidak wajib. Ini adalah teknik standar DSP. Cukup sebutkan di metodologi. Jika diminta referensi:
> Gustafsson, F. (1996) *"Determining the initial states in forward-backward filtering"*, IEEE Transactions on Signal Processing.

---

## 4. Time Windowing (Segmentasi Jendela Waktu 10 Detik)

**Apa yang dilakukan:**
Membagi rekaman sinyal PPG menjadi blok-blok berdurasi 10 detik (1000 sampel pada 100 Hz). Setiap blok dianggap sebagai satu sampel data independen untuk pelatihan model.

**Mengapa diperlukan:**
File rekaman asli berdurasi ~25 detik. Dengan membaginya menjadi jendela 10 detik, kita mendapatkan 2 sampel data dari setiap sesi rekaman, sehingga jumlah dataset bertambah. Selain itu, 10 detik sudah cukup untuk menangkap sekitar 12-15 detak jantung, yang secara statistik sudah representatif untuk menghitung rata-rata dan standar deviasi.

**Perlu jurnal?** ✅ Ya, disarankan. Teknik windowing pada sinyal PPG banyak dibahas di literatur:
> Sun, B. et al. (2023) *"The Case for tinyML in Healthcare"*, ACM SAC.

---

## 5. Signal Quality Assessment (SQA) menggunakan Coefficient of Variation (CV)

**Apa yang dilakukan:**
Menghitung dua buah Signal Quality Index (SQI) untuk setiap jendela data:
- **Time CV**: Mengukur konsistensi jarak waktu antar-puncak denyut. Batas: ≤15%.
- **Amplitude CV**: Mengukur konsistensi tinggi puncak denyut. Batas: ≤25%.
Jendela yang melebihi batas akan otomatis dibuang dari dataset pelatihan.

**Mengapa diperlukan:**
Sinyal PPG sangat rentan terhadap *motion artifact* (goyangan jari) dan *pressure shift* (perubahan tekanan jari). Jika data yang cacat masuk ke dataset pelatihan, model Random Forest akan belajar dari pola yang salah dan menghasilkan prediksi yang tidak akurat. SQA berfungsi sebagai gerbang penjaga kualitas data. (Lihat bukti visual dan dampaknya di `comparison_analysis.md`)

**Mengapa menggunakan CV?**
CV berbentuk persentase (%) sehingga bersifat proporsional. Ia "adil" untuk semua pasien, baik yang jantungnya berdenyut kuat maupun lemah.

**Perlu jurnal?** ✅ Ya, sangat disarankan. Kata kunci pencarian:
> *"PPG Signal Quality Index"*, *"Motion artifact detection in PPG using coefficient of variation"*
> Referensi terkait: Elgendi et al. (2019), atau jurnal dari database NIH/IEEE tentang SQI.

---

## 6. Settling Time (Pembuangan 100 Sampel Pertama / 1 Detik)

**Apa yang dilakukan:**
Saat jari pertama kali diletakkan pada sensor, 100 sampel pertama (1 detik) dibaca oleh sensor tetapi langsung dibuang tanpa disimpan. Baru setelah itu, sistem mulai merekam data sesungguhnya.

**Mengapa diperlukan:**
Pada detik pertama, sensor MAX30102 sedang menyesuaikan intensitas LED dan photodetector terhadap jaringan jari baru. Data yang dihasilkan pada fase ini mengandung *transient noise* (lonjakan awal) yang sangat ekstrem dan tidak merepresentasikan sinyal fisiologis yang sebenarnya.

**Perlu jurnal?** ❌ Tidak wajib. Ini adalah praktik standar dalam pengoperasian sensor optik. Cukup dijelaskan secara logis di metodologi.

---

## 7. Amplitude Bounding SQI (Filter Kualitas di ESP32)

**Apa yang dilakukan:**
Setelah ESP32 menghitung nilai `ir_std` dari data yang sudah difilter, sistem mengecek apakah nilainya berada di dalam rentang fisiologis normal (100 - 500). Jika di luar rentang, prediksi dibatalkan dan layar OLED menampilkan pesan error.

**Mengapa diperlukan:**
Karena implementasi SQA penuh (Time CV + Amp CV) membutuhkan algoritma *Peak Detection* yang sangat kompleks dan berat untuk ESP32, maka digunakan pendekatan yang lebih ringan secara komputasi. Nilai `ir_std < 100` mengindikasikan sinyal mati (*blanching*/jari menekan terlalu kuat), sedangkan `ir_std > 500` mengindikasikan goyangan ekstrem (*motion artifact*).

**Perlu jurnal?** ❌ Tidak wajib. Angka batas (100 dan 500) diperoleh dari analisis empiris dataset yang dikumpulkan sendiri, bukan dari literatur. Cukup jelaskan bahwa batas tersebut ditentukan berdasarkan observasi data.

---

## 8. Deteksi Keberadaan Jari (Finger Detection)

**Apa yang dilakukan:**
Sebelum memulai perekaman, ESP32 terus-menerus membaca nilai IR dari sensor. Jika nilai IR > 50.000, sistem menganggap ada jari dan mulai proses pengukuran. Jika tidak, sistem menunggu.

**Mengapa diperlukan:**
Tanpa deteksi jari, sistem akan merekam 10 detik data udara kosong (nilai IR ~2.000) lalu membuangnya sia-sia. Dengan deteksi jari, sistem hanya bekerja saat dibutuhkan, menghemat waktu dan daya.

**Perlu jurnal?** ❌ Tidak. Ini adalah logika kontrol perangkat keras sederhana.

---

## 9. Ekstraksi 4 Fitur Statistik (ir_mean, red_mean, ir_std, red_std)

**Apa yang dilakukan:**
Dari setiap jendela 10 detik, diekstraksi 4 nilai:
- **ir_mean & red_mean** (dari sinyal mentah): Merepresentasikan komponen DC (volume darah statis, kondisi vasokonstriksi).
- **ir_std & red_std** (dari sinyal yang sudah difilter): Merepresentasikan komponen AC (kekuatan denyut nadi, tingkat kekakuan pembuluh darah / *arterial stiffness*).

**Mengapa 2 kanal (IR dan Red)?**
Cahaya IR menembus lebih dalam (arteri besar), sedangkan cahaya Red hanya menembus permukaan (kapiler kecil). Kombinasi keduanya memberikan informasi vaskular 3 dimensi kepada model Machine Learning.

**Mengapa mean dan std?**
Mean mendeteksi penyempitan pembuluh darah basal (*vasokonstriksi*). STD mendeteksi kekakuan dinding pembuluh darah (*arterial stiffness*). Keduanya adalah indikator utama hipertensi kronis.

**Perlu jurnal?** ✅ Ya. Referensi yang sudah ada:
> Nath, R. K. et al. (2018) *"Towards PPG-based Non-Invasive Blood Pressure Classification"*
> Tjitra, M. A. et al. (2024) *"Wireless Noninvasive BP Measurement Using MAX30102 and Random Forest"*

---

## 10. Random Forest Classifier (30 Pohon Keputusan)

**Apa yang dilakukan:**
Model Machine Learning yang terdiri dari 30 pohon keputusan independen. Setiap pohon menebak kelas secara mandiri, lalu hasil akhir ditentukan oleh suara terbanyak (*majority voting*).

**Mengapa Random Forest?**
- Tahan terhadap *overfitting* pada dataset kecil (~98 sampel).
- Struktur IF-ELSE sangat ringan untuk ESP32 (inferensi hanya 0.3 ms).
- Mudah dikonversi ke kode C++ menggunakan `micromlgen`.

**Perlu jurnal?** ✅ Ya. Referensi yang sudah ada:
> Tjitra, M. A. et al. (2024), Nath et al. (2018), Ahmed & Hassan (2022).

---

## 11. Konversi Model ke C++ (micromlgen)

**Apa yang dilakukan:**
Mengubah model Random Forest yang dilatih di Python (scikit-learn) menjadi file header C++ (`rf_model.h`) yang berisi representasi statis seluruh pohon keputusan dalam bentuk kode IF-ELSE.

**Mengapa bukan TensorFlow Lite?**
TensorFlow Lite membutuhkan *runtime interpreter* yang berat (~100KB+). Sedangkan micromlgen menghasilkan kode C++ murni tanpa dependensi eksternal, sehingga jauh lebih ringan dan cepat untuk ESP32.

**Perlu jurnal?** ✅ Ya. Referensi yang sudah ada:
> Ahmed, K. & Hassan, M. (2022) *"tinyCare: A tinyML-based Low-Cost Continuous Blood Pressure Estimation on the Extreme Edge"*

---

## Ringkasan Tabel

| No | Metode | Tujuan Singkat | Perlu Jurnal? |
|:--:|:-------|:---------------|:-------------:|
| 1 | Linear Detrending | Hilangkan tren garis dasar | ❌ Buku teks |
| 2 | Butterworth Bandpass Orde 2 | Isolasi frekuensi jantung (0.5-5 Hz) | ✅ Elgendi 2019 |
| 3 | FiltFilt (Zero-Phase) | Cegah pergeseran posisi puncak | ❌ Buku teks |
| 4 | Time Windowing 10 detik | Segmentasi data menjadi sampel | ✅ Sun 2023 |
| 5 | SQA (Time CV + Amp CV) | Buang data cacat dari dataset | ✅ Jurnal SQI |
| 6 | Settling Time 1 detik | Buang noise awal sensor | ❌ Logika hardware |
| 7 | Amplitude Bounding (ESP32) | Tolak sinyal cacat secara real-time | ❌ Empiris |
| 8 | Finger Detection | Mulai ukur hanya saat ada jari | ❌ Logika hardware |
| 9 | 4 Fitur (mean & std, IR & Red) | Input untuk Random Forest | ✅ Nath 2018, Tjitra 2024 |
| 10 | Random Forest 30-Tree | Klasifikasi Normal vs Hipertensi | ✅ Tjitra 2024 |
| 11 | micromlgen (C++ Porting) | Tanam model ke ESP32 | ✅ Ahmed 2022 |
