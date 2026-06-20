"""
SANDBOX: Pulse Width sebagai pengganti Heart Rate di V2
Membandingkan 4 kombinasi fitur:
  A) V2 Asli:     ir_ratio, red_ratio, rise_time_ms, heart_rate
  B) PW Replace:  ir_ratio, red_ratio, rise_time_ms, pulse_width_ms
  C) 5 Fitur:     ir_ratio, red_ratio, rise_time_ms, pulse_width_ms, heart_rate
  D) V3 (acuan):  ir_ratio, red_ratio, crest_time_ratio, heart_rate
"""
import os
import glob
import pandas as pd
import numpy as np
import scipy.signal as signal
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import StratifiedGroupKFold, cross_val_score
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler

RAW_DATA_DIR = os.path.abspath('../../raw_data_pipeline_v2/dataset_v2')
SAMPLING_RATE_HZ = 100

def get_subject_id(filename):
    parts = os.path.basename(filename).split('_')
    if len(parts) >= 3 and parts[0] == 'raw' and parts[1] == 'data':
        return f"subj_{parts[2]}"
    return None

# ===== EKSTRAKSI FITUR LENGKAP (V2 + Pulse Width) =====
print("Mengekstraksi fitur V2 + Pulse Width dari raw data V2...")
csv_files = sorted(glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv')))
features = []

for file in csv_files:
    subject_id = get_subject_id(file)
    if subject_id is None:
        continue
    df = pd.read_csv(file)
    if len(df) < 500:
        continue

    window_size = 1000
    num_windows = len(df) // window_size
    for w in range(num_windows):
        start = w * window_size
        end   = start + window_size
        ir_raw  = df['ir_raw'].values[start:end]
        red_raw = df['red_raw'].values[start:end]

        # Preprocessing (sama dengan V3)
        ir_det = signal.detrend(ir_raw)
        nyq    = 0.5 * SAMPLING_RATE_HZ
        b, a   = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
        ir_flt  = -signal.filtfilt(b, a, ir_det)
        red_flt = -signal.filtfilt(b, a, signal.detrend(red_raw))

        peaks, _ = signal.find_peaks(ir_flt, distance=35, prominence=np.std(ir_flt)*0.5)
        if len(peaks) < 2:
            continue
        rr = np.diff(peaks)
        mean_rr = np.mean(rr)
        if mean_rr == 0:
            continue

        time_cv = (np.std(rr) / mean_rr) * 100
        if time_cv > 15.0:
            continue
        amp_cv = np.std(ir_flt[peaks]) / np.mean(ir_flt[peaks])
        if amp_cv > 0.25:
            continue

        # --- Fitur V2 ---
        rise_times = []
        for p in peaks:
            t_idx = p
            for j in range(p-1, max(0, p-45)-1, -1):
                if ir_flt[j] <= ir_flt[t_idx]:
                    t_idx = j
                elif ir_flt[j] > ir_flt[t_idx]:
                    break
            if p > t_idx:
                rise_times.append((p - t_idx) * 10.0)
        if not rise_times:
            continue
        mean_rise_time = np.mean(rise_times)

        heart_rate = 60.0 / (mean_rr / SAMPLING_RATE_HZ)

        # --- Pulse Width (fitur baru) ---
        widths, _, _, _ = signal.peak_widths(ir_flt, peaks, rel_height=0.5)
        mean_pulse_width_ms = np.mean(widths) * 10.0 if len(widths) > 0 else 0

        # --- AC/DC Ratio ---
        ir_ratio  = (np.std(ir_flt)  / np.mean(ir_raw))  * 100.0
        red_ratio = (np.std(red_flt) / np.mean(red_raw)) * 100.0

        # --- CTR untuk acuan V3 ---
        crest_time_ratio = mean_rise_time / (60000.0 / heart_rate) if heart_rate > 0 else 0

        systolic  = df['systolic_label'].iloc[0]
        diastolic = df['diastolic_label'].iloc[0]
        bp_class  = 1 if (systolic >= 140 or diastolic >= 90) else 0

        features.append({
            'subject_id':       subject_id,
            'ir_ratio':         ir_ratio,
            'red_ratio':        red_ratio,
            'rise_time_ms':     mean_rise_time,
            'heart_rate':       heart_rate,
            'pulse_width_ms':   mean_pulse_width_ms,
            'crest_time_ratio': crest_time_ratio,
            'bp_class':         bp_class
        })

df_all = pd.DataFrame(features)
print(f"Berhasil: {len(df_all)} windows dari {df_all['subject_id'].nunique()} subjek")
print(f"Class dist: {(df_all['bp_class']==0).sum()}N / {(df_all['bp_class']==1).sum()}H\n")

# ===== DEFINISI SET FITUR =====
feature_sets = {
    'A) V2 Asli (rise+HR)':        ['ir_ratio', 'red_ratio', 'rise_time_ms', 'heart_rate'],
    'B) PW Replace (rise+PW)':     ['ir_ratio', 'red_ratio', 'rise_time_ms', 'pulse_width_ms'],
    'C) 5 Fitur (rise+PW+HR)':     ['ir_ratio', 'red_ratio', 'rise_time_ms', 'pulse_width_ms', 'heart_rate'],
    'D) V3 Acuan (CTR+HR)':        ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate'],
}

models = [
    ('SVM-Linear-C1',    Pipeline([('sc', StandardScaler()), ('clf', SVC(kernel='linear', C=1.0, class_weight='balanced', random_state=42))])),
    ('SVM-RBF-C1-g0.01', Pipeline([('sc', StandardScaler()), ('clf', SVC(kernel='rbf', C=1.0, gamma=0.01, class_weight='balanced', random_state=42))])),
    ('RF-10t-d4-L1',     Pipeline([('sc', StandardScaler()), ('clf', RandomForestClassifier(n_estimators=10, max_depth=4, min_samples_leaf=1, class_weight='balanced', random_state=42))])),
    ('RF-20t-d4-L2',     Pipeline([('sc', StandardScaler()), ('clf', RandomForestClassifier(n_estimators=20, max_depth=4, min_samples_leaf=2, class_weight='balanced', random_state=42))])),
]

sgkf = StratifiedGroupKFold(n_splits=5)
groups = df_all['subject_id'].values
y = df_all['bp_class'].values

# ===== HASIL =====
print(f'{"Model":<22} | {"A) V2 Asli":>10} | {"B) PW Replace":>13} | {"C) 5 Fitur":>10} | {"D) V3 Acuan":>11}')
print('-' * 80)
for model_name, pipe in models:
    row = f'{model_name:<22}'
    for feat_name, feats in feature_sets.items():
        X = df_all[feats].values
        scores = cross_val_score(pipe, X, y, groups=groups, cv=sgkf, scoring='accuracy')
        row += f' | {scores.mean()*100:>7.1f}% ({scores.std()*100:.0f}%)'
    print(row)

print()
print('Format: Mean% (Std%)')
print('D) V3 Acuan adalah baseline terbaik saat ini')
