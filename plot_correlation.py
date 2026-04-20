import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import glob
import os

print("Memulai Analisis Korelasi Data Anda...")

# 1. Memuat seluruh data dari folder
csv_files = glob.glob(os.path.join('training_data', '*.csv'))
if not csv_files:
    print("ERROR: Tidak ditemukan file CSV di folder training_data/")
    exit()

df_list = []
for file in csv_files:
    try:
        df_list.append(pd.read_csv(file))
    except Exception as e:
        pass

df_all = pd.concat(df_list, ignore_index=True)

# 2. Membersihkan data (Hanya mengambil yang Valid & Realistis)
df = df_all[df_all['valid'] == 1].copy()
df = df[(df['systolic_bp'] >= 70) & (df['systolic_bp'] <= 200)]  # Filter typo tensi
df = df[(df['heart_rate'] >= 40) & (df['heart_rate'] <= 180)]    # Filter error HR

# 3. Memilih Parameter (Fitur) yang akan dikorelasikan
fitur = ['ir_mean', 'red_mean', 'ir_std', 'red_std', 'heart_rate', 'spo2', 'systolic_bp', 'diastolic_bp']
df_fitur = df[fitur]

# 4. Menghitung Matriks Korelasi (Pearson Correlation)
korelasi = df_fitur.corr()

# 5. Membuat Gambar Grafik Visualisasi (Heatmap)
plt.figure(figsize=(10, 8))
# Menggunakan cmap coolwarm (Merah = Positif, Biru = Negatif)
ax = sns.heatmap(korelasi, 
                 annot=True,          # Menampilkan angka
                 cmap='coolwarm',     # Warna tema
                 vmin=-1, vmax=1,     # Batas -1 sampai 1
                 fmt=".2f",           # 2 desimal
                 linewidths=1.0,      # Garis kotak
                 cbar_kws={'label': 'Nilai Korelasi (1=Kuat Searah, -1=Kuat Berlawanan)'})

# Estetika Label
plt.title('Matriks Korelasi: Parameter Sensor PPG vs Tekanan Darah\n(Nilai yang mendekati 1 atau -1 berarti memiliki hubungan terkuat)', fontsize=14, pad=15)
plt.xticks(rotation=45, ha='right')
plt.yticks(rotation=0)
plt.tight_layout()

# 6. Menyimpan Ke dalam File Gambar
output_image = 'grafik_korelasi_sensor.png'
plt.savefig(output_image, dpi=300)

print("="*60)
print(f"Total baris data bersih yang dianalisis: {len(df)} sampel")
print(f"SUKSES! Gambar Matriks Korelasi berhasil disimpan!")
print(f"Silakan buka file: {output_image}")
print("="*60)
