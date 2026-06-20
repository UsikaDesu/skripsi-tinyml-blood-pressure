import pandas as pd
import numpy as np
import os

# Path ke dataset di v3
dataset_path = os.path.join("dataset_v3_v2", "extracted_features_v3_v2.csv")

try:
    df = pd.read_csv(dataset_path)
    print("--- Jumlah Data per Subjek (Sebelum Sintesis) ---")
    print(df['subject_id'].value_counts())
    
    # Kumpulkan semua dataframe yang sudah disesuaikan
    df_list = []
    
    for subject, group in df.groupby('subject_id'):
        count = len(group)
        if count < 10:
            # Butuh mensintesis sebanyak (10 - count)
            diff = 10 - count
            # Ambil sampel acak dari data yang ada dengan replacement
            synthetic_rows = group.sample(n=diff, replace=True, random_state=42).copy()
            
            # Tambahkan sedikit Gaussian Noise pada fitur numerik
            numeric_cols = ['ir_ratio', 'red_ratio', 'rise_time_ms', 'heart_rate']
            for col in numeric_cols:
                std_val = df[col].std()
                noise = np.random.normal(0, std_val * 0.05, size=diff) # 5% dari standar deviasi fitur
                synthetic_rows[col] = synthetic_rows[col] + noise
                
            # Gabungkan data asli subjek dengan data sintetisnya
            group_extended = pd.concat([group, synthetic_rows], ignore_index=True)
            df_list.append(group_extended)
        else:
            # Jika sudah 10 atau lebih, biarkan saja
            df_list.append(group)
            
    # Gabungkan kembali semua subjek menjadi satu dataframe
    df_balanced = pd.concat(df_list, ignore_index=True)
    
    # Shuffle (acak) urutan baris jika diperlukan (opsional, di sini kita biarkan saja atau urutkan)
    # df_balanced = df_balanced.sample(frac=1, random_state=42).reset_index(drop=True)
    
    print("\n--- Jumlah Data per Subjek (Setelah Duplikasi) ---")
    print(df_balanced['subject_id'].value_counts())
    
    # Simpan kembali (menimpa file yang lama di v3)
    df_balanced.to_csv(dataset_path, index=False)
    print(f"\nData berhasil diduplikat dan disimpan ke {dataset_path}")

except Exception as e:
    print(f"Error: {e}")
