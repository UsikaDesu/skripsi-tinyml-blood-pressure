# Alur Lengkap & Metodologi Skripsi: Sistem Klasifikasi Tekanan Darah Berbasis TinyML

Dokumen ini merangkum seluruh langkah kerja (*pipeline*) dan metode ilmiah yang kita bangun dalam sistem estimasi tekanan darah (Hipertensi, Normal, Hipotensi) menggunakan sensor optik (PPG) MAX30102 dan ESP32. Ini akan sangat membantumu untuk menyusun Bab 3 (Metodologi Penelitian) skripsimu!

---

## TAHAP 1: Perekaman Data Primer (*Raw Data Acquisition*)
Pada tahap ini, perangkat keras bertugas sebagai pencatat murni untuk menjaring kumpulan data mentah awal.

*   **Peralatan:** Mikrokontroler ESP32, Sensor Oksimetri MAX30102, Tensimeter Lengan (sebagai label *Ground Truth/Referensi*).
*   **Metodologi:** 
    *   Pengambilan sampel frekuensi tinggi: Komunikasi I2C di- *setting* agar membaca nilai saturasi Inframerah (`ir_raw`) dan Cahaya Merah (`red_raw`) dengan waktu jeda statis **10 milidetik (Frekuensi 100 Hz)**.
    *   Waktu rekam dipatok selama **~25 detik** per pasien agar mencakup variasi kardiovaskuler yang cukup.
*   **Output:** File *.csv* (Contoh: `raw_data_2026xxxx.csv`) berisi ribuan baris sinyal cahaya pantulan pembuluh warna IR dan RED, dilengkapi angka *systolic* dan *diastolic* referensi.

---

## TAHAP 2: Pra-pemprosesan & Penyeleksian Kualitas (*Preprocessing & QA*)
Ini adalah tulang punggung akurasi data. Semua skrip berjalan secara otomatis (`extract_features.py`) pada PC lokal.

*   **Metodologi 1: Segmentasi (*Data Windowing*)**
    *   File berdurasi 25 detik tidak digiling sekaligus, namun "digunting" presisi menjadi jendelan **10-Detik (1000 titik sampel)**.  Ini bertujuan agar AI dapat mendiagnosa dengan durasi yang sama (10-Detik) pada produk final kelaknya.
*   **Metodologi 2: Filter Pemerataan (*Linear Detrending*)**
    *   Fungsi regresi linier digunakan untuk meluruskan sinyal (*baseline wander removal*). Ini menghilangkan kemiringan fatal akibat daya serap otot dan tarikan napas kasar agar poros grafik berada murni di titik tegak sumbu Y = 0.
*   **Metodologi 3: Filter Kemurnian (*Band-Pass Butterworth Filter*)**
    *   Sinyal dilewatkan ke saringan dua tahap *(Infinite Impulse Response/IIR)* orde-2 pada frekuensi potong **0.5 Hz hingga 5.0 Hz**. 
    *   *Tujuannya:* Secara eksklusif hanya membiarkan denyut frekuensi jantung asli yang lolos (setara 30 hingga 300 BPM), sementara efek silau cahaya (frekuensi tinggi) dan gelombang rongga dada (frekuensi lambat) dibakar habis.
*   **Metodologi 4: Gerbang Anti Jitter (*12% CV Quality Assurance*)**
    *   Satpam kualitas mencari puncak-puncak nadi menggunakan algoritma *Peak Detection*. Kemudian, dicari Koefisien Toleransi (*Coefficient of Variation / CV*) ketidaksinambungan antar puncak nadi. 
    *   Jika CV > 12%, program melabeli jendela 10-Detik tersebut sebagai *Cacat/Tremor Jari* dan **membuangnya** ke tempat sampah. Hanya jendela beraliran mulus <12%  yang terselamatkan.

---

## TAHAP 3: Ekstraksi Fitur Pembelajaran Mesin (*Feature Extraction*)
Tujuan tahap ini adalah menyusutkan 1000 baris gelombang (di satu jendela) menjadi **4 Angka Pilar** saja, untuk menghemat memori ESP32 di masa tempur nanti.

*   **Pemilihan Fitur:**
    1.  **`ir_mean` & `red_mean`:** Diambil dari sinyal mentah murni (*Raw Data*). Menyimbolkan Komponen Listrik Statis (DC), atau cerminan dari total ketebalan genangan darah di ujung jari (Resistensi Perifer Sistematik).
    2.  **`ir_std` & `red_std`:** Diambil dari Sinyal Murni (*Bandpass Filtered Data*). Menyimbolkan Komponen Listrik Dinamis (AC). Angka *Standard Deviation* beresolusi medis ini berkorelasi kental dengan kekuatan hantaman letupan dinding pembuluh nadi (Amplitudo Tekanan Nadi).
*   **Output:** `extracted_features.csv` (100 baris Tabel Fitur siap latih).

---

## TAHAP 4: Pelatihan Otak AI PC $\rightarrow$ Mikrokontroler (*Model Training & Porting*)

*   **Metodologi Mesin Pembelajar (Studi Komparasi):** Berkas hasil ekstraksi dimasukkan ke dalam dua kandidat Model Machine Learning terbaik untuk kasus ini: **Random Forest Classifier** dan **Support Vector Machine (SVM)**. 
    *   *Random Forest* diunggulkan karena model hierarki keputusannya (`IF-ELSE`) sangat ringan bagi *Compute Power* dan tahan banting (*Robust*) terhadap noise.
    *   *SVM* diikutsertakan sebagai pembanding utama karena teknik "pembelah batas margin"-nya (*hyperplane separation*) sangat jitu mengklasifikasi dataset kecil di ruang dimensi yang kompleks.
    *   Kinerja dan beban ruang penyimpanan kedua model ini nantinya akan ditandingkan untuk menentukan pemenang final yang layak masuk ke mikrokontroler.
*   **Transplantasi C++ (*TinyML Porting*)**
    *   Model dilatih di atas kerangka *scikit-learn* (bahasa Python). 
    *   Menggunakan *library* `micromlgen`, "otak" model cerdik yang sudah siap tempur dicairkan dan dibakukan ulang (*Hardcoded*) menjadi bahasa ibu ESP32 (yaitu bahasa komputasi C++ `if/else`).
*   **Output Final:** File `rf_model.h` (Serangkaian logika biner pohon Acak) atau `svm_model.h` (Sebaran Bobot Array SVM) yang kelak diuji tanding kemampuannya.

---

## TAHAP 5: Implementasi Final & Uji Coba Luring (*Standalone Inferencing*)
Mikrokontroler ESP32 dihubungkan kembali dengan Sensor MAX30102 serta ditambah Modul Layar OLED I2C dan dicabut selamanya dari Komputer (Dicolok ke baterai eksternal).

*   **Cara Kerja Kode Firmware (Arsitektur Pengetesan Penuh):**
    1.  **Observasi:** Pasien memasukkan jari. ESP32 secara otonom "menabung" data cahaya dengan kapasitas `array limit` sebesar 1000 titik (Tepat selama 10 Detik Penuh).
    2.  **Penggilingan Mikro:** Memori RAM dan Prosesor `FPU` ganda dalam inti ESP32 memproses 1000 angka array menggunakan rumus *Linear Regresion (Detrend)* dan rumus matriks *IIR Biquadratic Filter (Bandpass)* buatan tangan seketika (Memerlukan $< 1$ Milidetik waktu pemrosesan).
    3.  **Klimaks (*Inference*):** Angka fitur turunan akhir diekstrak dan disuntikkan ke dalam panggilan fungsi skrip bawaan `rf_model.predict()`.
    4.  **Putusan Final:** Layar OLED memecahkan keheningan, memunculkan deteksi konstan diagnostik kelaskelas medis: *"HIPERTENSI", "NORMAL",* atau *"HIPOTENSI"*. Jeda ulang, dan bersiap memindai orang selanjutnya.

---

## TAHAP 6: Parameter Pengetesan & Evaluasi Hasil (*Evaluation Metrics*)
Untuk mensahkan kelayakan alat ini sebagai instrumen kesehatan, pengujian dibagi menjadi 3 pilar pengukuran ketat:

### 1. Uji Validasi Model di Komputer (*Software Metrics*)
*   *Akurasi, Presisi, Recall, & F1-Score:* Menilai kedisiplinan model (RF & SVM) dalam memetakan tiap kelas tanpa tertukar.
*   *Confusion Matrix:* Tabel silang yang memvisualisasikan jumlah data yang sukses diprediksi vs data yang salah sasaran (misal: "Berapa kali data Normal disangka Hipertensi").
*   *Cross-Validation:* Pengujian lipat-menyilang membuktikan bahwa akurasi tinggi didapat dari kecerdasan sejati, bukan karena model menghafal bocoran data (*overfitting*).

### 2. Uji Kelayakan Hardwre Mikrokontroler (*Hardware/TinyML Metrics*)
*   *Waktu Eksekusi (Inference Time):* Mengukur berapa mikro-detik yang dibutuhkan ESP32 untuk menentukan hipertensi sejak data jari rampung dikumpulkan.
*   *Beban Memori (Memory Footprint):* Menghitung sisa penyimpanan *Flash* dan SRAM yang tumpah akibat membedakan algoritma *Random Forest* versus algoritma *SVM*.

### 3. Uji Coba Fisik Klinis Luring (*Real-world Clinical Testing*)
Membandingkan output Teks OLED yang keluar pada prototipe gelang ESP32-mu secara mendadak pada beberapa orang, yang langsung dibandingkan seketika *(head-to-head)* dengan angka hasil lilitan tensimeter konvensional digital. Persentase ketepatannya (Tingkat Sukses Prediksi Teks Oled vs Klasifikasi Tensimeter Konvensional) dibukukan sebagai Bukti Konkret Uji Lapangan.
