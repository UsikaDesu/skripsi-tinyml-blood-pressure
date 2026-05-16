# Cara Kerja Matematika Detrending (Menemukan Titik Nol)

Fungsi `scipy.signal.detrend` menggunakan metode matematika bernama **Linear Least Squares Regression** (Regresi Kuadrat Terkecil Linier). 

Intinya, ia mencari sebuah "Garis Lurus Terbaik" (`y = mx + c`) yang membelah sinyal mentah tepat di tengah-tengah pusat gravitasinya.

Perhatikan visualisasi berikut:

![Matematika Detrending](/C:/Users/Kayyis/.gemini/antigravity/brain/fd811ece-1af2-4dfc-ae91-b0155d9bc9ec/detrend_math.png)

### Penjelasan Grafik 1: Sinyal Mentah & Garis Merah
*   Garis biru adalah **Sinyal Mentah** milikmu.
*   Garis putus-putus merah adalah **Garis Lurus Terbaik** (Trend Line) yang dihitung oleh komputer. Di gambar ini, rumusnya adalah `y = 12.39x + 107466`.
*   Garis merah ini adalah "Rata-rata Bergerak" yang ditarik lurus. Kamu bisa melihat bahwa garis merah membelah tepat di tengah ombak biru.

### Penjelasan Grafik 2: Proses Menjadi Nol
Bagaimana mendapatkan grafik kedua? Rumusnya hanyalah:
**`Sinyal Baru = Sinyal Biru - Garis Merah`**

*   Saat Sinyal Biru berada di *atas* Garis Merah (contoh di Index paling awal atau paling akhir), hasil pengurangannya menjadi **Positif** (Berada di atas Titik 0).
*   Saat Sinyal Biru merosot ke *bawah* Garis Merah (contoh di Index 400), hasil pengurangannya menjadi **Negatif** (Berada di bawah Titik 0).
*   **Garis Merah yang awalnya miring, kini didatarkan menjadi sumbu horizontal persis di angka 0.**

Jadi, Titik 0 pada detrending **bukan diambil dari nilai Mean konvensional**, melainkan diambil dari **Garis Regresi Linear** yang dipaksa mendatar menjadi sumbu X (angka 0).
