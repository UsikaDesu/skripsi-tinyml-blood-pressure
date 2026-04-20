import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import os
import glob
import shutil

# ==========================================
# CONFIGURATION
# ==========================================
RAW_DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset')
OLD_TRAINING_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'training_data')
ARTIFACT_IMG = r'C:\Users\Kayyis\.gemini\antigravity\brain\0cbcc73c-da4f-4d41-acf5-bc7850b899c2\korelasi_raw_features.png'
OUTPUT_IMG = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'korelasi_raw_features.png')

def plot_correlation():
    print("=" * 60)
    print("PILIHAN SUMBER DATA KORELASI:")
    print("1. Kumpulan Data Ekstraksi Baru (extracted_features.csv)")
    print("2. Kumpulan Data Latih Lama (training_data/*.csv gabungan)")
    print("3. Analisis 1 File Mentah Spesifik (ir_raw vs red_raw)")
    print("=" * 60)
    
    choice = input("\nMasukkan angka pilihan Anda [1/2/3]: ").strip()
    
    df = None
    judul_grafik = ""
    
    if choice == '1':
        data_path = os.path.join(RAW_DATA_DIR, 'extracted_features.csv')
        if not os.path.exists(data_path):
            print(f"ERROR: File {data_path} tidak ditemukan.")
            return
        df = pd.read_csv(data_path)
        fitur = ['ir_mean', 'red_mean', 'ir_std', 'red_std', 'heart_rate', 'spo2', 'systolic_bp', 'diastolic_bp']
        judul_grafik = f"Matriks Korelasi (Fitur Pipeline Baru)\n({len(df)} Sampel Data Perekaman)"
        
    elif choice == '2':
        csv_files = glob.glob(os.path.join(OLD_TRAINING_DIR, '*.csv'))
        # Abaikan extracted_features lama jika masih ada disana dan abaikan synthetic
        csv_files = [f for f in csv_files if 'extracted' not in f and 'synthetic' not in f]
        
        if not csv_files:
            print("ERROR: CSV data latih lama tidak ditemukan.")
            return
            
        dfs = [pd.read_csv(f) for f in csv_files]
        df = pd.concat(dfs, ignore_index=True)
        fitur = ['ir_mean', 'red_mean', 'ir_std', 'red_std', 'heart_rate', 'spo2', 'systolic_bp', 'diastolic_bp']
        judul_grafik = f"Matriks Korelasi (Data Latih Lama)\n({len(df)} Sampel Pengukuran)"
        
    elif choice == '3':
        raw_files = glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv'))
        if not raw_files:
            print("ERROR: Tidak ada file raw.")
            return
            
        print("\nPilih file mentah (raw data):")
        for i, f in enumerate(raw_files):
            print(f"[{i+1}] {os.path.basename(f)}")
        idx_str = input(f"Pilih [1-{len(raw_files)}]: ").strip()
        try:
            selected_file = raw_files[int(idx_str)-1]
            df = pd.read_csv(selected_file)
            fitur = ['ir_raw', 'red_raw']
            judul_grafik = f"Matriks Korelasi Sinyal Mentah\n{os.path.basename(selected_file)}"
        except (ValueError, IndexError):
            print("Pilihan invalid.")
            return
    else:
        print("Pilihan tidak valid.")
        return

    if df is None or len(df) < 2:
        print("ERROR: Data tidak valid atau kurang dari 2 baris untuk korelasi.")
        return

    # Pastikan semua fitur ada di df
    missing = [f for f in fitur if f not in df.columns]
    if missing:
        print(f"ERROR: Beberapa kolom hilang dari CSV: {missing}")
        return
        
    df_fitur = df[fitur]
    korelasi = df_fitur.corr()

    # Membuat Gambar Grafik Visualisasi (Heatmap)
    plt.figure(figsize=(10, 8))
    sns.heatmap(korelasi, 
                     annot=True,          
                     cmap='coolwarm',     
                     vmin=-1, vmax=1,     
                     fmt=".2f",           
                     linewidths=1.0,      
                     cbar_kws={'label': 'Nilai Korelasi'})

    plt.title(judul_grafik, fontsize=14, pad=15)
    plt.xticks(rotation=45, ha='right')
    plt.yticks(rotation=0)
    plt.tight_layout()

    # Menyimpan Ke dalam File Gambar
    plt.savefig(OUTPUT_IMG, dpi=300)

    try:
        shutil.copy(OUTPUT_IMG, ARTIFACT_IMG)
        print(f"Copy ke Artifact Sukses: {ARTIFACT_IMG}")
    except Exception as e:
        pass

    print("="*60)
    print(f"Total baris data yang dianalisis: {len(df)} baris")
    print("SUKSES! Gambar Matriks Korelasi berhasil disimpan!")
    print(f"Silakan buka file: {OUTPUT_IMG}")
    print("="*60)

if __name__ == '__main__':
    plot_correlation()
