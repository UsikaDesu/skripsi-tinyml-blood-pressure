# DRAFT REVISI BAB 3 SKRIPSI (VERSI V4 FINAL)

> Dokumen ini adalah draf Bab 3 yang **sudah diperbarui secara total** untuk menyesuaikan dengan arsitektur V4 kita hari ini, **serta memenuhi 6 poin revisi spesifik dari Dosen Pembimbing Anda**.

---

## BAB III: METODOLOGI PENELITIAN / PERANCANGAN SISTEM

### 3.1 Deskripsi Umum Sistem
Prinsip kerja sistem didasarkan pada pengukuran intensitas cahaya reflektif dari pembuluh darah jari menggunakan sensor MAX30102. Sinyal mentah dari kanal Inframerah (IR) dan Merah (Red) pertama-tama diproses melalui tahapan *Linear Detrending* dan *Zero-Phase Butterworth Band-Pass Filter* orde-2 (0.5–5.0 Hz) untuk membuang *baseline drift* akibat pernapasan dan *noise* frekuensi tinggi. Setelah gelombang bersih, sistem melakukan *Peak Detection* untuk menemukan lembah dan puncak sistolik.

Dari puncak-puncak tersebut, sistem mengekstraksi empat fitur utama.

### 3.2 Ekstraksi Ciri (Feature Extraction)
Jendela sinyal (10 detik) akan diekstraksi menjadi 4 fitur fisiologis untuk memisahkan kelas Normal dan Hipertensi:
1. **Crest Time Ratio (CTR):** Rasio waktu sistolik dibagi interval total satu siklus jantung. Fitur ini independen terhadap kecepatan detak jantung.
2. **Heart Rate (HR):** Laju detak jantung rata-rata (BPM).
3. **IR Ratio:** Rasio komponen AC/DC kanal inframerah (deviasi standar sinyal filter dibagi rata-rata sinyal mentah).
4. **Red Ratio:** Rasio komponen AC/DC pada kanal cahaya merah.

### 3.3 Threshold Signal Quality Assessment (SQA) Berbasis Metode IQR
Mengingat sensor optik rentan terhadap gangguan pergerakan, sistem menerapkan seleksi ketat menggunakan perhitungan *Coefficient of Variation* (CV). Alih-alih menebak nilai ambang batas (*threshold*), penelitian ini menggunakan metode statistik matematis **Interquartile Range (IQR)** terhadap sebaran data mentah untuk menetapkan batas atas (*upper bound*) kewajaran sinyal:
1. **Time CV (Ambang Batas Maksimal = 10.0%):** Mengukur variabilitas jarak waktu antar puncak (interval *RR*). Berdasarkan standar klinis *Heart Rate Variability* (HRV) dan perhitungan IQR, variasi detak jantung normal pada kondisi istirahat berada di bawah 10%. Jika Time CV melampaui 10.0% dalam jendela 10 detik, hal ini secara kuat mengindikasikan adanya deteksi puncak palsu (*false peak*) akibat jari bergoyang, atau kelainan irama jantung ekstrem yang akan merusak ekstraksi *Crest Time*. Sinyal ini secara otomatis dibuang.
2. **Amplitude CV (Ambang Batas Maksimal = 37.0%):** Mengukur variabilitas tinggi puncak gelombang PPG. Sensor optik sangat sensitif terhadap tekanan kontak. Berdasarkan distribusi frekuensi jari partisipan menggunakan metode IQR, batas atas kewajaran fluktuasi ini berada di angka 37.0%. Perubahan volume darah melampaui 37.0% menandakan *Motion Artifact* berat. Sinyal yang melebihi batas ini dibuang agar model tidak mempelajari pola yang cacat.

> **Pencegahan Training-Serving Skew:**
> *Threshold* SQA ini diterapkan secara sinkron dan identik pada skrip *Python* saat ekstraksi data dan pada C++ ESP32 saat inferensi *real-time*.

### 3.4 Jumlah dan Karakteristik Dataset serta Penyeimbangan Kelas
Pengumpulan data awal melibatkan **14 subjek (pasien)** dengan beragam sesi pengukuran. Untuk menjamin kemurnian data biologis (tanpa algoritma data tiruan seperti SMOTE) namun tetap adil, diberlakukan aturan pemangkasan (*Undersampling*) dan pembersihan data ketat:
1. **2 Window per Sesi:** Rekaman sinyal dipotong menjadi dua bagian utama (10 detik pertama dan 10 detik kedua) untuk memperbanyak sampel data valid.
2. **Pembersihan Kelas Minoritas per Subjek:** Apabila dalam rekaman seorang pasien (yang mayoritas diklasifikasikan Normal) tiba-tiba terdapat 1 atau 2 detik pembacaan Hipertensi akibat kesalahan penempatan manset, data *outlier* tersebut secara otomatis dibuang untuk menjaga kemurnian profil fisiologis subjek.
3. **Maksimal 10 Window per Subjek:** Untuk mencegah terjadinya *overfitting* di mana model hanya "menghafal" pola jantung dari satu orang pasien yang sering diambil datanya, sistem menerapkan batas tegas maksimal 10 data (*window*) per subjek yang diambil menggunakan teknik *Round-Robin*.
Berdasarkan aturan tersebut, terbentuk dataset final sejumlah **107 data valid**, yang memiliki karakteristik dua kelas yang nyaris seimbang sempurna: **56 Non-Hipertensi (Normal)** dan **51 Hipertensi**.

### 3.5 Pemisahan Subjek Training dan Testing
Sistem dievaluasi menggunakan metode **Stratified Group K-Fold Cross Validation** (k=5) dengan identitas subjek (`subject_id`) sebagai grup pengunci. Metode ini menjamin bahwa seluruh data (*window*) dari subjek tertentu (misalnya Subjek A) secara utuh hanya akan dialokasikan ke dalam *Training Set* atau *Testing Set*, dan **tidak akan pernah berada di keduanya secara bersamaan**. 
Pada setiap iterasi (*fold*), model akan dilatih menggunakan data dari 11 subjek, dan murni diuji (*Testing*) pada 3 subjek baru yang sepenuhnya asing bagi mesin. Hal ini dilakukan untuk menghindari *Data Leakage* serta membuktikan alat dapat bekerja pada pasien baru yang belum pernah ditemui.

### 3.6 Parameter Model (RF dan SVM) & Normalisasi Fitur
Pencarian hyperparameter (*Memory-Constrained Auto-Tune*) dilakukan secara ekstensif menggunakan *Grid Search* untuk menemukan arsitektur terbaik dari ratusan kombinasi dua algoritma:
1. **Random Forest (RF):** Dieksplorasi dengan rentang parameter jumlah pohon (`n_estimators`: 5 hingga 100), kedalaman maksimal (`max_depth`: 3 hingga 7), dan batas minimum sampel daun (`min_samples_leaf`: 1 hingga 3). Untuk model RF, fitur diumpankan dalam bentuk mentah (*raw values*) tanpa normalisasi.
2. **Support Vector Machine (SVM):** Dieksplorasi dengan parameter jenis *kernel* (Linear, RBF, Sigmoid, Polynomial), penalti *error* (`C`), dan parameter kurva (`gamma`). Khusus untuk SVM, **sangat diwajibkan menerapkan Normalisasi Fitur (Z-Score Normalization)** menggunakan teknik `StandardScaler`. Nilai rata-rata (*Mean*) dan skala (*Scale*) dihitung saat *training*, lalu di-*hardcode* ke dalam ESP32 agar SVM dapat menghitung jarak *hyperplane* dengan presisi tinggi.

Pemilihan model pemenang dievaluasi menggunakan **Tabel Pareto** yang diurutkan berdasarkan metrik **Lower Bound Accuracy** (Akurasi rata-rata dikurangi *Standard Deviation* hasil *Cross Validation*). Metrik ini menjamin bahwa model yang terpilih benar-benar tahan banting (*robust*) dan stabil pada pengujian baru, bukan sekadar model yang kebetulan memiliki akurasi semu tinggi di salah satu pelipatan (*fold*).

### 3.7 Metrik Evaluasi: F1-Score dan Ukuran Model TinyML
Pemilihan model pemenang (*Champion Model*) tidak hanya didasarkan pada **Akurasi** (*Accuracy*) konvensional, namun divalidasi menggunakan dua metrik kritis berikut:
1. **F1-Score:** Metrik rata-rata harmonis dari *Precision* dan *Recall* wajib disertakan untuk memastikan model memiliki keseimbangan yang kuat dalam mengenali kelas Normal maupun Hipertensi tanpa adanya kecenderungan menebak secara bias ke satu kelas mayoritas.
2. **Ukuran Model (Memori Flash):** Untuk diimplementasikan ke dalam *Embedded System* (TinyML), model *Python* terpilih harus dikonversi menjadi bahasa C/C++ statis. Model hasil konversi dituntut (*threshold constraint*) memiliki ukuran *footprint* **di bawah batas 400 KB** agar tidak memicu *stack overflow* pada memori internal ESP32 yang memiliki keterbatasan memori, sekaligus menyisakan ruang lega bagi operasional *RTOS* dan modul *OLED Display*.
