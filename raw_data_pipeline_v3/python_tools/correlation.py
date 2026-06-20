import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import os

# 1. Load data CSV Anda (ganti nama filenya)
data_path = os.path.join(os.path.dirname(__file__), '..', 'dataset', 'extracted_features_v3.csv')
df = pd.read_csv(data_path)

# Filter hanya kolom numerik agar tidak error
df_numerik = df.select_dtypes(include=[np.number])

# 2. Hitung korelasi
corr_matrix = df_numerik.corr()

print("MATRIKS KORELASI:")
print(corr_matrix[['rise_time_ms', 'pulse_width_ms', 'heart_rate', 'bp_class']])

# 3. Gambar Heatmap
plt.figure(figsize=(10, 8))
sns.heatmap(corr_matrix, annot=True, cmap='coolwarm', vmin=-1, vmax=1, fmt=".2f")
plt.title('Matriks Korelasi Fitur PPG')
plt.tight_layout()

# Tampilkan atau simpan
plt.savefig('korelasi_data_asli.png')
print("\nGambar heatmap disimpan sebagai korelasi_data_asli.png")
