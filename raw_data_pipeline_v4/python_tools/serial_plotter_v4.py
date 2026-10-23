# -*- coding: utf-8 -*-
"""
SERIAL PLOTTER V4
==================
Pilih file raw CSV via CLI, lalu tampilkan grafik sinyal PPG interaktif.

Cara pakai:
  python serial_plotter_v4.py
"""
import os
import glob
import sys
import pandas as pd
import numpy as np
import scipy.signal as signal
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from matplotlib.widgets import RadioButtons
import warnings
warnings.filterwarnings('ignore')

matplotlib.rcParams['font.family'] = 'DejaVu Sans'

# ==========================================
# KONFIGURASI
# ==========================================
DATASET_DIR   = os.path.abspath(
    os.path.join(os.path.dirname(__file__), '..', '..', 'raw_data_pipeline_v2', 'dataset_v2')
)
SAMPLING_RATE = 100
WINDOW_SIZE   = 1000

COLORS = {
    'bg':      '#0f1117',
    'panel':   '#1a1d27',
    'ir_raw':  '#4a9eff',
    'ir_flt':  '#00e5ff',
    'red_raw': '#ff6b6b',
    'red_flt': '#ff4081',
    'peaks':   '#ffd740',
    'text':    '#e0e0e0',
    'subtext': '#9e9e9e',
    'good':    '#69f0ae',
    'bad':     '#ff5252',
    'grid':    '#2a2d3a',
    'accent':  '#7c4dff',
    'card':    '#222535',
}

# ==========================================
# DSP
# ==========================================
def preprocess(ir_raw, red_raw):
    nyq = 0.5 * SAMPLING_RATE
    b, a = signal.butter(2, [0.5 / nyq, 5.0 / nyq], btype='band')
    ir_flt  = -signal.filtfilt(b, a, signal.detrend(ir_raw))
    red_flt = -signal.filtfilt(b, a, signal.detrend(red_raw))
    return ir_flt, red_flt

def detect_peaks(ir_flt):
    peaks, _ = signal.find_peaks(ir_flt, distance=35, prominence=np.std(ir_flt) * 0.5)
    return peaks

def calc_sqa(ir_flt, peaks):
    if len(peaks) < 2:
        return 999.0, 999.0, 0.0
    rr       = np.diff(peaks)
    mean_rr  = np.mean(rr)
    time_cv  = (np.std(rr) / mean_rr) * 100.0 if mean_rr > 0 else 999.0
    amps     = ir_flt[peaks]
    mean_amp = np.mean(amps)
    amp_cv   = (np.std(amps) / mean_amp) if mean_amp > 0 else 999.0
    hr       = 60.0 / (mean_rr / SAMPLING_RATE) if mean_rr > 0 else 0.0
    return time_cv, amp_cv, hr

def parse_filename(filepath):
    name  = os.path.basename(filepath).replace('.csv', '')
    parts = name.split('_')
    subj  = parts[2] if len(parts) > 2 else '?'
    sys_v = parts[-2].replace('bp', '') if len(parts) > 2 else '?'
    dia_v = parts[-1] if len(parts) > 2 else '?'
    return subj, sys_v, dia_v

# ==========================================
# CLI: Pilih File
# ==========================================
def cli_pick_file(files):
    print("\n" + "=" * 65)
    print(f"  PPG SERIAL PLOTTER V4  —  {len(files)} file tersedia")
    print("=" * 65)

    # Filter / search dulu?
    query = input("\n  Ketik inisial subjek untuk filter (Enter = tampilkan semua): ").strip().upper()
    if query:
        filtered = [f for f in files if f'_{query}_' in os.path.basename(f).upper()]
        if not filtered:
            print(f"  [INFO] Tidak ada file untuk '{query}'. Menampilkan semua.")
            filtered = files
    else:
        filtered = files

    print(f"\n  {'No':>4}  {'Subjek':<8}  {'BP':>10}  Nama File")
    print("  " + "-" * 60)
    for i, f in enumerate(filtered, 1):
        subj, sys_v, dia_v = parse_filename(f)
        name = os.path.basename(f)
        print(f"  {i:>4}  {subj:<8}  {sys_v+'/'+dia_v:>10}  {name}")

    print()
    while True:
        try:
            raw = input(f"  Pilih nomor file [1-{len(filtered)}] atau 'q' untuk keluar: ").strip()
            if raw.lower() == 'q':
                sys.exit(0)
            idx = int(raw)
            if 1 <= idx <= len(filtered):
                return filtered[idx - 1]
            print(f"  [ERROR] Masukkan angka antara 1 dan {len(filtered)}.")
        except ValueError:
            print("  [ERROR] Input tidak valid.")

# ==========================================
# PLOTTER
# ==========================================
class Plotter:
    def __init__(self, filepath):
        self.filepath = filepath
        self.df       = pd.read_csv(filepath)
        self.window   = 0  # 0 = Window 1, 1 = Window 2
        subj, sys_v, dia_v = parse_filename(filepath)
        self.label = f"{subj}  |  {sys_v}/{dia_v} mmHg"

        self._build_ui()
        self._draw()
        plt.show()

    def _build_ui(self):
        self.fig = plt.figure(figsize=(14, 8), facecolor=COLORS['bg'])
        self.fig.canvas.manager.set_window_title(
            f"PPG Plotter  —  {os.path.basename(self.filepath)}"
        )

        gs = gridspec.GridSpec(
            3, 3,
            figure=self.fig,
            left=0.07, right=0.97,
            top=0.91, bottom=0.10,
            hspace=0.42, wspace=0.30
        )

        self.ax_ir  = self.fig.add_subplot(gs[0, :])
        self.ax_red = self.fig.add_subplot(gs[1, :])
        self.ax_sqa = self.fig.add_subplot(gs[2, 0])
        self.ax_hr  = self.fig.add_subplot(gs[2, 1])
        self.ax_inf = self.fig.add_subplot(gs[2, 2])

        for ax in [self.ax_ir, self.ax_red, self.ax_sqa, self.ax_hr, self.ax_inf]:
            ax.set_facecolor(COLORS['panel'])
            ax.tick_params(colors=COLORS['subtext'], labelsize=8)
            for sp in ax.spines.values(): sp.set_color(COLORS['grid'])

        # Window selector radio button
        ax_radio = self.fig.add_axes([0.30, 0.01, 0.22, 0.055])
        ax_radio.set_facecolor(COLORS['card'])
        self.radio = RadioButtons(
            ax_radio, ('Window 1  (0–10s)', 'Window 2  (10–20s)'),
            active=0, activecolor=COLORS['accent']
        )
        for lbl in self.radio.labels:
            lbl.set_color(COLORS['text'])
            lbl.set_fontsize(8)
        self.radio.on_clicked(self._change_window)

        # Keyboard hint
        self.fig.text(0.72, 0.028,
                      '[1] Window 1   [2] Window 2   [Q] Keluar',
                      ha='center', va='center', fontsize=8, color=COLORS['subtext'])

        self.fig.canvas.mpl_connect('key_press_event', self._on_key)

    def _draw(self):
        df = self.df
        s  = self.window * WINDOW_SIZE
        e  = s + WINDOW_SIZE
        n  = min(e, len(df))

        if s >= len(df):
            for ax in [self.ax_ir, self.ax_red, self.ax_sqa, self.ax_hr, self.ax_inf]:
                ax.cla()
                ax.set_facecolor(COLORS['panel'])
                ax.text(0.5, 0.5, 'Window 2 tidak tersedia\n(data < 20 detik)',
                        ha='center', va='center', color=COLORS['bad'],
                        transform=ax.transAxes, fontsize=10)
                for sp in ax.spines.values(): sp.set_color(COLORS['grid'])
            self.fig.canvas.draw_idle()
            return

        ir_raw  = df['ir_raw'].values[s:n].astype(float)
        red_raw = df['red_raw'].values[s:n].astype(float)
        t_ms    = df['time_ms'].values[s:n].astype(float)
        t_sec   = (t_ms - t_ms[0]) / 1000.0

        ir_flt, red_flt = preprocess(ir_raw, red_raw)
        peaks = detect_peaks(ir_flt)
        time_cv, amp_cv, hr = calc_sqa(ir_flt, peaks)

        MAX_TIME_CV = 9.4
        MAX_AMP_CV  = 0.370
        sqa_ok = time_cv <= MAX_TIME_CV and amp_cv <= MAX_AMP_CV and 30 <= hr <= 180

        win_label = f"Window {self.window + 1}"

        # ── IR ──────────────────────────────────────────────────────────
        ax = self.ax_ir
        ax.cla(); ax.set_facecolor(COLORS['panel'])
        ax.grid(True, color=COLORS['grid'], linewidth=0.5, alpha=0.5)
        ax.plot(t_sec, ir_raw,  color=COLORS['ir_raw'],  lw=0.8, alpha=0.35, label='IR Raw')
        ir_sc = ir_flt / np.max(np.abs(ir_flt) + 1e-9) * np.std(ir_raw) * 2 + np.mean(ir_raw)
        ax.plot(t_sec, ir_sc,   color=COLORS['ir_flt'],  lw=1.2, label='IR Filtered')
        if len(peaks):
            ax.scatter(t_sec[peaks], ir_sc[peaks], color=COLORS['peaks'],
                       s=25, zorder=5, label=f'Peaks ({len(peaks)})')
        ax.set_title(f'Sinyal IR  |  {self.label}  |  {win_label}',
                     color=COLORS['text'], fontsize=9, pad=4)
        ax.set_ylabel('ADC', color=COLORS['subtext'], fontsize=8)
        for sp in ax.spines.values(): sp.set_color(COLORS['grid'])
        ax.tick_params(colors=COLORS['subtext'], labelsize=7)
        leg = ax.legend(loc='upper right', fontsize=7, framealpha=0.3,
                        facecolor=COLORS['card'], edgecolor=COLORS['grid'])
        for t in leg.get_texts(): t.set_color(COLORS['text'])

        # ── Red ─────────────────────────────────────────────────────────
        ax = self.ax_red
        ax.cla(); ax.set_facecolor(COLORS['panel'])
        ax.grid(True, color=COLORS['grid'], linewidth=0.5, alpha=0.5)
        ax.plot(t_sec, red_raw, color=COLORS['red_raw'], lw=0.8, alpha=0.35, label='Red Raw')
        red_sc = red_flt / np.max(np.abs(red_flt) + 1e-9) * np.std(red_raw) * 2 + np.mean(red_raw)
        ax.plot(t_sec, red_sc,  color=COLORS['red_flt'], lw=1.2, label='Red Filtered')
        ax.set_title('Sinyal Red', color=COLORS['text'], fontsize=9, pad=4)
        ax.set_ylabel('ADC', color=COLORS['subtext'], fontsize=8)
        ax.set_xlabel('Waktu (detik)', color=COLORS['subtext'], fontsize=8)
        for sp in ax.spines.values(): sp.set_color(COLORS['grid'])
        ax.tick_params(colors=COLORS['subtext'], labelsize=7)
        leg = ax.legend(loc='upper right', fontsize=7, framealpha=0.3,
                        facecolor=COLORS['card'], edgecolor=COLORS['grid'])
        for t in leg.get_texts(): t.set_color(COLORS['text'])

        # ── SQA ─────────────────────────────────────────────────────────
        ax = self.ax_sqa
        ax.cla(); ax.set_facecolor(COLORS['panel'])
        ax.set_xticks([]); ax.set_yticks([])
        for sp in ax.spines.values(): sp.set_color(COLORS['grid'])
        ax.set_title('Signal Quality (SQA)', color=COLORS['text'], fontsize=8, pad=4)

        sc = COLORS['good'] if sqa_ok else COLORS['bad']
        ax.text(0.5, 0.80, 'PASSED' if sqa_ok else 'REJECTED',
                ha='center', va='center', fontsize=15, fontweight='bold',
                color=sc, transform=ax.transAxes)

        ct = COLORS['good'] if time_cv <= MAX_TIME_CV else COLORS['bad']
        ca = COLORS['good'] if amp_cv  <= MAX_AMP_CV  else COLORS['bad']
        ax.text(0.08, 0.55, 'Time CV:', ha='left', va='center', fontsize=8,
                color=COLORS['subtext'], transform=ax.transAxes)
        ax.text(0.92, 0.55, f'{time_cv:.1f}% / max {MAX_TIME_CV}%', ha='right', va='center',
                fontsize=8, color=ct, fontweight='bold', transform=ax.transAxes)
        ax.text(0.08, 0.38, 'Amp CV:', ha='left', va='center', fontsize=8,
                color=COLORS['subtext'], transform=ax.transAxes)
        ax.text(0.92, 0.38, f'{amp_cv:.3f} / max {MAX_AMP_CV}', ha='right', va='center',
                fontsize=8, color=ca, fontweight='bold', transform=ax.transAxes)
        ax.text(0.08, 0.21, 'Peaks:', ha='left', va='center', fontsize=8,
                color=COLORS['subtext'], transform=ax.transAxes)
        ax.text(0.92, 0.21, str(len(peaks)), ha='right', va='center',
                fontsize=8, color=COLORS['text'], transform=ax.transAxes)

        # ── HR ──────────────────────────────────────────────────────────
        ax = self.ax_hr
        ax.cla(); ax.set_facecolor(COLORS['panel'])
        ax.set_xticks([]); ax.set_yticks([])
        for sp in ax.spines.values(): sp.set_color(COLORS['grid'])
        ax.set_title('Heart Rate', color=COLORS['text'], fontsize=8, pad=4)
        hc = COLORS['good'] if 40 <= hr <= 180 else COLORS['bad']
        ax.text(0.5, 0.60, f'{hr:.0f}', ha='center', va='center',
                fontsize=32, fontweight='bold', color=hc, transform=ax.transAxes)
        ax.text(0.5, 0.25, 'BPM', ha='center', va='center',
                fontsize=11, color=COLORS['subtext'], transform=ax.transAxes)

        # ── Info ─────────────────────────────────────────────────────────
        ax = self.ax_inf
        ax.cla(); ax.set_facecolor(COLORS['panel'])
        ax.set_xticks([]); ax.set_yticks([])
        for sp in ax.spines.values(): sp.set_color(COLORS['grid'])
        ax.set_title('Info Rekaman', color=COLORS['text'], fontsize=8, pad=4)

        sys_v  = df['systolic_label'].iloc[0]
        dia_v  = df['diastolic_label'].iloc[0]
        subj, _, _ = parse_filename(self.filepath)
        bp_class = 'HIPERTENSI' if (sys_v >= 140 or dia_v >= 90) else 'NORMAL'
        bp_color = COLORS['bad'] if bp_class == 'HIPERTENSI' else COLORS['good']

        rows = [
            ('Subjek',  subj),
            ('BP',      f'{sys_v}/{dia_v} mmHg'),
            ('Kelas',   bp_class),
            ('Samples', str(n - s)),
            ('Durasi',  f'{(n-s)/SAMPLING_RATE:.1f} s'),
        ]
        y = 0.88
        for lbl, val in rows:
            c = bp_color if lbl == 'Kelas' else COLORS['text']
            ax.text(0.08, y, f'{lbl}:', ha='left', va='center', fontsize=8,
                    color=COLORS['subtext'], transform=ax.transAxes)
            ax.text(0.92, y, val, ha='right', va='center', fontsize=8,
                    color=c, fontweight='bold', transform=ax.transAxes)
            y -= 0.17

        self.fig.canvas.draw_idle()

    def _change_window(self, label):
        self.window = 0 if '1' in label else 1
        self._draw()

    def _on_key(self, event):
        if event.key == '1':
            self.window = 0; self.radio.set_active(0); self._draw()
        elif event.key == '2':
            self.window = 1; self.radio.set_active(1); self._draw()
        elif event.key in ('q', 'escape'):
            plt.close('all')


# ==========================================
# MAIN
# ==========================================
if __name__ == '__main__':
    files = sorted(glob.glob(os.path.join(DATASET_DIR, 'raw_data_*.csv')))
    if not files:
        print(f"[ERROR] Tidak ada file CSV di:\n  {DATASET_DIR}")
        sys.exit(1)

    while True:
        chosen = cli_pick_file(files)
        Plotter(chosen)

        again = input("\n  Buka file lain? [y/n]: ").strip().lower()
        if again != 'y':
            print("  Sampai jumpa!\n")
            break
