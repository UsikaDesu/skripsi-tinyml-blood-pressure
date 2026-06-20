import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Define the path to the dataset
data_path = os.path.join("dataset_v2", "extracted_features_v2.csv")

# Load the dataset
try:
    df = pd.read_csv(data_path)
    print(f"Berhasil memuat data dari {data_path}")
except FileNotFoundError:
    print(f"Error: File tidak ditemukan di {data_path}")
    print("Pastikan Anda menjalankan skrip ini dari dalam folder raw_data_pipeline_v2")
    exit()

# Pilih variabel (fitur) dan target (tekanan darah)
features = ['ir_ratio', 'red_ratio', 'rise_time_ms', 'heart_rate']
targets = ['bp_class']
columns_of_interest = features + targets

# Menampilkan korelasi
print("\n--- Matriks Korelasi dengan Status Hipertensi (bp_class) ---")
correlation_matrix = df[columns_of_interest].corr()
print(correlation_matrix[targets].loc[features])

# 1. Visualisasi Matriks Korelasi (Heatmap)
plt.figure(figsize=(8, 6))
sns.heatmap(correlation_matrix, annot=True, cmap='coolwarm', vmin=-1, vmax=1, fmt=".2f")
plt.title('Heatmap Korelasi antara Fitur dan Status Hipertensi (V2)')
plt.tight_layout()
plt.savefig('korelasi_heatmap_bp_class_v2.png')
print("Heatmap korelasi telah disimpan sebagai 'korelasi_heatmap_bp_class_v2.png'")
plt.show()

# 2. Visualisasi Boxplot (Distribusi fitur berdasarkan kelas HT dan Non-HT)
print("\nMembuat Boxplots (Distribusi fitur)...")
# Buat label untuk memperjelas plot
df['HT_Status'] = df['bp_class'].map({0: 'Non-HT', 1: 'HT'})

fig, axes = plt.subplots(1, 4, figsize=(16, 5))
for i, feature in enumerate(features):
    sns.boxplot(data=df, x='HT_Status', y=feature, ax=axes[i], hue='HT_Status', legend=False)
    axes[i].set_title(f'Distribusi {feature}')
    axes[i].set_xlabel('Status Hipertensi')

plt.suptitle('Distribusi Fitur Berdasarkan Status Hipertensi (Non-HT vs HT)', y=1.05)
plt.tight_layout()
plt.savefig('boxplot_distribusi_fitur_ht_v2.png')
print("Boxplot telah disimpan sebagai 'boxplot_distribusi_fitur_ht_v2.png'")
plt.show()

print("\nSelesai! Anda dapat melihat visualisasi pada jendela yang muncul atau melalui file gambar yang tersimpan.")
