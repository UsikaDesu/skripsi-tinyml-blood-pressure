# BAB IV IMPLEMENTASI (VERSI 2 - FISIOLOGIS)

*Catatan: Ini adalah draf revisi Bab 4 yang mengacu pada metode V2 (AC/DC Ratio, Systolic Rise Time, Heart Rate, & Subject-Wise Split).*

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

Nilai sistolik dan diastolik disimpan pada setiap baris data agar pelabelan melekat langsung pada sinyal mentah. Penamaan file menggunakan format otomatis yang menyertakan identitas sesi pengukuran dan nilai tekanan darah untuk memudahkan proses *Subject-Wise Split* di tahap selanjutnya.

---

## 4.2 Implementasi Pra-pemrosesan Sinyal

### 4.2.1 Segmentasi dan Detrending

Skrip `extract_features_v2.py` memproses seluruh file data mentah secara bertahap. Setiap file rekaman dipotong menjadi jendela-jendela berdurasi 10 detik (1000 sampel pada 100 Hz). Gambar 4.2 menunjukkan proses segmentasi dan *detrending*.

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

Setiap jendela berisi 1000 titik sampel (baris 1). Fungsi `signal.detrend()` dari pustaka SciPy (baris 12-13) menghitung garis tren linier (y = mx + c) dari seluruh data dalam jendela, lalu mengurangkannya dari sinyal asli untuk menghilangkan *baseline drift* akibat perubahan napas.

### 4.2.2 Bandpass Filter dan Zero-Phase Filtering

Sinyal yang telah di-*detrend* kemudian disaring menggunakan *IIR Butterworth Band-Pass Filter* orde-2 dengan frekuensi *cutoff* 0.5 Hz - 5.0 Hz. Filter diimplementasikan menggunakan metode *zero-phase filtering* (filtfilt). 

```python
 1. nyq = 0.5 * SAMPLING_RATE_HZ
 2. b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
 3. ir_filtered = signal.filtfilt(b, a, ir_detrended)
 4. red_filtered = signal.filtfilt(b, a, red_detrended)
```
Gambar 4.3 Implementasi Butterworth Bandpass Filter dengan FiltFilt

Koefisien filter dihitung menggunakan `signal.butter()` (baris 2). Fungsi `signal.filtfilt()` (baris 3-4) menerapkan filter secara dua arah (maju dan mundur) sehingga tidak terjadi pergeseran fase (*phase delay*). Hal ini **sangat esensial** pada metode V2 ini karena perhitungan morfologi seperti *Systolic Rise Time* membutuhkan posisi waktu lembah dan puncak gelombang yang mutlak akurat.

### 4.2.3 Signal Quality Assessment (SQA) Ekstraksi

Setiap jendela data dievaluasi kualitasnya menggunakan dua buah gerbang *Signal Quality Index* (SQI) berbasis fluktuasi (*Coefficient of Variation*).

```python
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
Gambar 4.4 Implementasi Signal Quality Assessment (SQA) Ekstraksi

Puncak gelombang PPG dideteksi (baris 1-2). Gerbang pertama (*Gate 1*, baris 7-13) menolak sinyal dengan fluktuasi irama detak (Time CV) melebihi 15% (mengindikasikan *motion artifact* goyangan). Gerbang kedua (*Gate 2*, baris 15-21) menolak sinyal dengan fluktuasi tinggi amplitudo (Amp CV) melebihi 25% (mengindikasikan perubahan tekanan jari).

### 4.2.4 Ekstraksi Fitur Fisiologis (V2)

Dari setiap jendela yang lolos SQA, diekstraksi empat fitur fisiologis yang kokoh terhadap variasi ketebalan kulit dan warna jari pasien. Gambar 4.5 menunjukkan proses perhitungan morfologi dan rasio.

```python
 1. # 1. Systolic Rise Time (Waktu tempuh dari lembah ke puncak)
 2. rise_times = []
 3. for p in peaks:
 4.     start_search = max(0, p - 45)
 5.     if p > start_search:
 6.         trough_idx = start_search + np.argmin(ir_filtered[start_search:p])
 7.         rise_times.append((p - trough_idx) * 10.0) # ms
 8. mean_rise_time = np.mean(rise_times)
 9. 
10. # 2. Heart Rate (BPM)
11. heart_rate = 60.0 / (mean_rr / SAMPLING_RATE_HZ)
12. 
13. # 3. AC/DC Ratios (Perfusion Index)
14. ir_ratio = (np.std(ir_filtered) / np.mean(ir_raw)) * 100.0
15. red_ratio = (np.std(red_filtered) / np.mean(red_raw)) * 100.0
```
Gambar 4.5 Ekstraksi Fitur Fisiologis V2

Pencarian waktu *Systolic Rise Time* (baris 1-8) dilakukan dengan merunut balik hingga 45 sampel (450 ms) dari setiap puncak untuk menemukan titik lembah terendah (awal detak/onset), lalu jaraknya dikalikan 10 ms. *Heart Rate* (baris 10-11) dihitung berdasarkan interval puncak. Sedangkan rasio komponen AC/DC (baris 13-15) dihitung untuk menormalisasi amplitudo, sehingga perbedaan daya serap cahaya pada jari tebal dan tipis saling meniadakan.

---

## 4.3 Implementasi Pelatihan Model

### 4.3.1 Pelatihan Random Forest & Validasi Subject-Wise

Pelatihan model dilakukan menggunakan skrip `train_rf_tinyml_v2.py`. Tidak seperti metode konvensional (record-wise split), penelitian ini menerapkan **Subject-Wise Split** menggunakan `GroupShuffleSplit` untuk memastikan validitas klinis.

```python
 1. from sklearn.model_selection import GroupShuffleSplit, GroupKFold
 2. 
 3. FEATURES = ['ir_ratio', 'red_ratio', 'rise_time_ms', 'heart_rate']
 4. 
 5. # Mencegah Kebocoran Data (Data Leakage) antar pasien
 6. gss = GroupShuffleSplit(n_splits=1, test_size=0.2, random_state=42)
 7. train_idx, test_idx = next(gss.split(X, y, groups))
 8. 
 9. clf = RandomForestClassifier(
10.     n_estimators=50,
11.     max_depth=5,
12.     min_samples_leaf=2,
13.     class_weight='balanced',
14.     random_state=42
15. )
16. clf.fit(X[train_idx], y[train_idx])
```
Gambar 4.6 Konfigurasi Model dan Split Berbasis Subjek

Metode pembagian data (baris 5-7) menggunakan parameter pengelompokan (*group*) yang diambil dari nama *file* (identitas subjek). Hal ini menjamin bahwa seluruh rekaman jendela waktu dari seorang pasien akan masuk seluruhnya ke himpunan latih (*training*) atau himpunan uji (*testing*), namun tidak akan pernah terbelah ke keduanya, sehingga model teruji menghadapi pasien yang benar-benar asing.

### 4.3.2 Konversi Model ke Kode C++ (TinyML)

Model V2 yang telah dilatih kemudian dikonversi menjadi representasi kode C++ menggunakan `micromlgen`. File header hasil ekspor disimpan dengan nama `rf_model_v2.h` untuk digunakan di proyek firmware ESP32.

---

## 4.4 Implementasi Firmware Inferensi ESP32

### 4.4.1 Implementasi DSP Fisiologis pada ESP32

Karena ESP32 tidak memiliki *library* otomatis seperti SciPy, deteksi fitur fisiologis diimplementasikan secara manual dan efisien dalam bahasa C++ di `esp32_bp_inference_v2.ino`. 

Fungsi *Linear Detrending* (Least Squares) dan *Zero-phase FiltFilt* digunakan persis seperti di Python. Bagian paling krusial adalah algoritma penemuan puncak (*Peak Detection*) yang bekerja tanpa alokasi memori dinamis besar untuk menghindari *stack overflow*.

```cpp
 1. float get_rise_time_and_hr(float* buffer, int length, float std_val, float& out_hr) {
 2.   int peak_indices[100];
 3.   int peak_count = 0;
 4.   float peak_threshold = std_val * 0.5f; // Dynamic prominence
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
16.   // 2. Kalkulasi Rise Time dan Heart Rate...
17. }
```
Gambar 4.7 Algoritma Pencarian Puncak Ringan (*Lightweight Peak Detection*) di C++

Algoritma (baris 7-15) merayapi array untuk mencari nilai puncak lokal (*local maxima*) yang tingginya melebihi ambang batas (*dynamic prominence*) setengah dari standar deviasi, dan memiliki jarak minimal 50 sampel (500 ms) dari puncak sebelumnya. Ini adalah adaptasi *on-chip* langsung dari fungsi `scipy.signal.find_peaks()`.

### 4.4.2 SQA Real-time dan Inferensi 

Berbeda dengan teknik potong amplitudo kaku, alat fisik mengevaluasi kelayakan sinyal berdasarkan kewajaran fisiologis manusia:

```cpp
 1. // Hitung Rasio AC/DC
 2. float irRatio = (irMean > 0) ? ((irStd / irMean) * 100.0f) : 0;
 3. float redRatio = (redMean > 0) ? ((redStd / redMean) * 100.0f) : 0;
 4. 
 5. // SQA Berbasis Fisiologis
 6. if (heartRate < 40.0f || heartRate > 180.0f || riseTimeMs <= 0.0f) {
 7.   oledMessage(">> ERROR <<", "", " Sinyal Rusak!", " Coba Lagi...");
 8.   return; 
 9. }
10. 
11. // Inferensi Random Forest V2
12. float features[4] = {irRatio, redRatio, riseTimeMs, heartRate};
13. int prediction = rf.predict(features);
```
Gambar 4.8 Ekstraksi Fitur V2 dan SQA Fisiologis di ESP32

Sistem memastikan sinyal tidak cacat dengan memeriksa bahwa *Heart Rate* (BPM) yang terhitung berada di batas kehidupan normal (40 hingga 180 BPM) (baris 6). Jika hasil algoritma peak detection memuntahkan angka di luar ini (akibat sensor bergeser atau bergetar), pengukuran akan diabaikan. Jika lolos, empat fitur dimasukkan ke fungsi `rf.predict()` (baris 13) untuk klasifikasi tekanan darah (Normal vs Hipertensi).

---

## 4.5 Dokumentasi Kode (Versi 2)

### 4.5.1 Struktur Direktori Proyek Utama (V2)

```
skripsi/koding/
├── raw_data_pipeline_v2/                   # Pipeline pengolahan data V2
│   ├── esp32_bp_inference_v2/              # Firmware inferensi (alat)
│   │   ├── esp32_bp_inference_v2.ino
│   │   └── rf_model_v2.h                   # Model Random Forest V2
│   ├── python_tools/                       # Skrip Python V2
│   │   ├── extract_features_v2.py          # Ekstraksi AC/DC & Waktu
│   │   └── train_rf_tinyml_v2.py           # Pelatihan Subject-wise RF
│   └── dataset/                            # Output dataset baru
│       └── extracted_features_v2.csv       # Hasil fitur V2
```

### 4.5.2 Konfigurasi dan Konstanta Penting (V2)

| Konstanta | Nilai | Keterangan |
|:----------|:-----:|:-----------|
| SAMPLE_RATE | 100 Hz | Frekuensi sampling sensor (ESP32) |
| SAMPLE_WINDOW | 1000 | Jumlah sampel per jendela inferensi (10 detik) |
| Bandpass Low-High | 0.5 - 5.0 Hz | Ambang IIR Butterworth Filter (0.5 Hz - 5.0 Hz) |
| Min Peak Distance | 50 sampel | Toleransi jarak detak jantung maksimal setara 240 BPM |
| Valid HR Range | 40 - 180 BPM | SQA (Physiological Gate) di ESP32 |
| n_estimators | 50 | Jumlah pohon Random Forest |
| Split Method | GroupShuffleSplit | Membagi data latih/uji murni berdasarkan pasien unik |
