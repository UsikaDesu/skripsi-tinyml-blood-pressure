# CATATAN REVISI PROPOSAL → V3 (Crest Time Ratio)

> Dokumen ini mencatat secara detail seluruh bagian yang harus diubah
> pada file `Proposal Kayyis Real.docx` agar sesuai dengan implementasi V3.
> Referensi baris mengacu pada `proposal_text_extract.txt`.

---

## 🔴 PERUBAHAN WAJIB (Fundamental — Harus Diubah)

### 1. INTISARI — Deskripsi Fitur (Baris 65)

**Lokasi di Word:** Halaman Intisari, paragraf 2

**Teks Lama:**
> "...sistem akan melakukan ekstraksi fitur statistik langsung dari intensitas pantulan cahaya sensor MAX30102. Fitur yang diekstraksi meliputi **nilai rata-rata (mean) dan standar deviasi (std) dari kanal Infrared (IR), serta nilai rata-rata (mean) dan standar deviasi (std) dari kanal Red**..."

**Teks Baru (V3):**
> "...sistem akan melakukan ekstraksi fitur fisiologis dari morfologi sinyal PPG yang diperoleh sensor MAX30102. Fitur yang diekstraksi meliputi **Crest Time Ratio (rasio waktu sistolik terhadap panjang siklus jantung), Heart Rate (laju detak jantung), serta rasio AC/DC dari kanal Infrared (IR Ratio) dan kanal Red (Red Ratio)** sebagai indikator indeks perfusi..."

---

### 2. ABSTRACT — Deskripsi Fitur (Baris 73)

**Lokasi di Word:** Halaman Abstract, paragraf 2

**Teks Lama:**
> "...The extracted features include the **mean and standard deviation (std) values of the Infrared (IR) channel, as well as the mean and standard deviation (std) values of the Red channel**..."

**Teks Baru (V3):**
> "...The extracted features include the **Crest Time Ratio (the ratio of systolic upstroke duration to the total pulse cycle), Heart Rate, and the AC/DC ratio of the Infrared (IR Ratio) and Red (Red Ratio) channels** as perfusion index indicators..."

---

### 3. ABSTRACT — Judul (Baris 68)

**Lokasi di Word:** Halaman Abstract, baris judul

**Teks Lama:**
> "Blood Pressure **Estimation** Based on PPG Signals..."

**Teks Baru:**
> "Blood Pressure **Level Classification** Based on PPG Signals..."

*(Kata "Estimation" tidak sesuai karena sistem melakukan Klasifikasi, bukan menebak angka BP)*

---

### 4. BAB III — Deskripsi Umum: Penjelasan Fitur (Baris 133)

**Lokasi di Word:** Bab III, subbab 3.1 Deskripsi Umum, paragraf 2

**Teks Lama (seluruh penjelasan "dua cabang" fitur mean/std):**
> "...Prinsip kerja sistem didasarkan pada... Untuk fitur nilai rata-rata (mean) yang merepresentasikan komponen DC, sinyal mentah (raw) langsung diekstraksi... Sebaliknya, untuk mengekstraksi nilai fluktuasi denyut nadi atau standar deviasi (std)..."

**Teks Baru (V3):**
> "...Prinsip kerja sistem didasarkan pada pengukuran intensitas cahaya yang dipantulkan oleh jari subjek pada sensor MAX30102. Sinyal PPG yang diperoleh kemudian diproses melalui tahapan Linear Detrending dan Zero-Phase Butterworth Band-Pass Filter (0.5–5.0 Hz) untuk menghilangkan baseline drift dan noise. Setelah itu, sistem mendeteksi puncak-puncak sistolik gelombang PPG untuk mengekstraksi empat fitur fisiologis utama: (1) **Crest Time Ratio** — rasio waktu dari lembah ke puncak sistolik terhadap interval antar-puncak, yang mencerminkan kekakuan arteri (arterial stiffness) secara independen dari detak jantung; (2) **Heart Rate** — laju detak jantung dalam satuan BPM; (3) **IR Ratio** — rasio komponen AC/DC kanal infrared sebagai indeks perfusi; dan (4) **Red Ratio** — rasio komponen AC/DC kanal merah..."

---

### 5. BAB III — Ekstraksi Fitur: Daftar 4 Fitur (Baris 142–150)

**Lokasi di Word:** Bab III, subbab 3.3 Rancangan Sistem, daftar bullet "Keempat fitur tersebut adalah"

**Teks Lama (ganti seluruh 4 poin lama):**
> - IR Mean (nilai rata-rata infrared)
> - IR Std (standar deviasi infrared)
> - Red Mean (nilai rata-rata red)
> - Red Std (standar deviasi red)

**Teks Baru (V3):**
> - **Crest Time Ratio** — Rasio waktu sistolik (dari lembah ke puncak) terhadap interval siklus jantung. Secara matematis: CTR = Rise Time (ms) / Pulse Interval (ms). Fitur ini merupakan indikator kekakuan arteri yang tidak bergantung pada laju detak jantung.
> - **Heart Rate** — Laju detak jantung dalam satuan BPM, dihitung dari jarak rata-rata antar puncak gelombang PPG.
> - **IR Ratio** — Rasio AC/DC kanal infrared, yaitu standar deviasi sinyal terfilter dibagi nilai rata-rata sinyal mentah, dikali 100. Merepresentasikan indeks perfusi (Perfusion Index) pada panjang gelombang IR.
> - **Red Ratio** — Rasio AC/DC kanal merah, dengan perhitungan serupa dengan IR Ratio.

---

### 6. BAB III — Rancangan Sistem: Paragraf "Dua Cabang Pemrosesan" (Baris 141)

**Lokasi di Word:** Bab III, subbab 3.3, paragraf setelah Gambar 3.1

**Teks Lama:**
> "...aliran data akan dibagi menjadi dua cabang secara paralel setelah 100 data pertama dibuang. Cabang pertama memproses sinyal mentah kanal merah dan inframerah secara langsung untuk mencari nilai mean komponen DC. Cabang kedua menerima sinyal inframerah dan merah yang telah difilter..."

**Teks Baru (V3):**
> "...aliran data diproses melalui satu jalur preprocessing tunggal. Sinyal mentah dari kanal IR dan Red masing-masing melewati tahapan: (1) Linear Detrending untuk meratakan baseline drift, (2) Inversi sinyal karena sifat reflektansi sensor MAX30102 (puncak sistolik sebenarnya bernilai minimum pada sinyal mentah), dan (3) Zero-Phase Butterworth Band-Pass Filter orde-2 (0.5–5.0 Hz). Setelah preprocessing, sistem melakukan deteksi puncak (Peak Detection) pada sinyal IR yang telah diinversi. Dari puncak-puncak tersebut, sistem mengekstraksi empat fitur fisiologis yang kemudian dinormalisasi (Z-Score) sebelum dimasukkan ke model klasifikasi..."

---

### 7. BAB III — Proses Akuisisi: Penjelasan SQA (Baris 162)

**Lokasi di Word:** Bab III, subbab 3.5 Proses Akuisisi Data, poin 4

**Teks Lama:**
> "...Sistem akan menghitung Coefficient of Variation (CV) dari perbedaan jarak antar puncak nadi dalam jendela waktu tersebut. Jika nilai CV melebihi persentase yang telah ditentukan maka data tersebut akan langsung dibuang..."

**Teks Baru (V3 — tambahkan Gate 2):**
> "...Sistem mengimplementasikan dua gerbang SQA secara berurutan: **(Gate 1) Time CV** — sistem menghitung Coefficient of Variation (CV) dari interval waktu antar puncak. Jika Time CV > 15%, jendela data dianggap mengandung jitter (artefak gerak) dan dibuang. **(Gate 2) Amplitude CV** — sistem menghitung CV dari tinggi amplitudo antar puncak. Jika Amplitude CV > 25%, jendela data dianggap mengandung pergeseran tekanan jari (pressure shift artifact) dan dibuang. Hanya jendela yang lolos kedua gerbang ini yang diteruskan ke tahap ekstraksi fitur..."

---

### 8. BAB III — Optimasi Model: Normalisasi (Baris 165–166)

**Lokasi di Word:** Bab III, subbab 3.6 Optimasi dan Konversi Model TinyML

**Tambahkan paragraf baru setelah kalimat pertama tentang Random Forest dan SVM:**

> "Sebelum proses pelatihan, fitur-fitur yang telah diekstraksi dinormalisasi menggunakan metode Z-Score Normalization (StandardScaler). Normalisasi ini memastikan setiap fitur memiliki skala yang setara sehingga algoritma SVM (yang sangat sensitif terhadap skala fitur) dapat bekerja secara optimal. Parameter normalisasi (nilai Mean dan Standar Deviasi) yang diperoleh dari data latih kemudian di-hardcode ke dalam firmware ESP32, sehingga alat dapat melakukan normalisasi secara mandiri tanpa memerlukan komputer."

---

### 9. BAB III — Optimasi Model: Validasi (Baris 165)

**Tambahkan paragraf baru tentang metodologi validasi:**

> "Validasi model dilakukan menggunakan metode **Stratified Group K-Fold Cross Validation** (5 lipatan). Metode ini dipilih karena memastikan dua kondisi sekaligus: (1) tidak ada data dari subjek yang sama yang bocor antara data latih dan data uji (subject-independent evaluation), dan (2) setiap lipatan memiliki rasio kelas Normal dan Hipertensi yang seimbang (stratified). Pendekatan ini menghasilkan evaluasi yang lebih konservatif namun lebih valid secara klinis dibandingkan metode pembagian data konvensional."

---

### 10. BAB III — Rancangan Pengujian (Baris 171)

**Lokasi di Word:** Bab III, subbab 3.7

**Teks Lama:**
> "...Angka dari empat fitur turunan yang dihasilkan kemudian dimasukkan secara bergantian ke dalam fungsi klasifikasi bawaan rf_model.predict() dan svm_model.predict()..."

**Teks Baru (V3):**
> "...Keempat fitur fisiologis (Crest Time Ratio, Heart Rate, IR Ratio, Red Ratio) yang telah diekstraksi kemudian dinormalisasi secara manual menggunakan parameter Z-Score yang telah di-hardcode. Hasil normalisasi kemudian dimasukkan ke dalam fungsi klasifikasi svm_model.predict(). Sistem juga memiliki mekanisme penolakan sinyal real-time (SQA on-device): jika Heart Rate terhitung di luar rentang 40–180 BPM, atau jika Crest Time Ratio bernilai nol (artefak), sistem akan menampilkan pesan kesalahan dan meminta pengukuran ulang tanpa memberikan prediksi..."

---

## 🟡 PERUBAHAN PENDUKUNG (Disarankan)

### 11. INTISARI — Kata Kunci (Baris 66)

**Tambahkan kata kunci:**
> "Kata kunci: Photoplethysmography (PPG), TinyML, ESP32, tekanan darah, machine learning, MAX30102, **Crest Time Ratio, arterial stiffness**."

---

### 12. ABSTRACT — Keywords (Baris 74)

**Tambahkan keyword:**
> "Keywords: Photoplethysmography (PPG), TinyML, ESP32, blood pressure, machine learning, MAX30102, **Crest Time Ratio, arterial stiffness**."

---

### 13. BAB II — Tinjauan Pustaka: Tambah Referensi CTR (Baris 126)

**Lokasi di Word:** Bab II, setelah paragraf tentang SVM dan Random Forest

**Tambahkan kalimat:**
> "Secara fisiologis, parameter waktu morfologi gelombang PPG seperti **Crest Time** (waktu dari lembah ke puncak sistolik) telah diakui sebagai penanda kekakuan arteri (arterial stiffness) yang berkorelasi dengan kondisi hipertensi (Elgendi et al., 2019). Normalisasi Crest Time terhadap interval siklus jantung menghasilkan **Crest Time Ratio** yang bersifat independen terhadap laju detak jantung, sehingga lebih robust untuk pemantauan lintas subjek (Allen, 2007)."

---

### 14. BAB II — Tinjauan Pustaka: Update Tabel 2.1

Di kolom "Fitur yang Digunakan" pada tabel tinjauan pustaka, pastikan terdapat referensi yang menggunakan Crest Time atau fitur morfologi PPG sebagai fitur klasifikasi.

---

### 15. DAFTAR PUSTAKA — Tambah Referensi CTR

**Tambahkan referensi berikut:**

> Allen, J. (2007) 'Photoplethysmography and its application in clinical physiological measurement', *Physiological Measurement*, 28(3), pp. R1–R39. https://doi.org/10.1088/0967-3334/28/3/R01

*(Ini adalah paper referensi utama yang mendefinisikan Crest Time dalam konteks PPG secara klinis)*

---

## 📋 RINGKASAN PRIORITAS

| No | Bagian | Urgensi | Isi Perubahan |
|:--:|:-------|:-------:|:-------------|
| 1 | Intisari — Fitur | 🔴 Wajib | Ganti mean/std → CTR, HR, IR/Red Ratio |
| 2 | Abstract — Fitur | 🔴 Wajib | Ganti mean/std → CTR, HR, IR/Red Ratio |
| 3 | Abstract — Judul | 🔴 Wajib | Estimation → Level Classification |
| 4 | Bab III 3.1 — Deskripsi Umum | 🔴 Wajib | Jelaskan pipeline V3 (inversi, CTR) |
| 5 | Bab III 3.3 — Daftar Fitur | 🔴 Wajib | Ganti 4 poin fitur lama → 4 fitur V3 |
| 6 | Bab III 3.3 — Dua Cabang | 🔴 Wajib | Ganti arsitektur dua cabang → satu jalur |
| 7 | Bab III 3.5 — SQA | 🔴 Wajib | Tambahkan Gate 2 (Amplitude CV) |
| 8 | Bab III 3.6 — Normalisasi | 🔴 Wajib | Tambahkan Z-Score Normalization |
| 9 | Bab III 3.6 — Validasi | 🔴 Wajib | Tambahkan StratifiedGroupKFold |
| 10 | Bab III 3.7 — Pengujian | 🔴 Wajib | Update nama fitur + SQA on-device |
| 11 | Intisari — Kata Kunci | 🟡 Saran | Tambah CTR, arterial stiffness |
| 12 | Abstract — Keywords | 🟡 Saran | Tambah CTR, arterial stiffness |
| 13 | Bab II — Tinjauan | 🟡 Saran | Tambah kalimat tentang CTR fisiologis |
| 14 | Tabel 2.1 | 🟡 Saran | Update fitur di baris referensi PPG |
| 15 | Daftar Pustaka | 🟡 Saran | Tambah Allen (2007) sebagai referensi CTR |
