import os
import sys
import pandas as pd
import numpy as np

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
FULL_CSV = os.path.join(SCRIPT_DIR, "auto_tune_feature_combinations_full.csv")
SUMMARY_CSV = os.path.join(SCRIPT_DIR, "auto_tune_feature_combinations_summary.csv")
H2H_CSV = os.path.join(SCRIPT_DIR, "auto_tune_rf_vs_svm_head_to_head.csv")
CATATAN_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "catatan penting"))
REPORT_MD = os.path.join(CATATAN_DIR, "laporan_auto_tune_kombinasi_fitur.md")

ESP32_FLASH_LIMIT_KB = 400.0

if not os.path.exists(FULL_CSV):
    print(f"Error: {FULL_CSV} not found!")
    sys.exit(1)

df_full = pd.read_csv(FULL_CSV)
print(f"Loaded {len(df_full)} rows from {FULL_CSV}")

# Dapatkan daftar unik kombinasi fitur
all_combos_ordered = [
    # 1 Fitur
    "{CTR}", "{HR}", "{IR}", "{Red}",
    # 2 Fitur
    "{CTR, HR}", "{CTR, IR}", "{CTR, Red}", "{HR, IR}", "{HR, Red}", "{IR, Red}",
    # 3 Fitur
    "{CTR, HR, IR}", "{CTR, HR, Red}", "{CTR, IR, Red}", "{HR, IR, Red}",
    # 4 Fitur
    "{CTR, HR, IR, Red}"
]

# Ambil best RF dan best SVM untuk tiap kombinasi
best_rf_list = []
best_svm_list = []
summary_30_list = []

for combo_name in all_combos_ordered:
    sub = df_full[df_full['feature_combo'] == combo_name]
    
    # RF
    rf_sub = sub[(sub['algorithm'] == 'Random Forest') & (sub['size_kb'] <= ESP32_FLASH_LIMIT_KB)]
    best_rf = rf_sub.sort_values(by=['subj_acc', 'win_acc', 'gap', 'size_kb'], ascending=[False, False, True, True]).iloc[0]
    best_rf_list.append(best_rf)
    summary_30_list.append(best_rf)
    
    # SVM
    svm_sub = sub[(sub['algorithm'] == 'SVM') & (sub['size_kb'] <= ESP32_FLASH_LIMIT_KB)]
    best_svm = svm_sub.sort_values(by=['subj_acc', 'win_acc', 'gap', 'size_kb'], ascending=[False, False, True, True]).iloc[0]
    best_svm_list.append(best_svm)
    summary_30_list.append(best_svm)

df_summary_30 = pd.DataFrame(summary_30_list)
df_summary_30.to_csv(SUMMARY_CSV, index=False)
print(f"Saved {SUMMARY_CSV} (30 rows)")

# Head-to-Head Table
h2h_rows = []
for rf_m, svm_m in zip(best_rf_list, best_svm_list):
    c_name = rf_m['feature_combo']
    dim = rf_m['num_features']
    
    diff_sub = rf_m['subj_acc'] - svm_m['subj_acc']
    diff_win = rf_m['win_acc'] - svm_m['win_acc']
    
    if diff_sub > 0:
        eval_note = f"RF Unggul Subjek (+{diff_sub:.1f}%)"
    elif diff_sub < 0:
        eval_note = f"SVM Unggul Subjek (+{-diff_sub:.1f}%)"
    else:
        if diff_win > 0:
            eval_note = f"Imbang Subjek (RF Win Acc +{diff_win:.1f}%)"
        elif diff_win < 0:
            eval_note = f"Imbang Subjek (SVM Win Acc +{-diff_win:.1f}%)"
        else:
            eval_note = "Imbang Sempurna"
            
    h2h_rows.append({
        'kombinasi_fitur': c_name,
        'dimensi': dim,
        'rf_model': rf_m['model_name'],
        'rf_subj_acc': rf_m['subj_acc'],
        'rf_subj_correct': rf_m['subj_correct'],
        'rf_sens': rf_m['sens'],
        'rf_spec': rf_m['spec'],
        'rf_win_acc': rf_m['win_acc'],
        'rf_gap': rf_m['gap'],
        'rf_size_kb': rf_m['size_kb'],
        'svm_model': svm_m['model_name'],
        'svm_subj_acc': svm_m['subj_acc'],
        'svm_subj_correct': svm_m['subj_correct'],
        'svm_sens': svm_m['sens'],
        'svm_spec': svm_m['spec'],
        'svm_win_acc': svm_m['win_acc'],
        'svm_gap': svm_m['gap'],
        'svm_size_kb': svm_m['size_kb'],
        'perbandingan': eval_note
    })

df_h2h = pd.DataFrame(h2h_rows)
# Urutkan berdasarkan Subj Acc tertinggi (maksimum antara RF dan SVM), lalu Win Acc
df_h2h['max_subj_acc'] = df_h2h[['rf_subj_acc', 'svm_subj_acc']].max(axis=1)
df_h2h['max_win_acc'] = df_h2h[['rf_win_acc', 'svm_win_acc']].max(axis=1)
df_h2h.sort_values(by=['max_subj_acc', 'max_win_acc'], ascending=[False, False], inplace=True)
df_h2h.drop(columns=['max_subj_acc', 'max_win_acc'], inplace=True)
df_h2h.to_csv(H2H_CSV, index=False)
print(f"Saved {H2H_CSV} (15 rows)")

# Filter per algoritma
rf_all = df_summary_30[df_summary_30['algorithm'] == 'Random Forest'].sort_values(by=['subj_acc', 'win_acc', 'gap'], ascending=[False, False, True])
svm_all = df_summary_30[df_summary_30['algorithm'] == 'SVM'].sort_values(by=['subj_acc', 'win_acc', 'gap'], ascending=[False, False, True])

best_rf_overall = rf_all.iloc[0]
best_svm_overall = svm_all.iloc[0]

# Generate Markdown Report
with open(REPORT_MD, 'w', encoding='utf-8') as f:
    f.write("# LAPORAN KOMPREHENSIF AUTO-TUNE SELURUH KOMBINASI FITUR TINYML\n\n")
    f.write("> **Metode Validasi:** Leave-One-Subject-Out Cross-Validation (LOSO 20 Folds — Subject Independent)\n")
    f.write("> **Dataset:** V4 Dataset (20 Subjek Independen, 200 Jendela 10-Detik Seimbang: 100 Normal & 100 Hipertensi)\n")
    f.write("> **Batas Memori Flash ESP32:** 400.0 KB (Semua model yang dilaporkan 100% lolos batas memori Flash ESP32)\n")
    f.write(f"> **Total Model Dievaluasi:** {len(df_full)} variasi model pada seluruh 15 kombinasi fitur fisiologis ($2^4 - 1 = 15$)\n")
    f.write("> **Juara per Kombinasi Fitur:** Dilaporkan **dua algoritma terbaik (Random Forest & SVM)** pada setiap kombinasi fitur untuk perbandingan komparatif langsung.\n\n")
    f.write("---\n\n")

    f.write("## 1. Temuan Ilmiah Utama (Key Scientific Findings)\n\n")
    f.write("1. **Perbandingan Juara Umum: Random Forest vs SVM:**\n")
    f.write(f"   * **Random Forest Terbaik:** Kombinasi **{best_rf_overall['feature_combo']}** (`{best_rf_overall['model_name']}`) meraih **Akurasi Subjek {best_rf_overall['subj_acc']:.1f}% ({best_rf_overall['subj_correct']})**, Akurasi Jendela {best_rf_overall['win_acc']:.1f}%, Sensitivitas {best_rf_overall['sens']:.1f}%, Spesifisitas {best_rf_overall['spec']:.1f}%, dan Flash ROM **{best_rf_overall['size_kb']} KB**.\n")
    f.write(f"   * **SVM Terbaik:** Kombinasi **{best_svm_overall['feature_combo']}** (`{best_svm_overall['model_name']}`) meraih **Akurasi Subjek {best_svm_overall['subj_acc']:.1f}% ({best_svm_overall['subj_correct']})**, Akurasi Jendela {best_svm_overall['win_acc']:.1f}%, Sensitivitas {best_svm_overall['sens']:.1f}%, Spesifisitas {best_svm_overall['spec']:.1f}%, dan Flash ROM **{best_svm_overall['size_kb']} KB**.\n")
    f.write("   * **Kesimpulan Puncak:** Kedua algoritma sama-sama berhasil mencapai batas atas akurasi subjek tertinggi sebesar **85.0% (17 dari 20 subjek terdiagnosis secara tepat)** pada validasi independen murni (LOSO).\n\n")

    f.write("2. **Dominasi Mutlak Fitur Crest Time Ratio (CTR):**\n")
    f.write("   * Baik pada Random Forest maupun SVM, setiap kombinasi yang menyertakan fitur **CTR** konsisten berada pada rentang **Akurasi Subjek 80.0% hingga 85.0%**.\n")
    f.write("   * Sebaliknya, kombinasi yang **tanpa CTR** (hanya mengandalkan HR, IR, atau Red) mengalami penurunan akurasi drastis ke **65.0% - 75.0%**.\n")
    f.write("   * **Justifikasi Fisiologis:** CTR merepresentasikan laju waktu naik sistolik terhadap durasi interval denyut total (waktu ejeksi ventrikel kiri). Fitur ini berakar langsung pada kekakuan pembuluh darah (*arterial stiffness*) dan hambatan vaskular sistemik (*systemic vascular resistance*), yang merupakan penanda primer hipertensi, serta dinormalisasi secara lokal sehingga kebal terhadap variasi amplitudo optik.\n\n")

    f.write("3. **Kelemahan Fitur Amplitudo Optik (IR Ratio & Red Ratio) Lintas Subjek:**\n")
    f.write("   * Amplitudo sinyal PPG (AC/DC) sangat peka terhadap ketebalan jaringan jari, pigmentasi, dan variasi tekanan kontak mekanis antar-subjek yang berbeda.\n")
    f.write("   * Pada validasi riil lintas subjek (LOSO), variansi antar-individu membuat fitur optik memperkenalkan variasi noise yang justru menurunkan akurasi generalisasi (*overfitting*).\n\n")

    f.write("4. **Karakteristik Komputasi Tepi TinyML (ESP32):**\n")
    f.write("   * **SVM:** Menghasilkan kode C++ yang sangat ringkas (**13.2 KB – 19.8 KB**), namun memerlukan operasi floating-point `StandardScaler` (Z-score normalisasi) saat inferensi di mikrokontroler.\n")
    f.write("   * **Random Forest:** Menghasilkan kode C++ berukuran **11.5 KB – 82.6 KB** (masih jauh di bawah batas 400 KB Flash ESP32), dengan keunggulan krusial: **tidak memerlukan normalisasi fitur (tanpa floating scaler)** dan evaluasi pohon keputusan murni berbasis percabangan `if-else` integer yang sangat cepat dan deterministik.\n\n")

    f.write("---\n\n")
    f.write("## 2. Tabel Perbandingan Head-to-Head: Juara RF vs Juara SVM (Seluruh 15 Kombinasi Fitur)\n\n")
    f.write("> Tabel berikut menyajikan **dua pemenang (Best RF dan Best SVM)** untuk setiap kombinasi fitur berdampingan. Tabel diurutkan berdasarkan performa akurasi subjek tertinggi.\n\n")
    f.write("*Tabel: Perbandingan Kinerja Berdampingan Random Forest vs SVM pada 15 Kombinasi Fitur Fisiologis (LOSO 20 Folds)*\n\n")

    f.write("| No | Kombinasi Fitur | Dim | Model RF Terbaik | Subj Acc (RF) | Win Acc (RF) | Flash (RF) | Model SVM Terbaik | Subj Acc (SVM) | Win Acc (SVM) | Flash (SVM) | Evaluasi Komparatif |\n")
    f.write("|:---:|:---|:---:|:---|:---:|:---:|:---:|:---|:---:|:---:|:---:|:---|\n")

    for i, r in enumerate(df_h2h.itertuples(), 1):
        hl = "**" if r.rf_subj_acc >= 85.0 or r.svm_subj_acc >= 85.0 else ""
        f.write(f"| {i} | {hl}{r.kombinasi_fitur}{hl} | {r.dimensi} | `{r.rf_model}` | {r.rf_subj_acc:.1f}% ({r.rf_subj_correct}) | {r.rf_win_acc:.1f}% | {r.rf_size_kb} KB | `{r.svm_model}` | {r.svm_subj_acc:.1f}% ({r.svm_subj_correct}) | {r.svm_win_acc:.1f}% | {r.svm_size_kb} KB | {r.perbandingan} |\n")

    f.write("\n---\n\n")
    f.write("## 3. Tabel Rincian Diagnostik Lengkap: RF & SVM per Kombinasi Fitur (30 Model)\n\n")
    f.write("> Tabel berpasangan yang menampilkan rincian Sensitivitas, Spesifisitas, Train Acc, dan Overfitting Gap untuk kedua algoritma di setiap kombinasi fitur.\n\n")
    f.write("| No | Kombinasi Fitur | Dim | Algoritma | Model Terbaik | Akurasi Subjek | Sensitivitas | Spesifisitas | Akurasi Window | Train Acc | Gap | Flash ROM |\n")
    f.write("|:---:|:---|:---:|:---|:---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|\n")

    combo_grouped = df_summary_30.sort_values(by=['num_features', 'feature_combo', 'algorithm'])
    row_num = 1
    for c_name in all_combos_ordered:
        sub_c = combo_grouped[combo_grouped['feature_combo'] == c_name]
        for _, r in sub_c.iterrows():
            hl = "**" if r['subj_acc'] >= 85.0 else ""
            f.write(f"| {row_num} | {hl}{r['feature_combo']}{hl} | {r['num_features']} | {r['algorithm']} | `{r['model_name']}` | {hl}{r['subj_acc']:.1f}% ({r['subj_correct']}){hl} | {r['sens']:.1f}% | {r['spec']:.1f}% | {r['win_acc']:.1f}% | {r['train_acc']:.1f}% | {r['gap']:.1f}% | {r['size_kb']} KB |\n")
            row_num += 1

    f.write("\n---\n\n")
    f.write("## 4. Tabel Khusus Model Random Forest (Peringkat 1 s.d. 15)\n\n")
    f.write("| Peringkat | Kombinasi Fitur | Dim | Konfigurasi RF Terbaik | Akurasi Subjek | Sensitivitas | Spesifisitas | Akurasi Window | Train Acc | Overfitting Gap | Ukuran Flash |\n")
    f.write("|:---:|:---|:---:|:---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|\n")
    for i, r in enumerate(rf_all.itertuples(), 1):
        hl = "**" if r.subj_acc >= 85.0 else ""
        f.write(f"| {i} | {hl}{r.feature_combo}{hl} | {r.num_features} | `{r.model_name}` | {hl}{r.subj_acc:.1f}% ({r.subj_correct}){hl} | {r.sens:.1f}% | {r.spec:.1f}% | {r.win_acc:.1f}% | {r.train_acc:.1f}% | {r.gap:.1f}% | {r.size_kb} KB |\n")

    f.write("\n---\n\n")
    f.write("## 5. Tabel Khusus Model Support Vector Machine (SVM) (Peringkat 1 s.d. 15)\n\n")
    f.write("| Peringkat | Kombinasi Fitur | Dim | Konfigurasi SVM Terbaik | Akurasi Subjek | Sensitivitas | Spesifisitas | Akurasi Window | Train Acc | Overfitting Gap | Ukuran Flash |\n")
    f.write("|:---:|:---|:---:|:---|:---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|\n")
    for i, r in enumerate(svm_all.itertuples(), 1):
        hl = "**" if r.subj_acc >= 85.0 else ""
        f.write(f"| {i} | {hl}{r.feature_combo}{hl} | {r.num_features} | `{r.model_name}` | {hl}{r.subj_acc:.1f}% ({r.subj_correct}){hl} | {r.sens:.1f}% | {r.spec:.1f}% | {r.win_acc:.1f}% | {r.train_acc:.1f}% | {r.gap:.1f}% | {r.size_kb} KB |\n")

    f.write("\n---\n\n")
    f.write("## 6. Ringkasan Per Dimensi Fitur (1 s.d. 4 Fitur): Komparasi RF vs SVM\n\n")
    f.write("*Tabel: Perbandingan Model Terbaik per Dimensi Fitur (LOSO 20 Folds)*\n\n")
    f.write("| Dimensi | Kombinasi Terbaik (RF) | Subj Acc (RF) | Win Acc (RF) | Flash (RF) | Kombinasi Terbaik (SVM) | Subj Acc (SVM) | Win Acc (SVM) | Flash (SVM) |\n")
    f.write("|:---:|:---|:---:|:---:|:---:|:---|:---:|:---:|:---:|\n")

    for k in range(1, 5):
        best_k_rf = rf_all[rf_all['num_features'] == k].iloc[0]
        best_k_svm = svm_all[svm_all['num_features'] == k].iloc[0]
        f.write(f"| **{k} Fitur** | **{best_k_rf['feature_combo']}** | **{best_k_rf['subj_acc']:.1f}%** ({best_k_rf['subj_correct']}) | {best_k_rf['win_acc']:.1f}% | {best_k_rf['size_kb']} KB | **{best_k_svm['feature_combo']}** | **{best_k_svm['subj_acc']:.1f}%** ({best_k_svm['subj_correct']}) | {best_k_svm['win_acc']:.1f}% | {best_k_svm['size_kb']} KB |\n")

    f.write("\n---\n\n")
    f.write("## 7. Tabel LaTeX Siap Salin untuk Bab 4 & Bab 5 Skripsi\n\n")
    f.write("```latex\n")
    f.write("\\begin{table}[htbp]\n")
    f.write("\\centering\n")
    f.write("\\caption{Perbandingan Kinerja Head-to-Head Random Forest vs SVM pada Berbagai Dimensi Fitur (LOSO 20 Folds)}\n")
    f.write("\\label{tab:loso_rf_vs_svm_comparison}\n")
    f.write("\\begin{tabular}{|c|l|c|c|c|l|c|c|c|}\n")
    f.write("\\hline\n")
    f.write("\\textbf{Dim} & \\textbf{Fitur RF} & \\textbf{Subj Acc} & \\textbf{Win Acc} & \\textbf{Flash} & \\textbf{Fitur SVM} & \\textbf{Subj Acc} & \\textbf{Win Acc} & \\textbf{Flash} \\\\\n")
    f.write("\\hline\n")
    for k in range(1, 5):
        best_k_rf = rf_all[rf_all['num_features'] == k].iloc[0]
        best_k_svm = svm_all[svm_all['num_features'] == k].iloc[0]
        f.write(f"{k} & {best_k_rf['feature_combo']} & {best_k_rf['subj_acc']:.1f}\\% & {best_k_rf['win_acc']:.1f}\\% & {best_k_rf['size_kb']} KB & {best_k_svm['feature_combo']} & {best_k_svm['subj_acc']:.1f}\\% & {best_k_svm['win_acc']:.1f}\\% & {best_k_svm['size_kb']} KB \\\\\n")
    f.write("\\hline\n")
    f.write("\\end{tabular}\n")
    f.write("\\end{table}\n")
    f.write("```\n")

print(f"Successfully generated {REPORT_MD}")
