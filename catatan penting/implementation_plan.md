# Rencana Implementasi: Filter Sinyal Digital C++ di ESP32

## Latar Belakang
Model Random Forest (*TinyML*) yang dilatih di Python mengekstrak fitur Standar Deviasi (`std`) dari data yang telah melalui proses **Detrending** dan **Bandpass Filter (0.5 - 5 Hz)**. 
Saat ini, ESP32 mengekstrak nilai `std` secara langsung dari data mentah (*Raw Data*), yang menyebabkan nilai fiturnya akan ribuan kali lebih besar dari yang diharapkan oleh model, sehingga akurasi di ESP32 akan hancur.

## Tujuan
Kita harus menanamkan kemampuan pemrosesan sinyal digital (*Digital Signal Processing / DSP*) langsung ke dalam bahasa pemrograman C++ di ESP32 agar fiturnya 100% identik dengan hasil ekstraksi Python.

---

> [!IMPORTANT]  
> **User Review Required**  
> Penambahan fungsi matematika ini akan membutuhkan sedikit tambahan *RAM* (untuk *array buffer* sementara saat proses *reverse filtering*) dan waktu komputasi sekitar beberapa milidetik di ESP32. Mengingat ESP32 memiliki prosesor 240MHz, ini seharusnya sangat aman. Apakah kamu setuju untuk melanjutkan?

---

## Perubahan yang Diusulkan

### `esp32_bp_inference_classification.ino`

Kita akan menambahkan dua fungsi matematika baru ke dalam *firmware* ESP32:

#### 1. Fungsi `detrend_signal(float* buffer, int length)`
Fungsi ini akan meniru `scipy.signal.detrend`.
*   Menghitung *Linear Least Squares Regression* dari 1.000 titik data.
*   Menemukan garis `y = mx + c`.
*   Mengurangi setiap titik data dengan garis tersebut sehingga titik nol-nya berada tepat di tengah (rata).

#### 2. Fungsi `filtfilt_bandpass(float* buffer, int length)`
Fungsi ini akan meniru `scipy.signal.filtfilt(b, a, data)`.
*   Kita menggunakan koefisien *Butterworth Bandpass Filter* Orde 2 (0.5 Hz - 5 Hz, Nyquist 50 Hz) yang saya ekstrak langsung dari Python:
    *   **b (Numerator):** `{0.01658, 0.0, -0.03316, 0.0, 0.01658}`
    *   **a (Denominator):** `{1.0, -3.58623, 4.84628, -2.93042, 0.67045}`
*   Fungsi ini akan melakukan *forward filtering* (maju dari index 0 ke 999), lalu membalik datanya dan melakukan *backward filtering* (maju dari index 999 ke 0) untuk menghasilkan efek *Zero-Phase Filter* persis seperti `filtfilt`.

#### 3. Modifikasi Loop Utama (Fitur Ekstraksi)
*   **Baris 224-225:** Menghitung `irMean` dan `redMean` dari data **MENTAH**.
*   **[BARU]:** Menjalankan `detrend` dan `filtfilt` pada kedua *buffer*.
*   **Baris 227-233:** Menghitung `irStd` dan `redStd` dari data yang sudah **BERSIH/DIFILTER**.
*   Memasukkan ke-4 fitur tersebut ke Model Random Forest.

## Rencana Verifikasi
Setelah kode ditulis:
1.  Saya akan memastikan sintaks C++ aman dan tidak memakan terlalu banyak memori (*Stack overflow*).
2.  Kamu dapat langsung mengunggahnya (*upload*) ke ESP32 dan melihat hasil cetakan `Serial.print`. Angka `ir_std` yang dicetak di Serial Monitor seharusnya tidak lagi berada di kisaran ribuan (1000-3000), melainkan akan turun ke kisaran ratusan (**200-500**) seperti saat kamu memproses datanya di Python!
