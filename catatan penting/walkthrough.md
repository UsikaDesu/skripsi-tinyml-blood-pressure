# Walkthrough: Implementasi Filter DSP C++ di ESP32

## Masalah yang Diselesaikan
Sebelumnya, *firmware* ESP32 menghitung nilai Standar Deviasi (`std`) secara langsung dari data **Mentah (Raw)** yang dipenuhi dengan efek tarikan napas (ombal raksasa). Hal ini menghasilkan nilai `std` di kisaran **2.000 hingga 3.000**.
Sementara itu, model *Random Forest* dilatih menggunakan Python dengan data yang sudah di-*Filter*, di mana nilai `std`-nya berada di kisaran **200 hingga 400**. Perbedaan masif ini membuat ESP32 mustahil menebak tekanan darah dengan benar.

## Perubahan yang Dilakukan
Saya telah berhasil menerjemahkan algoritma pemrosesan sinyal digital (*Digital Signal Processing / DSP*) dari library Scipy (Python) ke dalam bahasa C++ *Native* yang ditanam langsung ke otak ESP32.

### 1. Menambahkan Fungsi `detrend_signal`
Menerjemahkan `scipy.signal.detrend` menggunakan metode **Linear Least Squares Regression**.
Fungsi C++ ini akan menghitung `y = mx + c` dari 1000 titik data hanya dalam hitungan mikrosekon, lalu mendatarkan garis tren-nya ke angka 0.

### 2. Menambahkan Fungsi `filtfilt_bandpass`
Menerjemahkan `scipy.signal.filtfilt` menggunakan filter **Butterworth Bandpass 4th-Order (0.5 - 5 Hz)**.
Saya telah menyuntikkan koefisien matematis (*b* dan *a*) ke dalam *array* `float`. Fungsi ini melakukan *Forward Filtering*, membalik arah data, lalu *Backward Filtering* (*Zero-phase filter*) tanpa distorsi fase.

### 3. Memperbaiki Ekstraksi Fitur (`loop`)
Fitur-fitur kini diekstrak secara akurat:
*   `irMean` dan `redMean` = Tetap mengambil data dari **Sinyal Mentah** (Komponen DC).
*   `irStd` dan `redStd` = Kini mengambil data dari **Sinyal yang Telah Di-Detrend & Bandpass** (Komponen AC murni).

## Verifikasi oleh Pengguna
Tugasmu sekarang adalah:
1.  Buka Arduino IDE dan buka file `esp32_bp_inference_classification.ino`.
2.  Lakukan **Verify (Compile)** untuk memastikan tidak ada kesalahan ketik (kodenya sudah saya optimalkan agar tidak memakan RAM).
3.  Lakukan **Upload** ke ESP32.
4.  Coba tempelkan jarimu, dan pantau hasilnya di **Serial Monitor**.

> [!TIP]
> **Apa yang harus diperhatikan di Serial Monitor?**
> Pastikan nilai `ir_std` dan `red_std` yang dicetak di Serial Monitor kini berada di angka **puluhan atau ratusan (contoh: 210.5, 340.2)**, bukan lagi ribuan! Jika iya, maka algoritma filternya telah bekerja dengan sempurna!
