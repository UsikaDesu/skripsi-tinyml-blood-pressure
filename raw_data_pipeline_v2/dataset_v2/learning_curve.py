import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import learning_curve, GroupKFold

# 1. Load Dataset yang sudah Anda ekstrak
dataset_path = os.path.join(os.path.dirname(__file__), 'extracted_features_v2.csv')
df = pd.read_csv(dataset_path)

# 2. Definisikan Fitur, Target, dan Aturan Grup (Subject-Wise)
features = ['ir_ratio', 'red_ratio', 'rise_time_ms', 'heart_rate']
X = df[features]
y = df['bp_class']
groups = df['subject_id']

# 3. Panggil Konfigurasi Model Terbaik Anda (Misal: RF 10-tree max-depth 4)
model = RandomForestClassifier(n_estimators=10, max_depth=4, random_state=42)

# Gunakan GroupKFold untuk mencegah kebocoran data antar subjek
gkf = GroupKFold(n_splits=5)

print("Menghitung Learning Curve... Mohon tunggu sebentar.")

# 4. Kalkulasi Titik-Titik Learning Curve
# Parameter train_sizes akan memotong data dari porsi 20% hingga 100% secara bertahap
train_sizes_abs, train_scores, test_scores = learning_curve(
    estimator=model,
    X=X, 
    y=y, 
    groups=groups,
    train_sizes=np.linspace(0.2, 1.0, 5), 
    cv=gkf,
    scoring='accuracy',
    n_jobs=-1
)

# 5. Hitung Rata-rata dan Standar Deviasi (Simpangan) dari setiap titik
train_mean = np.mean(train_scores, axis=1) * 100
train_std = np.std(train_scores, axis=1) * 100
test_mean = np.mean(test_scores, axis=1) * 100
test_std = np.std(test_scores, axis=1) * 100

# 6. Mulai Menggambar Grafik Visual
plt.figure(figsize=(10, 6))

# Garis Latihan (Warna Biru)
plt.plot(train_sizes_abs, train_mean, 'o-', color='#2b83ba', linewidth=2, label='Akurasi Latihan (Menghafal)')
plt.fill_between(train_sizes_abs, train_mean - train_std, train_mean + train_std, alpha=0.15, color='#2b83ba')

# Garis Ujian (Warna Merah/Oranye)
plt.plot(train_sizes_abs, test_mean, 's-', color='#d7191c', linewidth=2, label='Akurasi Ujian (Pasien Asing / Holdout)')
plt.fill_between(train_sizes_abs, test_mean - test_std, test_mean + test_std, alpha=0.15, color='#d7191c')

# Estetika Grafik ala Jurnal Ilmiah
plt.title('Kurva Belajar (Learning Curve) - Klasifikasi Hipertensi V2', fontsize=14, fontweight='bold', pad=15)
plt.xlabel('Jumlah Baris Data Latihan', fontsize=12)
plt.ylabel('Akurasi (%)', fontsize=12)
plt.ylim(0, 105) # Mengunci sumbu Y dari 0 sampai 100%
plt.xticks(train_sizes_abs) # Menampilkan angka jumlah data di sumbu X dengan presisi
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(loc='lower right', fontsize=11)
plt.tight_layout()

# Tampilkan ke layar
plt.show()