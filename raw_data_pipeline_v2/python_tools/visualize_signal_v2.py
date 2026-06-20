"""
Visualisasi Pipeline Sinyal PPG V2
===================================
Menampilkan 4 tahap pemrosesan sinyal:
  1. Sinyal Mentah (Raw IR & RED)
  2. Setelah Detrending
  3. Setelah Bandpass Filter + Inversi
  4. Hasil Akhir: Puncak Sistolik, Lembah Diastolik, Rise Time

Cara pakai:
  python visualize_signal_v2.py                          # File terbaru
  python visualize_signal_v2.py raw_data_KYS_..._bp130_74.csv   # File spesifik
  python visualize_signal_v2.py raw_data_KYS_..._bp130_74.csv 1 # Window ke-2
"""
import os
import sys
import glob
import numpy as np
import pandas as pd
import scipy.signal as signal
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec

# ==========================================
# CONFIGURATION
# ==========================================
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
RAW_DATA_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, '..', 'dataset_v2'))
SAMPLING_RATE_HZ = 100
WINDOW_SIZE = 1000  # 10 detik @ 100Hz

def get_subject_bp(filename):
    """Ekstrak info dari nama file."""
    bn = os.path.basename(filename)
    parts = bn.split('_')
    subj = parts[2] if len(parts) >= 3 else '???'
    date_str = parts[3] if len(parts) >= 4 else '????????'
    time_str = parts[4] if len(parts) >= 5 else '??????'
    # Ekstrak BP dari bpXXX
    bp_part = [p for p in parts if p.startswith('bp')]
    if bp_part:
        bp_vals = bp_part[0].replace('bp', '').replace('.csv', '')
        sys_val = bp_vals
    else:
        sys_val = '???'
    # Format tanggal
    date_fmt = f"{date_str[:4]}-{date_str[4:6]}-{date_str[6:8]}" if len(date_str) == 8 else date_str
    time_fmt = f"{time_str[:2]}:{time_str[2:4]}:{time_str[4:6]}" if len(time_str) == 6 else time_str
    return subj, date_fmt, time_fmt

def visualize_pipeline(filepath, window_idx=0):
    """Visualisasi 4 tahap pipeline sinyal PPG."""
    
    df = pd.read_csv(filepath)
    basename = os.path.basename(filepath)
    subj, date_fmt, time_fmt = get_subject_bp(filepath)
    sys_bp = df['systolic_label'].iloc[0]
    dia_bp = df['diastolic_label'].iloc[0]
    bp_class = "HIPERTENSI" if (sys_bp >= 140 or dia_bp >= 90) else "NORMAL"
    
    num_windows = len(df) // WINDOW_SIZE
    if window_idx >= num_windows:
        print(f"Error: Window {window_idx} tidak ada. File hanya punya {num_windows} window (0-{num_windows-1}).")
        return
    
    start = window_idx * WINDOW_SIZE
    end = start + WINDOW_SIZE
    
    ir_raw = df['ir_raw'].values[start:end]
    red_raw = df['red_raw'].values[start:end]
    time_ms = df['time_ms'].values[start:end]
    time_s = (time_ms - time_ms[0]) / 1000.0  # Konversi ke detik
    
    # ==========================================
    # TAHAP 1: DETRENDING
    # ==========================================
    ir_detrended = signal.detrend(ir_raw)
    red_detrended = signal.detrend(red_raw)
    
    # ==========================================
    # TAHAP 2: BANDPASS FILTER (0.5 - 5 Hz)
    # ==========================================
    nyq = 0.5 * SAMPLING_RATE_HZ
    b, a = signal.butter(2, [0.5/nyq, 5.0/nyq], btype='band')
    ir_filtered = signal.filtfilt(b, a, ir_detrended)
    red_filtered = signal.filtfilt(b, a, red_detrended)
    
    # ==========================================
    # TAHAP 3: INVERSI SINYAL (Fisika MAX30102)
    # ==========================================
    ir_inverted = -ir_filtered
    red_inverted = -red_filtered
    
    # ==========================================
    # TAHAP 4: DETEKSI PUNCAK & LEMBAH
    # ==========================================
    peaks, _ = signal.find_peaks(ir_inverted, distance=35, prominence=np.std(ir_inverted)*0.5)
    
    # Cari lembah (trough) untuk setiap puncak
    troughs = []
    rise_times = []
    for p in peaks:
        trough_idx = p
        start_search = max(0, p - 45)
        for j in range(p - 1, start_search - 1, -1):
            if ir_inverted[j] <= ir_inverted[trough_idx]:
                trough_idx = j
            elif ir_inverted[j] > ir_inverted[trough_idx]:
                break
        if p > trough_idx:
            troughs.append(trough_idx)
            rise_time_ms = (p - trough_idx) * 10.0
            rise_times.append(rise_time_ms)
    
    # Hitung fitur
    if len(peaks) > 1:
        rr_intervals = np.diff(peaks) / SAMPLING_RATE_HZ
        heart_rate = 60.0 / np.mean(rr_intervals)
    else:
        heart_rate = 0
    
    ir_std = np.std(ir_inverted)
    ir_mean = np.mean(ir_raw)
    red_std = np.std(red_inverted)
    red_mean = np.mean(red_raw)
    ir_ratio = (ir_std / ir_mean) * 100.0 if ir_mean > 0 else 0
    red_ratio = (red_std / red_mean) * 100.0 if red_mean > 0 else 0
    mean_rise_time = np.mean(rise_times) if rise_times else 0
    
    # SQA Metrics
    if len(peaks) > 1:
        rr_samples = np.diff(peaks)
        time_cv = (np.std(rr_samples) / np.mean(rr_samples)) * 100
        amplitudes = ir_inverted[peaks]
        amp_cv = (np.std(amplitudes) / np.mean(amplitudes)) * 100 if np.mean(amplitudes) != 0 else 0
    else:
        time_cv = 0
        amp_cv = 0
    
    sqa_pass = time_cv <= 15.0 and amp_cv <= 25.0 and len(peaks) >= 2
    sqa_status = "✅ LOLOS SQA" if sqa_pass else "❌ DITOLAK SQA"
    
    # ==========================================
    # PLOTTING
    # ==========================================
    fig = plt.figure(figsize=(16, 14))
    fig.patch.set_facecolor('#1a1a2e')
    
    gs = gridspec.GridSpec(4, 1, hspace=0.35, top=0.92, bottom=0.05, left=0.08, right=0.95)
    
    title = f"Pipeline Sinyal PPG — {subj} ({date_fmt} {time_fmt}) — BP: {sys_bp}/{dia_bp} [{bp_class}] — Window {window_idx+1}/{num_windows}"
    fig.suptitle(title, fontsize=14, fontweight='bold', color='white', y=0.97)
    
    colors_ir = '#00d4ff'
    colors_red = '#ff6b6b'
    colors_peak = '#ffd700'
    colors_trough = '#00ff88'
    bg_color = '#16213e'
    grid_color = '#2a2a4a'
    
    # ── PANEL 1: Sinyal Mentah ──
    ax1 = fig.add_subplot(gs[0])
    ax1.set_facecolor(bg_color)
    ax1.plot(time_s, ir_raw, color=colors_ir, linewidth=0.8, alpha=0.9, label=f'IR Raw (μ={ir_raw.mean():.0f})')
    ax1.plot(time_s, red_raw, color=colors_red, linewidth=0.8, alpha=0.9, label=f'RED Raw (μ={red_raw.mean():.0f})')
    ax1.set_title('① Sinyal Mentah (Raw PPG)', fontsize=11, fontweight='bold', color='white', pad=8)
    ax1.set_ylabel('ADC Value', color='white', fontsize=9)
    ax1.legend(loc='upper right', fontsize=8, facecolor=bg_color, edgecolor=grid_color, labelcolor='white')
    ax1.tick_params(colors='white', labelsize=8)
    ax1.grid(True, alpha=0.2, color=grid_color)
    ax1.spines['bottom'].set_color(grid_color)
    ax1.spines['left'].set_color(grid_color)
    ax1.spines['top'].set_visible(False)
    ax1.spines['right'].set_visible(False)
    
    # ── PANEL 2: Setelah Detrend ──
    ax2 = fig.add_subplot(gs[1])
    ax2.set_facecolor(bg_color)
    ax2.plot(time_s, ir_detrended, color=colors_ir, linewidth=0.8, alpha=0.9, label='IR Detrended')
    ax2.plot(time_s, red_detrended, color=colors_red, linewidth=0.8, alpha=0.9, label='RED Detrended')
    ax2.axhline(y=0, color='white', linewidth=0.5, alpha=0.3, linestyle='--')
    ax2.set_title('② Setelah Detrending (Hapus Drift Baseline)', fontsize=11, fontweight='bold', color='white', pad=8)
    ax2.set_ylabel('Amplitude', color='white', fontsize=9)
    ax2.legend(loc='upper right', fontsize=8, facecolor=bg_color, edgecolor=grid_color, labelcolor='white')
    ax2.tick_params(colors='white', labelsize=8)
    ax2.grid(True, alpha=0.2, color=grid_color)
    ax2.spines['bottom'].set_color(grid_color)
    ax2.spines['left'].set_color(grid_color)
    ax2.spines['top'].set_visible(False)
    ax2.spines['right'].set_visible(False)
    
    # ── PANEL 3: Setelah Bandpass + Inversi ──
    ax3 = fig.add_subplot(gs[2])
    ax3.set_facecolor(bg_color)
    ax3.plot(time_s, ir_inverted, color=colors_ir, linewidth=0.8, alpha=0.9, label='IR Filtered+Inverted')
    ax3.plot(time_s, red_inverted, color=colors_red, linewidth=0.8, alpha=0.9, label='RED Filtered+Inverted')
    ax3.axhline(y=0, color='white', linewidth=0.5, alpha=0.3, linestyle='--')
    ax3.set_title('③ Setelah Bandpass Filter (0.5-5Hz) + Inversi Fisika', fontsize=11, fontweight='bold', color='white', pad=8)
    ax3.set_ylabel('Amplitude', color='white', fontsize=9)
    ax3.legend(loc='upper right', fontsize=8, facecolor=bg_color, edgecolor=grid_color, labelcolor='white')
    ax3.tick_params(colors='white', labelsize=8)
    ax3.grid(True, alpha=0.2, color=grid_color)
    ax3.spines['bottom'].set_color(grid_color)
    ax3.spines['left'].set_color(grid_color)
    ax3.spines['top'].set_visible(False)
    ax3.spines['right'].set_visible(False)
    
    # ── PANEL 4: Hasil Akhir dengan Puncak & Lembah ──
    ax4 = fig.add_subplot(gs[3])
    ax4.set_facecolor(bg_color)
    ax4.plot(time_s, ir_inverted, color=colors_ir, linewidth=1.0, alpha=0.9, label='IR (Siap Fitur)')
    
    # Plot puncak sistolik
    if len(peaks) > 0:
        ax4.scatter(time_s[peaks], ir_inverted[peaks], color=colors_peak, s=60, zorder=5, 
                    marker='v', label=f'Puncak Sistolik ({len(peaks)})')
    
    # Plot lembah diastolik dan garis rise time
    if len(troughs) > 0:
        ax4.scatter(time_s[troughs], ir_inverted[troughs], color=colors_trough, s=60, zorder=5,
                    marker='^', label=f'Lembah Diastolik ({len(troughs)})')
        
        # Gambar garis rise time (trough -> peak)
        for i, (t, p) in enumerate(zip(troughs, peaks[:len(troughs)])):
            ax4.plot([time_s[t], time_s[p]], [ir_inverted[t], ir_inverted[p]], 
                     color='#ff9f43', linewidth=1.5, alpha=0.7, linestyle='--')
            # Label rise time di tengah garis
            mid_x = (time_s[t] + time_s[p]) / 2
            mid_y = (ir_inverted[t] + ir_inverted[p]) / 2
            if i == 0:  # Hanya label di puncak pertama agar tidak berantakan
                ax4.annotate(f'{rise_times[i]:.0f}ms', xy=(mid_x, mid_y), fontsize=7,
                           color='#ff9f43', fontweight='bold', ha='center',
                           bbox=dict(boxstyle='round,pad=0.2', facecolor=bg_color, edgecolor='#ff9f43', alpha=0.8))
    
    ax4.axhline(y=0, color='white', linewidth=0.5, alpha=0.3, linestyle='--')
    ax4.set_title(f'④ Hasil Akhir: Deteksi Puncak & Rise Time — {sqa_status}', 
                  fontsize=11, fontweight='bold', color='white', pad=8)
    ax4.set_xlabel('Waktu (detik)', color='white', fontsize=9)
    ax4.set_ylabel('Amplitude', color='white', fontsize=9)
    ax4.legend(loc='upper right', fontsize=8, facecolor=bg_color, edgecolor=grid_color, labelcolor='white')
    ax4.tick_params(colors='white', labelsize=8)
    ax4.grid(True, alpha=0.2, color=grid_color)
    ax4.spines['bottom'].set_color(grid_color)
    ax4.spines['left'].set_color(grid_color)
    ax4.spines['top'].set_visible(False)
    ax4.spines['right'].set_visible(False)
    
    # ── INFO BOX: Fitur yang Diekstrak ──
    info_text = (
        f"HR: {heart_rate:.1f} BPM  |  "
        f"Rise Time: {mean_rise_time:.1f} ms  |  "
        f"IR Ratio: {ir_ratio:.4f}%  |  "
        f"RED Ratio: {red_ratio:.4f}%  |  "
        f"Time CV: {time_cv:.1f}%  |  "
        f"Amp CV: {amp_cv:.1f}%"
    )
    fig.text(0.5, 0.008, info_text, ha='center', fontsize=9, color='#aaaaaa',
             style='italic', fontfamily='monospace',
             bbox=dict(boxstyle='round,pad=0.4', facecolor='#0f3460', edgecolor='#2a2a4a', alpha=0.9))
    
    plt.show()
    print(f"\n[INFO] Fitur yang diekstrak dari window ini:")
    print(f"  Heart Rate    : {heart_rate:.2f} BPM")
    print(f"  Rise Time     : {mean_rise_time:.2f} ms")
    print(f"  IR Ratio      : {ir_ratio:.4f}%")
    print(f"  RED Ratio     : {red_ratio:.4f}%")
    print(f"  Time CV (SQA) : {time_cv:.1f}% {'✅' if time_cv <= 15 else '❌'}")
    print(f"  Amp CV (SQA)  : {amp_cv:.1f}% {'✅' if amp_cv <= 25 else '❌'}")


def interactive_menu():
    """Menu interaktif untuk memilih file dan window."""
    all_files = sorted(glob.glob(os.path.join(RAW_DATA_DIR, 'raw_data_*.csv')))
    if not all_files:
        print("Tidak ada file raw data ditemukan!")
        return
    
    # Kelompokkan per subjek
    subjects = {}
    for f in all_files:
        parts = os.path.basename(f).split('_')
        subj = parts[2]
        if subj not in subjects:
            subjects[subj] = []
        subjects[subj].append(f)
    
    while True:
        # ── PILIH SUBJEK ──
        print("\n" + "=" * 60)
        print("  VISUALISASI PIPELINE SINYAL PPG V2")
        print("=" * 60)
        subj_list = sorted(subjects.keys())
        for i, subj in enumerate(subj_list):
            count = len(subjects[subj])
            print(f"  [{i+1:2d}] {subj} ({count} file)")
        print(f"  [ 0] Keluar")
        print("-" * 60)
        
        try:
            choice = input("Pilih subjek (nomor): ").strip()
            if choice == '0' or choice.lower() == 'q':
                print("Bye!")
                break
            subj_idx = int(choice) - 1
            if subj_idx < 0 or subj_idx >= len(subj_list):
                print("Nomor tidak valid!")
                continue
        except (ValueError, EOFError):
            print("Input tidak valid!")
            continue
        
        selected_subj = subj_list[subj_idx]
        subj_files = subjects[selected_subj]
        
        # ── PILIH FILE ──
        print(f"\n--- File untuk subjek {selected_subj} ---")
        for i, f in enumerate(subj_files):
            bn = os.path.basename(f)
            parts = bn.split('_')
            date_str = parts[3]
            time_str = parts[4]
            df_tmp = pd.read_csv(f)
            sys_val = df_tmp['systolic_label'].iloc[0]
            dia_val = df_tmp['diastolic_label'].iloc[0]
            n_win = len(df_tmp) // WINDOW_SIZE
            bp_tag = "HT" if (sys_val >= 140 or dia_val >= 90) else "N "
            date_fmt = f"{date_str[:4]}-{date_str[4:6]}-{date_str[6:8]}"
            time_fmt = f"{time_str[:2]}:{time_str[2:4]}"
            print(f"  [{i+1:2d}] {date_fmt} {time_fmt} | BP: {sys_val}/{dia_val} [{bp_tag}] | {n_win} window")
        print(f"  [ 0] Kembali")
        print("-" * 60)
        
        try:
            choice = input("Pilih file (nomor): ").strip()
            if choice == '0':
                continue
            file_idx = int(choice) - 1
            if file_idx < 0 or file_idx >= len(subj_files):
                print("Nomor tidak valid!")
                continue
        except (ValueError, EOFError):
            print("Input tidak valid!")
            continue
        
        filepath = subj_files[file_idx]
        df_sel = pd.read_csv(filepath)
        n_windows = len(df_sel) // WINDOW_SIZE
        
        # ── PILIH WINDOW ──
        if n_windows > 1:
            print(f"\nFile ini memiliki {n_windows} window (masing-masing 10 detik).")
            for w in range(n_windows):
                print(f"  [{w+1}] Window {w+1}")
            print(f"  [0] Semua window (satu per satu)")
            
            try:
                choice = input(f"Pilih window (1-{n_windows}, atau 0 untuk semua): ").strip()
                if choice == '0':
                    for w in range(n_windows):
                        print(f"\n>>> Menampilkan Window {w+1}/{n_windows}...")
                        visualize_pipeline(filepath, w)
                    continue
                win_idx = int(choice) - 1
                if win_idx < 0 or win_idx >= n_windows:
                    print("Nomor tidak valid!")
                    continue
            except (ValueError, EOFError):
                print("Input tidak valid!")
                continue
        else:
            win_idx = 0
        
        print(f"\n>>> Menampilkan: {os.path.basename(filepath)} — Window {win_idx+1}")
        visualize_pipeline(filepath, win_idx)


if __name__ == "__main__":
    if len(sys.argv) >= 2:
        # Mode command-line (tetap didukung)
        target = sys.argv[1]
        if os.path.exists(target):
            filepath = target
        else:
            filepath = os.path.join(RAW_DATA_DIR, target)
        
        if not os.path.exists(filepath):
            print(f"File tidak ditemukan: {filepath}")
            sys.exit(1)
        
        window_idx = int(sys.argv[2]) if len(sys.argv) >= 3 else 0
        visualize_pipeline(filepath, window_idx)
    else:
        # Mode interaktif
        interactive_menu()
