import pandas as pd
import numpy as np
import scipy.signal as signal
import glob, os

DATASET = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'dataset')
SR = 100
WIN = 1000

files = sorted(glob.glob(os.path.join(DATASET, 'raw_data_*.csv')))

print('Mencari file dengan perbedaan TERBESAR antar tahap preprocessing...')
print('=' * 80)

nyq = 0.5 * SR
b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')

results = []
for i, f in enumerate(files):
    df = pd.read_csv(f)
    ir = df['ir_raw'].values[:WIN].astype(float)
    
    # Ukur DRIFT (seberapa miring garis dasar)
    detrended = signal.detrend(ir)
    drift = np.std(ir - detrended)
    
    # Ukur NOISE (seberapa banyak noise di luar 0.5-5Hz)
    filtered = signal.filtfilt(b, a, detrended)
    noise = np.std(detrended - filtered)
    
    skor = drift + noise * 2
    results.append((skor, drift, noise, i+1, os.path.basename(f)))

results.sort(key=lambda x: -x[0])

print('\nTOP 5 FILE TERBAIK UNTUK VISUALISASI:')
print('=' * 80)
for rank, (skor, drift, noise, idx, name) in enumerate(results[:5], 1):
    print(f'  #{rank}: Nomor [{idx}] {name}')
    print(f'       Drift = {drift:.1f} (semakin besar = baseline semakin miring)')
    print(f'       Noise = {noise:.1f} (semakin besar = noise semakin banyak)')
    print(f'       Skor  = {skor:.1f}')
    print()

print('TOP 5 FILE PALING BERSIH (perbedaan paling kecil):')
print('=' * 80)
results_clean = sorted(results, key=lambda x: x[0])
for rank, (skor, drift, noise, idx, name) in enumerate(results_clean[:5], 1):
    print(f'  #{rank}: Nomor [{idx}] {name}')
    print(f'       Drift = {drift:.1f}  Noise = {noise:.1f}  Skor = {skor:.1f}')
    print()
