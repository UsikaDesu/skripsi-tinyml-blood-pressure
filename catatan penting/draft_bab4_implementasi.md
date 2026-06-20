# BAB IV IMPLEMENTASI

## 4.1 Implementasi Akuisisi Data

### 4.1.1 Firmware Akuisisi Data (ESP32)

Pada tahap akuisisi data, firmware `esp32_raw_streamer.ino` ditanamkan pada mikrokontroler ESP32. Firmware ini bertugas membaca data mentah dari sensor MAX30102 secara kontinu pada frekuensi sampling 100 Hz, kemudian mengirimkan data tersebut melalui komunikasi serial ke komputer dalam format `timestamp,IR,RED`.

Konfigurasi sensor MAX30102 yang digunakan pada tahap akuisisi meliputi pengaturan kecerahan LED (*LED Brightness*) sebesar `0x1F`, jumlah *sample average* sebanyak 4, mode LED ganda (*Red + IR*), laju sampling 100 Hz, lebar pulsa 411 μs, dan rentang ADC 4096 (resolusi penuh). Konfigurasi ini dirancang agar sinyal PPG yang dihasilkan memiliki resolusi dan kestabilan yang optimal.

### 4.1.2 Skrip Penangkap Data (Python)

Di sisi komputer, skrip Python `raw_data_logger.py` menangkap aliran data serial dari ESP32 dan menyimpannya ke dalam file CSV. Gambar 4.1 menunjukkan fungsi utama dari skrip tersebut.

```
 1. def record_raw_session():
 2.     port = find_esp32_port()
 3.     ser = serial.Serial(port, BAUD_RATE, timeout=1)
 4.     ser.reset_input_buffer()
 5.  
 6.     start_time = time.time()
 7.     raw_rows = []
 8.  
 9.     while (time.time() - start_time) < RECORDING_TIME_SECONDS:
10.         if ser.in_waiting > 0:
11.             line = ser.readline().decode('utf-8', errors='ignore').strip()
12.             parts = line.split(',')
13.             if len(parts) == 3:
14.                 try:
15.                     timestamp = int(parts[0])
16.                     ir_raw = int(parts[1])
17.                     red_raw = int(parts[2])
18.                     raw_rows.append([timestamp, ir_raw, red_raw])
19.                 except ValueError:
20.                     pass
21.  
22.     ser.close()
23.     raw_rows = raw_rows[100:]  # Buang 100 sampel pertama (settling time)
```
Gambar 4.1 Fungsi Perekaman Data Serial

Fungsi `record_raw_session()` bekerja dengan membuka koneksi serial ke ESP32 pada *baud rate* 115200 (baris 3), kemudian membaca data secara kontinu selama durasi yang telah ditentukan (baris 9). Setiap baris data yang diterima di-*parsing* berdasarkan pemisah koma menjadi tiga komponen: *timestamp*, nilai IR, dan nilai Red (baris 12-18). Setelah perekaman selesai, 100 sampel pertama dibuang (baris 23) untuk menghilangkan *noise* transien akibat proses stabilisasi sensor (*settling time*).

### 4.1.3 Pelabelan Data

Setelah perekaman selesai, sistem meminta pengguna memasukkan nilai tekanan darah dari tensimeter digital sebagai data referensi (*ground truth*). Gambar 4.2 menunjukkan proses pelabelan dan penyimpanan data.

```
 1.     label_in = input("Enter BP (or 'X' to discard): ").strip()
 2.     sys_str, dia_str = label_in.split('/')
 3.     systolic = int(sys_str)
 4.     diastolic = int(dia_str)
 5.  
 6.     filename = f"raw_data_{datetime.now().strftime('%Y%m%d_%H%M%S')}_bp{systolic}_{diastolic}.csv"
 7.     filepath = os.path.join(DATASET_DIR, filename)
 8.  
 9.     with open(filepath, 'w', newline='') as f:
10.         writer = csv.writer(f)
11.         writer.writerow(["time_ms", "ir_raw", "red_raw", "systolic_label", "diastolic_label"])
12.         for row in raw_rows:
13.             writer.writerow([row[0], row[1], row[2], systolic, diastolic])
```
Gambar 4.2 Proses Pelabelan dan Penyimpanan Dataset

Nilai sistolik dan diastolik disimpan pada setiap baris data (baris 12-13) agar pelabelan melekat langsung pada sinyal mentah. Penamaan file menggunakan format otomatis yang menyertakan *timestamp* dan nilai tekanan darah (baris 6) untuk memudahkan identifikasi.

---

## 4.2 Implementasi Pra-pemrosesan Sinyal

### 4.2.1 Segmentasi dan Detrending

Skrip `extract_features.py` memproses seluruh file data mentah secara bertahap. Setiap file rekaman (~25 detik) dipotong menjadi jendela-jendela berdurasi 10 detik (1000 sampel pada 100 Hz). Gambar 4.3 menunjukkan proses segmentasi dan *detrending*.

```
 1. window_size = 1000
 2. num_windows = len(df) // window_size
 3.  
 4. for w in range(num_windows):
 5.     start_idx = w * window_size
 6.     end_idx = start_idx + window_size
 7.  
 8.     ir_raw = df['ir_raw'].values[start_idx:end_idx]
 9.     red_raw = df['red_raw'].values[start_idx:end_idx]
10.  
11.     # Detrending: Menghilangkan baseline drift
12.     ir_detrended = signal.detrend(ir_raw)
13.     red_detrended = signal.detrend(red_raw)
```
Gambar 4.3 Segmentasi Jendela Waktu dan Linear Detrending

Setiap jendela berisi 1000 titik sampel (baris 1), yang kemudian dipotong menggunakan indeks awal dan akhir (baris 5-6). Fungsi `signal.detrend()` dari pustaka SciPy (baris 12-13) menghitung garis tren linier (y = mx + c) dari seluruh data dalam jendela, lalu mengurangkannya dari sinyal asli untuk menghilangkan *baseline drift*.

### 4.2.2 Bandpass Filter dan Zero-Phase Filtering

Sinyal yang telah di-*detrend* kemudian disaring menggunakan *IIR Butterworth Band-Pass Filter* orde-2 dengan frekuensi *cutoff* 0.5 Hz - 5.0 Hz. Filter diimplementasikan menggunakan metode *zero-phase filtering* (filtfilt). Gambar 4.4 menunjukkan proses penyaringan sinyal.

```
 1. nyq = 0.5 * SAMPLING_RATE_HZ
 2. b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
 3. ir_filtered = signal.filtfilt(b, a, ir_detrended)
 4. red_filtered = signal.filtfilt(b, a, red_detrended)
```
Gambar 4.4 Implementasi Butterworth Bandpass Filter dengan FiltFilt

Koefisien filter dihitung menggunakan `signal.butter()` (baris 2) berdasarkan frekuensi Nyquist (baris 1). Fungsi `signal.filtfilt()` (baris 3-4) menerapkan filter secara dua arah (maju dan mundur) sehingga tidak terjadi pergeseran fase (*phase delay*) pada posisi puncak gelombang asli. Hal ini sangat penting karena perhitungan *Signal Quality Index* pada tahap selanjutnya bergantung pada ketepatan posisi puncak.

### 4.2.3 Signal Quality Assessment (SQA)

Setiap jendela data dievaluasi kualitasnya menggunakan dua buah *Signal Quality Index* (SQI) berbasis *Coefficient of Variation* (CV). Gambar 4.5 menunjukkan implementasi SQA dengan dua gerbang penolakan.

```
 1. peaks, _ = signal.find_peaks(ir_filtered, distance=50,
 2.                              prominence=np.std(ir_filtered)*0.5)
 3.  
 4. if len(peaks) < 2:
 5.     continue  # Flatline: Tidak ada puncak terdeteksi
 6.  
 7. # Gate 1: Time CV (konsistensi jarak antar-puncak)
 8. rr_intervals = np.diff(peaks)
 9. mean_rr = np.mean(rr_intervals)
10. time_cv = (np.std(rr_intervals) / mean_rr) * 100
11.  
12. if time_cv > 15.0:
13.     continue  # Jitter terdeteksi, buang jendela ini
14.  
15. # Gate 2: Amplitude CV (konsistensi tinggi puncak)
16. amplitudes = ir_filtered[peaks]
17. mean_amp = np.mean(amplitudes)
18. cv_amp = np.std(amplitudes) / mean_amp
19.  
20. if cv_amp > 0.25:
21.     continue  # Goyangan vertikal terdeteksi, buang jendela ini
```
Gambar 4.5 Implementasi Signal Quality Assessment (SQA) Dua Gerbang

Puncak-puncak gelombang PPG dideteksi menggunakan `signal.find_peaks()` dengan parameter jarak minimum antar-puncak 50 sampel (baris 1). Gerbang pertama (*Gate 1*, baris 7-13) menghitung *Time CV* dari jarak waktu antar-puncak; jika nilainya melebihi 15%, jendela data ditolak karena mengindikasikan adanya *jitter* akibat *motion artifact*. Gerbang kedua (*Gate 2*, baris 15-21) menghitung *Amplitude CV* dari tinggi puncak gelombang; jika nilainya melebihi 25%, jendela ditolak karena mengindikasikan adanya perubahan tekanan jari terhadap sensor.

### 4.2.4 Ekstraksi Fitur

Dari setiap jendela yang lolos kedua gerbang SQA, diekstraksi empat fitur statistik. Gambar 4.6 menunjukkan proses ekstraksi fitur.

```
 1. # Fitur DC (dari sinyal mentah, tanpa filter)
 2. ir_mean = int(np.round(np.mean(ir_raw)))
 3. red_mean = int(np.round(np.mean(red_raw)))
 4.  
 5. # Fitur AC (dari sinyal yang telah difilter)
 6. ir_std = int(np.round(np.std(ir_filtered)))
 7. red_std = int(np.round(np.std(red_filtered)))
 8.  
 9. # Pelabelan kelas berdasarkan standar WHO
10. if systolic >= 140 or diastolic >= 90:
11.     bp_class = 1  # Hipertensi
12. else:
13.     bp_class = 0  # Non-Hipertensi
```
Gambar 4.6 Ekstraksi Fitur dan Pelabelan Kelas

Nilai `ir_mean` dan `red_mean` (baris 2-3) dihitung dari sinyal mentah yang belum difilter untuk merepresentasikan komponen DC (volume darah dasar). Sedangkan `ir_std` dan `red_std` (baris 6-7) dihitung dari sinyal yang telah difilter untuk merepresentasikan komponen AC (fluktuasi denyut nadi). Pelabelan kelas (baris 10-13) mengikuti standar WHO untuk klasifikasi biner.

---

## 4.3 Implementasi Pelatihan Model

### 4.3.1 Pelatihan Random Forest

Pelatihan model Random Forest dilakukan menggunakan skrip `train_rf_tinyml.py`. Gambar 4.7 menunjukkan konfigurasi model dan proses pelatihan.

```
 1. from sklearn.ensemble import RandomForestClassifier
 2. from sklearn.model_selection import train_test_split, cross_val_score
 3. from sklearn.metrics import classification_report, accuracy_score, confusion_matrix
 4. from micromlgen import port
 5.  
 6. FEATURES = ['ir_mean', 'red_mean', 'ir_std', 'red_std']
 7.  
 8. X_train, X_test, y_train, y_test = train_test_split(
 9.     X, y, test_size=0.2, random_state=42, stratify=y)
10.  
11. clf = RandomForestClassifier(
12.     n_estimators=50,
13.     max_depth=5,
14.     min_samples_leaf=2,
15.     class_weight='balanced',
16.     random_state=42
17. )
18. clf.fit(X_train, y_train)
```
Gambar 4.7 Konfigurasi dan Pelatihan Model Random Forest

Model Random Forest dikonfigurasi dengan 50 pohon keputusan (baris 12), kedalaman maksimum 5 (baris 13) untuk menjaga ukuran model tetap ringan, dan parameter `class_weight='balanced'` (baris 15) yang secara otomatis menyesuaikan bobot kelas untuk mengatasi ketimpangan jumlah data antara kelas Non-Hipertensi dan Hipertensi. Dataset dibagi menjadi 80% data latih dan 20% data uji dengan stratifikasi (baris 8-9) untuk memastikan distribusi kelas yang proporsional.

### 4.3.2 Konversi Model ke Kode C++ (TinyML Porting)

Model yang telah dilatih kemudian dikonversi menjadi representasi kode C++ murni menggunakan pustaka `micromlgen`. Gambar 4.8 menunjukkan proses konversi.

```
 1. cpp_code = port(clf)
 2.  
 3. with open(MODEL_OUTPUT_PATH, 'w') as f:
 4.     f.write(cpp_code)
 5.  
 6. size_kb = os.path.getsize(MODEL_OUTPUT_PATH) / 1024
 7. print(f"Header File: {MODEL_OUTPUT_PATH}")
 8. print(f"Sketch size: {size_kb:.1f} KB")
```
Gambar 4.8 Konversi Model ke Kode C++ Menggunakan micromlgen

Fungsi `port()` dari pustaka `micromlgen` (baris 1) mengubah seluruh struktur pohon keputusan Random Forest menjadi serangkaian pernyataan `IF-ELSE` dalam bahasa C++. Hasil konversi disimpan sebagai file header `rf_model.h` (baris 3-4) yang kemudian disertakan ke dalam proyek firmware ESP32.

---

## 4.4 Implementasi Firmware Inferensi ESP32

### 4.4.1 Implementasi DSP pada ESP32

Karena mikrokontroler ESP32 tidak memiliki pustaka pemrosesan sinyal bawaan, tiga fungsi DSP diimplementasikan secara manual dalam bahasa C++. Gambar 4.9 menunjukkan fungsi *Linear Detrending*.

```
 1. void detrend_signal(float* buffer, int length) {
 2.   double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
 3.   for (int i = 0; i < length; i++) {
 4.     double x = i;
 5.     double y = buffer[i];
 6.     sum_x += x;  sum_y += y;
 7.     sum_xy += x * y;  sum_xx += x * x;
 8.   }
 9.   double n = length;
10.   double m = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
11.   double c = (sum_y - m * sum_x) / n;
12.  
13.   for (int i = 0; i < length; i++) {
14.     double trend = m * i + c;
15.     buffer[i] = (float)(buffer[i] - trend);
16.   }
17. }
```
Gambar 4.9 Implementasi Linear Detrending pada ESP32

Fungsi ini menghitung koefisien gradien (m) dan *intercept* (c) dari garis tren menggunakan metode *Least Squares* (baris 2-11), kemudian mengurangkan nilai tren dari setiap titik data (baris 13-16).

Gambar 4.10 menunjukkan implementasi filter IIR dan metode *zero-phase filtering* (filtfilt).

```
 1. void lfilter(float* b, float* a, float* x, float* y, int length) {
 2.   for (int n = 0; n < length; n++) {
 3.     float xn  = x[n];
 4.     float xn1 = (n >= 1) ? x[n-1] : 0;
 5.     float xn2 = (n >= 2) ? x[n-2] : 0;
 6.     float xn3 = (n >= 3) ? x[n-3] : 0;
 7.     float xn4 = (n >= 4) ? x[n-4] : 0;
 8.     float yn1 = (n >= 1) ? y[n-1] : 0;
 9.     float yn2 = (n >= 2) ? y[n-2] : 0;
10.     float yn3 = (n >= 3) ? y[n-3] : 0;
11.     float yn4 = (n >= 4) ? y[n-4] : 0;
12.     y[n] = b[0]*xn + b[1]*xn1 + b[2]*xn2 + b[3]*xn3 + b[4]*xn4
13.          - a[1]*yn1 - a[2]*yn2 - a[3]*yn3 - a[4]*yn4;
14.   }
15. }
16.  
17. void filtfilt_bandpass(float* buffer, int length) {
18.   float b[5] = {0.01658193f, 0.0f, -0.03316386f, 0.0f, 0.01658193f};
19.   float a[5] = {1.0f, -3.5862398f, 4.8462898f, -2.9304272f, 0.6704579f};
20.  
21.   float* temp_y = new float[length];
22.   lfilter(b, a, buffer, temp_y, length);         // Forward filter
23.   for (int i = 0; i < length; i++)
24.     buffer[i] = temp_y[length - 1 - i];           // Reverse
25.   lfilter(b, a, buffer, temp_y, length);          // Backward filter
26.   for (int i = 0; i < length; i++)
27.     buffer[i] = temp_y[length - 1 - i];           // Reverse back
28.   delete[] temp_y;
29. }
```
Gambar 4.10 Implementasi Filter IIR dan Zero-Phase Filtering (FiltFilt) pada ESP32

Fungsi `lfilter()` (baris 1-15) mengimplementasikan filter IIR (*Infinite Impulse Response*) orde-4 dengan menggunakan koefisien pembilang (b) dan penyebut (a). Fungsi `filtfilt_bandpass()` (baris 17-29) menerapkan filter secara dua arah: pertama maju (baris 22), kemudian membalik urutan data (baris 23-24), lalu memfilter maju lagi (baris 25), dan membalik kembali (baris 26-27). Koefisien filter (baris 18-19) diperoleh dari fungsi `scipy.signal.butter()` pada Python agar identik dengan proses pelatihan.

### 4.4.2 Deteksi Jari dan Settling Time

Sebelum memulai perekaman, firmware melakukan *polling* untuk mendeteksi keberadaan jari pada sensor. Gambar 4.11 menunjukkan implementasi deteksi jari dan fase stabilisasi.

```
 1. // Polling: Tunggu sampai jari terdeteksi
 2. while (true) {
 3.   while (!particleSensor.available()) {
 4.     particleSensor.check();
 5.   }
 6.   long irCheck = particleSensor.getIR();
 7.   particleSensor.getRed();
 8.   particleSensor.nextSample();
 9.  
10.   if (irCheck > 50000) {
11.     break;  // Jari terdeteksi
12.   }
13. }
14.  
15. // Settling time: Buang 100 sampel pertama (1 detik)
16. for (int i = 0; i < 100; i++) {
17.   while (!particleSensor.available()) {
18.     particleSensor.check();
19.   }
20.   particleSensor.getIR();   // Baca tapi BUANG
21.   particleSensor.getRed();
22.   particleSensor.nextSample();
23. }
```
Gambar 4.11 Deteksi Jari dan Settling Time

Sistem membaca nilai IR secara kontinu (baris 2-12); jika nilai IR melebihi 50.000 (baris 10), jari dianggap terdeteksi dan proses dilanjutkan. Setelah jari terdeteksi, 100 sampel pertama dibaca namun langsung dibuang (baris 16-23) untuk menghilangkan *noise* transien akibat proses stabilisasi komponen optik internal sensor MAX30102.

### 4.4.3 Ekstraksi Fitur dan Inferensi pada ESP32

Setelah 1000 sampel dikumpulkan, firmware melakukan pra-pemrosesan dan inferensi. Gambar 4.12 menunjukkan proses ekstraksi fitur dan klasifikasi.

```
 1. // Mean dari sinyal mentah (komponen DC)
 2. float irSum = 0, redSum = 0;
 3. for (int i = 0; i < SAMPLE_WINDOW; i++) {
 4.   irSum += irBuffer[i];
 5.   redSum += redBuffer[i];
 6. }
 7. float irMean = irSum / SAMPLE_WINDOW;
 8. float redMean = redSum / SAMPLE_WINDOW;
 9.  
10. // Konversi ke float, lalu Detrend + Bandpass FiltFilt
11. float* irFiltered = new float[SAMPLE_WINDOW];
12. for (int i = 0; i < SAMPLE_WINDOW; i++)
13.   irFiltered[i] = (float)irBuffer[i];
14. detrend_signal(irFiltered, SAMPLE_WINDOW);
15. filtfilt_bandpass(irFiltered, SAMPLE_WINDOW);
16.  
17. // Std dari sinyal bersih (komponen AC)
18. float irStd = sqrt(irVar / SAMPLE_WINDOW);
19. float redStd = sqrt(redVar / SAMPLE_WINDOW);
20.  
21. // Amplitude Bounding SQI
22. if (irStd < 100.0 || irStd > 500.0) {
23.   oledMessage(">> ERROR <<", "", " Sinyal Rusak!", " Jari Goyang?");
24.   return;  // Batalkan prediksi
25. }
26.  
27. // Inferensi Random Forest
28. float features[4] = {irMean, redMean, irStd, redStd};
29. int prediction = rf.predict(features);
```
Gambar 4.12 Ekstraksi Fitur, SQI, dan Inferensi pada ESP32

Nilai rata-rata dihitung dari sinyal mentah (baris 2-8), sementara standar deviasi dihitung dari sinyal yang telah melalui proses *detrending* dan *bandpass filtfilt* (baris 10-19). Sebelum inferensi, dilakukan pemeriksaan *Amplitude Bounding* SQI (baris 22-25): jika nilai `ir_std` di luar rentang 100-500, sinyal dianggap cacat dan prediksi dibatalkan. Empat fitur yang telah diekstraksi kemudian dimasukkan ke fungsi `rf.predict()` (baris 29) untuk menghasilkan klasifikasi akhir.

---

## 4.5 Dokumentasi Kode

### 4.5.1 Struktur Direktori Proyek

```
skripsi/koding/
├── esp32_bp_inference_classification/      # Firmware inferensi (produksi)
│   ├── esp32_bp_inference_classification.ino
│   └── rf_model.h                          # Model Random Forest (C++)
├── raw_data_pipeline/                      # Pipeline pengolahan data
│   ├── esp32_raw_streamer/                 # Firmware akuisisi data
│   │   └── esp32_raw_streamer.ino
│   ├── python_tools/                       # Skrip Python
│   │   ├── raw_data_logger.py              # Penangkap data serial
│   │   ├── extract_features.py             # Ekstraksi fitur + SQA
│   │   ├── train_rf_tinyml.py              # Pelatihan + konversi RF
│   │   └── train_compare.py                # Komparasi RF vs SVM
│   └── dataset/                            # Direktori data mentah + fitur
│       ├── raw_data_*.csv                  # File data mentah per sesi
│       └── extracted_features.csv          # Hasil ekstraksi fitur
└── catatan penting/                        # Dokumentasi analisis
```

### 4.5.2 Konfigurasi dan Konstanta Penting

| Konstanta | Nilai | Lokasi | Keterangan |
|:----------|:-----:|:------:|:-----------|
| SAMPLE_RATE | 100 Hz | ESP32 + Python | Frekuensi sampling sensor |
| SAMPLE_WINDOW | 1000 | ESP32 + Python | Jumlah sampel per jendela (10 detik) |
| Bandpass Low | 0.5 Hz | ESP32 + Python | Batas bawah frekuensi |
| Bandpass High | 5.0 Hz | ESP32 + Python | Batas atas frekuensi |
| Filter Order | 2 | ESP32 + Python | Orde filter Butterworth |
| Time CV Limit | 15% | Python | Ambang batas SQA jitter waktu |
| Amp CV Limit | 25% | Python | Ambang batas SQA amplitudo |
| IR Std Min | 100 | ESP32 | Batas bawah Amplitude Bounding |
| IR Std Max | 500 | ESP32 | Batas atas Amplitude Bounding |
| Finger Threshold | 50.000 | ESP32 | Ambang deteksi keberadaan jari |
| n_estimators | 50 | Python | Jumlah pohon keputusan Random Forest |
| max_depth | 5 | Python | Kedalaman maksimum pohon |

### 4.5.3 Daftar File Program

| No | Nama File | Bahasa | Fungsi |
|:--:|:----------|:------:|:-------|
| 1 | `esp32_raw_streamer.ino` | C++ | Firmware untuk *streaming* data mentah ke komputer |
| 2 | `raw_data_logger.py` | Python | Menangkap data serial dan menyimpan ke CSV |
| 3 | `extract_features.py` | Python | Pra-pemrosesan sinyal, SQA, dan ekstraksi fitur |
| 4 | `train_rf_tinyml.py` | Python | Pelatihan Random Forest dan konversi ke C++ |
| 5 | `train_compare.py` | Python | Studi komparasi Random Forest vs SVM |
| 6 | `esp32_bp_inference_classification.ino` | C++ | Firmware inferensi pada ESP32 |
| 7 | `rf_model.h` | C++ | File header model Random Forest hasil konversi |
