# BAB IV IMPLEMENTASI (VERSI 3 - FINAL)

*Catatan: Ini adalah draf revisi Bab 4 yang mengacu pada metode V3 (AC/DC Ratio, Crest Time Ratio, Heart Rate, & Stratified Subject-Independent Split).*

## 4.1 Implementasi Akuisisi Data

### 4.1.1 Firmware Akuisisi Data (ESP32)

Pada tahap akuisisi data, firmware `esp32_raw_streamer.ino` ditanamkan pada mikrokontroler ESP32. Firmware ini bertugas membaca data mentah dari sensor MAX30102 secara kontinu pada frekuensi sampling 100 Hz, kemudian mengirimkan data tersebut melalui komunikasi serial ke komputer dalam format `timestamp,IR,RED`.

Konfigurasi sensor MAX30102 yang digunakan pada tahap akuisisi meliputi pengaturan kecerahan LED (*LED Brightness*) sebesar `0x1F`, jumlah *sample average* sebanyak 1, mode LED ganda (*Red + IR*), laju sampling 100 Hz, lebar pulsa 411 μs, dan rentang ADC 4096 (resolusi penuh). Konfigurasi ini dirancang agar sinyal PPG yang dihasilkan memiliki resolusi dan respons seketika (*zero delay*) yang optimal untuk analisis morfologi gelombang.

### 4.1.2 Skrip Penangkap Data (Python)

Di sisi komputer, skrip Python `raw_data_logger.py` menangkap aliran data serial dari ESP32 dan menyimpannya ke dalam file CSV. Gambar 4.1 menunjukkan fungsi utama dari skrip tersebut.

```python
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

Setelah perekaman selesai, sistem meminta pengguna memasukkan nilai tekanan darah dari tensimeter digital sebagai data referensi (*ground truth*). 

Nilai sistolik dan diastolik disimpan pada setiap baris data agar pelabelan melekat langsung pada sinyal mentah. Penamaan file menggunakan format otomatis yang menyertakan identitas sesi pengukuran dan nilai tekanan darah untuk memudahkan proses pemisahan kelompok (*Stratified Group Split*) di tahap selanjutnya.

---

## 4.2 Implementasi Pra-pemrosesan Sinyal

### 4.2.1 Segmentasi dan Detrending

Skrip `extract_features_v3.py` memproses seluruh file data mentah secara bertahap. Setiap file rekaman dipotong menjadi jendela-jendela berdurasi 10 detik (1000 sampel pada 100 Hz). Gambar 4.2 menunjukkan proses segmentasi dan *detrending*.

```python
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
Gambar 4.2 Segmentasi Jendela Waktu dan Linear Detrending

Fungsi `signal.detrend()` dari pustaka SciPy (baris 12-13) menghitung garis tren linier dari seluruh data dalam jendela, lalu mengurangkannya dari sinyal asli untuk menghilangkan *baseline drift* akibat pernapasan.

### 4.2.2 Bandpass Filter dan Zero-Phase Filtering

Sinyal kemudian disaring menggunakan *IIR Butterworth Band-Pass Filter* orde-2 dengan frekuensi *cutoff* 0.5 Hz - 5.0 Hz. Filter diimplementasikan menggunakan metode *zero-phase filtering* (filtfilt). 

```python
 1. nyq = 0.5 * SAMPLING_RATE_HZ
 2. b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
 3. ir_filtered = signal.filtfilt(b, a, ir_detrended)
 4. red_filtered = signal.filtfilt(b, a, red_detrended)
```
Gambar 4.3 Implementasi Butterworth Bandpass Filter dengan FiltFilt

Fungsi `signal.filtfilt()` (baris 3-4) menerapkan filter secara dua arah sehingga tidak terjadi pergeseran fase (*phase delay*), hal ini **sangat esensial** karena perhitungan *Crest Time Ratio* membutuhkan posisi mutlak puncak dan lembah gelombang.

### 4.2.3 Signal Quality Assessment (SQA) Ekstraksi

Setiap jendela data dievaluasi kualitasnya menggunakan dua buah gerbang *Signal Quality Index* (SQI) berbasis fluktuasi (*Coefficient of Variation*).

```python
 1. peaks, _ = signal.find_peaks(ir_filtered, distance=50,
 2.                              prominence=np.std(ir_filtered)*0.5)
 3.  
 4. if len(peaks) < 2:
 5.     continue  # Flatline
 6.  
 7. # Gate 1: Time CV (konsistensi jarak antar-puncak)
 8. rr_intervals = np.diff(peaks)
 9. mean_rr = np.mean(rr_intervals)
10. time_cv = (np.std(rr_intervals) / mean_rr) * 100
11.  
12. if time_cv > 15.0:
13.     continue  # Jitter terdeteksi
14.  
15. # Gate 2: Amplitude CV (konsistensi tinggi puncak)
16. amplitudes = ir_filtered[peaks]
17. mean_amp = np.mean(amplitudes)
18. cv_amp = np.std(amplitudes) / mean_amp
19.  
20. if cv_amp > 0.25:
21.     continue  # Goyangan vertikal (Pressure Shift)
```
Gambar 4.4 Implementasi Signal Quality Assessment (SQA) Ekstraksi

Gerbang pertama menolak fluktuasi irama detak melebihi 15% (mengindikasikan *motion artifact*), dan gerbang kedua menolak fluktuasi tinggi amplitudo melebihi 25% (mengindikasikan pergeseran tekanan jari).

### 4.2.4 Ekstraksi Fitur Fisiologis (V3)

Dari setiap jendela yang lolos SQA, diekstraksi empat fitur fisiologis utama. Pada versi V3, fitur waktu menggunakan **Crest Time Ratio (CTR)** yang secara fisiologis tidak bergantung pada detak jantung (*Heart Rate Independent*).

```python
 1. # 1. Crest Time Ratio (Rise Time / Pulse Interval)
 2. ratios = []
 3. for i, p in enumerate(peaks):
 4.     start_search = max(0, p - int(0.45 * SAMPLING_RATE_HZ))
 5.     if p > start_search:
 6.         trough_idx = start_search + np.argmin(ir_filtered[start_search:p])
 7.         rise_time_ms = (p - trough_idx) * (1000.0 / SAMPLING_RATE_HZ)
 8.         if i > 0:
 9.             pulse_interval_ms = (p - peaks[i-1]) * (1000.0 / SAMPLING_RATE_HZ)
10.             ratios.append(rise_time_ms / pulse_interval_ms)
11. mean_crest_time_ratio = np.mean(ratios)
12. 
13. # 2. Heart Rate (BPM)
14. heart_rate = 60.0 / (mean_rr / SAMPLING_RATE_HZ)
15. 
16. # 3. AC/DC Ratios (Perfusion Index)
17. ir_ratio = (np.std(ir_filtered) / np.mean(ir_raw)) * 100.0
18. red_ratio = (np.std(red_filtered) / np.mean(red_raw)) * 100.0
```
Gambar 4.5 Ekstraksi Fitur Fisiologis V3

Perhitungan *Crest Time Ratio* (baris 1-11) dilakukan dengan membagi durasi dari lembah ke puncak (*Rise Time*) terhadap durasi gelombang secara keseluruhan (*Pulse Interval*). Ini menghasilkan parameter rasio yang berkorelasi langsung dengan kekakuan dinding pembuluh darah, tanpa terdistorsi oleh laju napas atau kelelahan pasien.

### 4.2.5 Penyeimbangan Data (Undersampling dan Filter Minoritas)

Dataset kelas medis rentan terhadap bias mayoritas. Oleh karena itu, diterapkan logika pembersihan *outlier* kelas dan *undersampling* sebelum data dimasukkan ke algoritma pembelajaran mesin.
Sistem secara otomatis memeriksa riwayat rekam medis setiap pasien. Jika terdapat data anomali (misalnya tekanan darah turun drastis di satu sesi rekaman sehingga kelasnya berbeda dengan kelas mayoritas pasien tersebut), data minoritas tersebut dibuang agar tidak merusak representasi pasien tersebut di mata mesin. Selanjutnya, jumlah maksimal jendela per subjek dibatasi (maksimal 10 jendela) menggunakan algoritma *Round-Robin* untuk memastikan data seimbang.

---

## 4.3 Implementasi Pelatihan Model

### 4.3.1 Pelatihan SVM & Validasi Stratified Subject-Independent

Pengujian algoritma menggunakan algoritma *Support Vector Machine* (SVM) dengan validasi silang `StratifiedGroupKFold`. Pemisahan *Stratified Group* menjamin dua hal: Subjek tidak bocor antara data latih dan uji, serta rasio subjek Hipertensi dan Normal dipastikan selalu setara (50:50) di setiap lipatan pelatihan.

```python
 1. from sklearn.svm import SVC
 2. from sklearn.model_selection import StratifiedGroupKFold, cross_val_score
 3. from sklearn.pipeline import Pipeline
 4. from sklearn.preprocessing import StandardScaler
 5. 
 6. FEATURES = ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']
 7. 
 8. # Mencegah Kebocoran Data dan Standardisasi secara aman
 9. clf = SVC(kernel='linear', C=1.0, class_weight='balanced', random_state=42)
10. pipe = Pipeline([
11.     ('scaler', StandardScaler()),
12.     ('svm', clf)
13. ])
14. 
15. sgkf = StratifiedGroupKFold(n_splits=5)
16. scores = cross_val_score(pipe, X_base, y, groups=groups, cv=sgkf, scoring='accuracy')
```
Gambar 4.6 Konfigurasi SVM dan Stratified Split

Metode pembagian data secara *stratified* menghasilkan evaluasi performa model yang sangat objektif dan kebal terhadap ilusi probabilitas data ganda.

### 4.3.2 Konversi Model ke Kode C++ (TinyML)

Model V3 SVM terbaik yang telah dilatih secara penuh kemudian dikonversi menjadi representasi kode C++ menggunakan `micromlgen`. File header hasil ekspor disimpan dengan nama `svm_model_v3.h` beserta parameter konstanta *StandardScaler*-nya untuk digunakan di proyek firmware ESP32.

---

## 4.4 Implementasi Firmware Inferensi ESP32

### 4.4.1 Implementasi DSP Fisiologis pada ESP32

Deteksi fitur fisiologis diimplementasikan secara mandiri dan efisien di ESP32 dalam file `esp32_bp_inference_svm_v3.ino`. Algoritma dirancang dengan pendekatan *stateless* tanpa array dinamis besar.

```cpp
 1. float get_crest_time_and_hr(float* buffer, int length, float std_val, float& out_hr) {
 2.   int peak_indices[100];
 3.   int peak_count = 0;
 4.   float peak_threshold = std_val * 0.5f; 
 5.   
 6.   // 1. Temukan puncak lokal (Systolic Peaks)
 7.   for (int i = 1; i < length - 1; i++) {
 8.     if (buffer[i] > buffer[i-1] && buffer[i] > buffer[i+1]) { 
 9.       if (buffer[i] > peak_threshold && (i - last_peak_idx >= 50)) { 
10.           peak_indices[peak_count++] = i;
11.           last_peak_idx = i;
12.           if (peak_count >= 100) break;
13.       }
14.     }
15.   }
16.   // 2. Kalkulasi Crest Time Ratio berdasarkan titik puncak dan jarak antar-puncak...
17. }
```
Gambar 4.7 Algoritma Pencarian Puncak Ringan (*Lightweight Peak Detection*) di C++

Algoritma (baris 7-15) merayapi array untuk mencari nilai puncak lokal (*local maxima*) yang tingginya melebihi ambang batas adaptif (*dynamic prominence*) setengah dari standar deviasi.

### 4.4.2 SQA Real-time dan Inferensi 

Penyetaraan input sensor (*StandardScaler*) diterapkan pada fitur sebelum dilakukan prediksi oleh model SVM di dalam mikrokontroler.

```cpp
 1. // Parameter StandardScaler dari Python
 2. float mean_ir_ratio = 0.400491f; float std_ir_ratio = 0.191633f;
 3. float mean_red_ratio = 0.176177f; float std_red_ratio = 0.084899f;
 4. float mean_heart_rate = 76.784615f; float std_heart_rate = 7.445168f;
 5. float mean_crest_time_ratio = 0.265718f; float std_crest_time_ratio = 0.023677f;
 6. 
 7. // SQA Fisiologis
 8. if (heartRate < 40.0f || heartRate > 180.0f || crestTimeRatio <= 0.0f) {
 9.   oledMessage(">> ERROR <<", "", " Sinyal Rusak!", " Coba Lagi...");
10.   return; 
11. }
12. 
13. // Z-Score Normalization
14. float features[4] = {
15.   (irRatio - mean_ir_ratio) / std_ir_ratio,
16.   (redRatio - mean_red_ratio) / std_red_ratio,
17.   (crestTimeRatio - mean_crest_time_ratio) / std_crest_time_ratio,
18.   (heartRate - mean_heart_rate) / std_heart_rate
19. };
20. 
21. // Inferensi SVM V3
22. int prediction = svm.predict(features);
```
Gambar 4.8 Ekstraksi Fitur V3, Skala Z-Score, dan SQA Fisiologis di ESP32

Sistem memeriksa bahwa *Heart Rate* dan *Crest Time Ratio* berada pada rentang batas wajar. Jika valid, keempat fitur dinormalisasi (*Z-Score Normalization*) (baris 13-19) lalu dimasukkan ke dalam objek fungsi klasifikasi SVM (baris 22) untuk mendeteksi Hipertensi dengan latensi milidetik.

---

## 4.5 Dokumentasi Kode (Versi 3)

### 4.5.1 Struktur Direktori Proyek Utama (V3)

```
skripsi/koding/
├── raw_data_pipeline_v3/                   # Pipeline pengolahan data V3
│   ├── esp32_bp_inference_svm_v3/          # Firmware inferensi SVM (alat)
│   │   ├── esp32_bp_inference_svm_v3.ino
│   │   └── svm_model_v3.h                  # Model SVM V3 hasil ekspor
│   ├── python_tools/                       # Skrip Python V3
│   │   ├── extract_features_v3.py          # Ekstraksi CTR, HR, IR, RED
│   │   └── optimize_svm_v3.py              # Pelatihan Stratified-Split SVM
│   └── dataset/                            # Output dataset V3 terfilter
│       └── extracted_features_v3.csv       
```

### 4.5.2 Konfigurasi dan Konstanta Penting (V3)

| Konstanta | Nilai | Keterangan |
|:----------|:-----:|:-----------|
| SAMPLE_RATE | 100 Hz | Frekuensi sampling sensor (ESP32) |
| SAMPLE_WINDOW | 1000 | Jumlah sampel per jendela inferensi (10 detik) |
| Bandpass Low-High | 0.5 - 5.0 Hz | Ambang IIR Butterworth Filter (0.5 Hz - 5.0 Hz) |
| Min Peak Distance | 50 sampel | Toleransi jarak detak jantung maksimal setara 240 BPM |
| Valid HR Range | 40 - 180 BPM | SQA (Physiological Gate) di ESP32 |
| Undersampling | Maks 10 windows | Round-robin limit per subjek untuk klasifikasi seimbang |
| Split Method | StratifiedGroupKFold | Membagi latih/uji murni berdasarkan pasien unik (50:50 rasio) |
| SVM Kernel | Linear / RBF | Optimal pada parameter C=1.0 |
