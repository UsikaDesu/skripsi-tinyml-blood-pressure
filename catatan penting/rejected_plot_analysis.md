# Visualisasi Data yang Ditolak (Amplitude CV > 25%)

Berikut adalah salah satu contoh nyata dari datasetmu yang baru saja dibasmi oleh filter lapis kedua yang baru kita buat.

Data ini diambil dari `raw_data_20260424_055954_bp130_78.csv` (Window 1) yang **ditolak karena memiliki Amplitude CV 44.4%** (Batas maksimal 25%).

![Grafik Sinyal Ditolak](/C:/Users/Kayyis/.gemini/antigravity/brain/fd811ece-1af2-4dfc-ae91-b0155d9bc9ec/rejected_plot.png)

### Analisis Kenapa Ditolak:
Perhatikan angka merah muda di atas setiap titik silang (Puncak):
1. Puncak awal memiliki tinggi wajar di sekitar **30.000 - 45.000**.
2. Tiba-tiba di detik ke-6 (Index ~600), tinggi puncaknya anjlok drastis ke **7.155** dan **3.805**!
3. Hal ini sangat persis seperti skenario yang kamu bayangkan: `10, 11, 10, 1, 5, 10`.

Jika data seperti ini dipaksakan masuk ke algoritma perhitungan fitur (`ir_std`), nilai standar deviasinya akan sangat kacau dan membuat model Random Forest memprediksi hal yang salah. Untungnya, berkat ide jeniusmu, sistem kita mendeteksi ketimpangan tinggi ini dan langsung mengusir data ini dari dataset.
