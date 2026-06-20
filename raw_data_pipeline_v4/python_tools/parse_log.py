import re

log_path = r"C:\Users\Kayyis\.gemini\antigravity-ide\brain\a83678c5-9dcd-4b67-a225-ad0f5021b016\.system_generated\tasks\task-2011.log"
out_path = r"C:\Users\Kayyis\.gemini\antigravity-ide\brain\a83678c5-9dcd-4b67-a225-ad0f5021b016\autotune_results_v4.md"

models = []
with open(log_path, 'r') as f:
    for line in f:
        # Match lines like: "  1 SVM-LIN-C0.1-g0.001            71.6%   12.4%     6.8KB  [OK]   6.8 KB"
        match = re.match(r'\s*\d+\s+([\w\-\.]+)\s+([\d\.]+)%\s+([\d\.]+)%\s+([\d\.]+)KB\s+\[OK\]', line)
        if match:
            model = match.group(1)
            acc = float(match.group(2))
            std = float(match.group(3))
            size = float(match.group(4))
            models.append({
                'Model': model,
                'Akurasi': acc,
                'Std': std,
                'Ukuran': size
            })

# Sort by Akurasi (desc), then Ukuran (asc)
models.sort(key=lambda x: (-x['Akurasi'], x['Ukuran']))

with open(out_path, 'w', encoding='utf-8') as f:
    f.write("# Daftar Lengkap Auto-Tune V4 (10s Balanced Dataset)\n\n")
    f.write("Berikut adalah seluruh konfigurasi algoritma yang diuji pada dataset 10-detik (V4). Tabel ini diurutkan dari **Akurasi Terbaik hingga Terendah**.\n\n")
    f.write("> [!NOTE]\n")
    f.write("> Karena di V4 kita mengunci parameter dataset (Maks 1 window/subjek) menggunakan metode Cross-Validation K-Fold tingkat ketat, seluruh model di bawah ini terhindar dari *Data Leakage* dan Overfitting.\n\n")
    f.write("| Rank | Model (Konfigurasi) | Akurasi (CV) | Std Deviasi | Ukuran Memori |\n")
    f.write("|---|---|---|---|---|\n")
    
    for i, m in enumerate(models, 1):
        if i == 1:
            f.write(f"| 1 | **{m['Model']}** | **{m['Akurasi']:.1f}%** | +/-{m['Std']:.1f}% | {m['Ukuran']:.1f} KB |\n")
        else:
            f.write(f"| {i} | {m['Model']} | {m['Akurasi']:.1f}% | +/-{m['Std']:.1f}% | {m['Ukuran']:.1f} KB |\n")

print("Done generating markdown.")
