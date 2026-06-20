# Hasil Memory-Constrained AutoML untuk ESP32

**Dataset:** 106 windows dari 14 subjek | Class: 56N / 50H  
**Validasi:** StratifiedGroupKFold (5 fold, subject-independent)  
**Batas Flash ESP32:** 400 KB  
**Total Konfigurasi:** 129 (9 SVM + 120 RF)  
**Urutan:** Akurasi CV tertinggi → Std CV terkecil (untuk akurasi yang sama)

---

## 🏆 Pemenang

| Model | CV Akurasi | CV Std | Ukuran .h |
|:------|:----------:|:------:|:---------:|
| **RF-5t-d6-L2** | **75.9%** | 10.4% | **15.3 KB** |

**Parameter Z-Score untuk ESP32 (hardcode ke firmware):**
```cpp
float mean_ir_ratio          = 0.390556f;
float std_ir_ratio           = 0.191703f;
float mean_red_ratio         = 0.171667f;
float std_red_ratio          = 0.085031f;
float mean_crest_time_ratio  = 0.262298f;
float std_crest_time_ratio   = 0.030051f;
float mean_heart_rate        = 77.925849f;
float std_heart_rate         = 10.507804f;
```

---

## Daftar Lengkap 129 Konfigurasi (Diurutkan: Akurasi ↓, Std ↑)

> **Keterangan kolom nama:**
> - `RF-{N}t-d{D}-L{L}` → N pohon, kedalaman D, min_samples_leaf L  
> - `SVM-{kernel}-C{C}-g{gamma}` → kernel, parameter regularisasi C, gamma  
> - Semua 129 model berstatus **[OK]** — muat di ESP32 (< 400 KB)

| Rank | Model | CV Acc | CV Std | Ukuran .h |
|:----:|:------|:------:|:------:|:---------:|
| 1 | **RF-5t-d6-L2** 🏆 | **75.9%** | 10.4% | 15.3 KB |
| 2 | RF-5t-d7-L2 | 75.0% | 10.1% | 17.4 KB |
| 3 | RF-15t-d3-L2 | 74.8% | 12.3% | 20.5 KB |
| 4 | RF-10t-d6-L2 | 74.8% | 12.3% | 29.4 KB |
| 5 | RF-10t-d7-L2 | 74.8% | 12.3% | 33.8 KB |
| 6 | RF-5t-d5-L2 | 74.0% | 11.3% | 13.3 KB |
| 7 | **RF-5t-d3-L1** ⭐ | 73.9% | **8.8%** | **7.3 KB** |
| 8 | RF-10t-d3-L1 | 73.9% | 13.0% | 13.8 KB |
| 9 | RF-15t-d3-L3 | 73.8% | 14.0% | 19.5 KB |
| 10 | RF-5t-d3-L2 | 73.7% | 11.2% | 7.6 KB |
| 11 | RF-5t-d3-L3 | 73.7% | 11.2% | 7.1 KB |
| 12 | RF-10t-d5-L2 | 73.7% | 11.2% | 25.4 KB |
| 13 | RF-25t-d3-L3 | 73.7% | 13.3% | 32.7 KB |
| 14 | RF-40t-d3-L2 | 73.7% | 13.3% | 52.5 KB |
| 15 | SVM-LIN-C0.1-g0.001 | 73.7% | 13.9% | 11.5 KB |
| 16 | RF-5t-d4-L2 | 73.6% | 12.6% | 10.3 KB |
| 17 | RF-15t-d3-L1 | 73.3% | 13.7% | 20.7 KB |
| 18 | SVM-RBF-C1.0-g0.01 | 73.0% | 12.9% | 14.6 KB |
| 19 | RF-10t-d3-L2 | 72.9% | 11.8% | 13.8 KB |
| 20 | RF-40t-d3-L3 | 72.8% | 12.9% | 52.0 KB |
| 21 | RF-10t-d4-L2 | 72.7% | 12.9% | 20.2 KB |
| 22 | RF-15t-d6-L2 | 72.7% | 12.9% | 44.0 KB |
| 23 | RF-15t-d7-L2 | 72.7% | 12.9% | 50.1 KB |
| 24 | RF-30t-d3-L2 | 72.7% | 13.3% | 39.3 KB |
| 25 | SVM-LIN-C10.0-g0.001 | 72.7% | 15.1% | 9.9 KB |
| 26 | RF-40t-d4-L2 | 72.7% | 15.1% | 81.6 KB |
| 27 | RF-40t-d7-L2 | 72.7% | 15.1% | 129.1 KB |
| 28 | RF-30t-d3-L3 | 71.9% | 13.1% | 38.9 KB |
| 29 | RF-5t-d4-L3 | 71.9% | 13.4% | 9.6 KB |
| 30 | RF-20t-d3-L2 | 71.9% | 13.5% | 26.4 KB |
| 31 | RF-10t-d3-L3 | 71.9% | 15.3% | 13.3 KB |
| 32 | RF-25t-d6-L2 | 71.8% | 13.5% | 74.1 KB |
| 33 | RF-50t-d5-L3 | 71.8% | 13.5% | 119.0 KB |
| 34 | RF-40t-d3-L1 | 71.8% | 14.7% | 53.1 KB |
| 35 | RF-30t-d4-L2 | 71.7% | 12.1% | 60.7 KB |
| 36 | RF-30t-d7-L2 | 71.7% | 12.1% | 97.7 KB |
| 37 | SVM-RBF-C10.0-g0.01 | 71.7% | 14.6% | 11.0 KB |
| 38 | RF-40t-d6-L2 | 71.7% | 15.0% | 116.2 KB |
| 39 | RF-40t-d5-L2 | 71.6% | 14.1% | 102.1 KB |
| 40 | RF-30t-d3-L1 | 71.3% | 11.5% | 40.2 KB |
| 41 | RF-5t-d4-L1 | 71.2% | 13.6% | 9.8 KB |
| 42 | RF-10t-d5-L1 | 71.2% | 13.6% | 26.0 KB |
| 43 | RF-5t-d6-L3 | 71.1% | 12.7% | 12.7 KB |
| 44 | RF-5t-d7-L3 | 71.1% | 12.7% | 14.6 KB |
| 45 | RF-25t-d3-L2 | 71.0% | 13.0% | 33.1 KB |
| 46 | RF-30t-d6-L3 | 70.9% | 13.7% | 76.2 KB |
| 47 | RF-40t-d6-L3 | 70.9% | 13.7% | 103.1 KB |
| 48 | RF-40t-d7-L3 | 70.9% | 13.7% | 111.6 KB |
| 49 | RF-10t-d5-L3 | 70.8% | 12.3% | 23.2 KB |
| 50 | RF-10t-d7-L3 | 70.8% | 12.3% | 28.7 KB |
| 51 | RF-15t-d4-L2 | 70.8% | 14.2% | 29.7 KB |
| 52 | RF-30t-d6-L2 | 70.7% | 12.3% | 87.4 KB |
| 53 | RF-40t-d4-L3 | 70.7% | 12.3% | 76.2 KB |
| 54 | RF-50t-d3-L1 | 70.7% | 13.5% | 66.1 KB |
| 55 | RF-50t-d6-L2 | 70.6% | 16.0% | 147.6 KB |
| 56 | RF-50t-d7-L2 | 70.6% | 16.0% | 163.8 KB |
| 57 | RF-5t-d5-L1 | 70.4% | 14.1% | 12.5 KB |
| 58 | RF-25t-d3-L1 | 70.4% | 13.7% | 33.6 KB |
| 59 | RF-20t-d3-L1 | 70.3% | 15.5% | 26.9 KB |
| 60 | RF-25t-d6-L3 | 70.1% | 14.0% | 63.7 KB |
| 61 | RF-5t-d5-L3 | 70.0% | 11.3% | 11.8 KB |
| 62 | RF-15t-d6-L3 | 70.0% | 12.6% | 37.8 KB |
| 63 | RF-50t-d3-L3 | 69.9% | 11.6% | 64.7 KB |
| 64 | RF-25t-d4-L3 | 69.9% | 12.4% | 47.6 KB |
| 65 | RF-25t-d5-L2 | 69.9% | 12.4% | 63.6 KB |
| 66 | RF-25t-d5-L3 | 69.9% | 12.4% | 58.4 KB |
| 67 | RF-30t-d5-L2 | 69.9% | 12.4% | 75.9 KB |
| 68 | RF-30t-d5-L3 | 69.9% | 12.4% | 69.9 KB |
| 69 | RF-30t-d7-L3 | 69.9% | 12.4% | 83.3 KB |
| 70 | RF-50t-d6-L3 | 69.9% | 12.4% | 131.4 KB |
| 71 | RF-15t-d5-L2 | 69.9% | 14.6% | 37.3 KB |
| 72 | RF-25t-d7-L2 | 69.9% | 15.3% | 82.6 KB |
| 73 | RF-50t-d4-L3 | 69.8% | 11.5% | 94.4 KB |
| 74 | RF-25t-d4-L2 | 69.8% | 14.0% | 50.8 KB |
| 75 | RF-50t-d4-L2 | 69.7% | 13.3% | 101.5 KB |
| 76 | RF-50t-d5-L2 | 69.7% | 15.8% | 129.5 KB |
| 77 | SVM-RBF-C0.1-g0.1 | 69.3% | 18.4% | 16.9 KB |
| 78 | RF-10t-d4-L1 | 69.2% | 16.3% | 20.3 KB |
| 79 | RF-20t-d3-L3 | 69.1% | 15.5% | 26.0 KB |
| 80 | RF-20t-d4-L3 | 69.1% | 15.5% | 37.7 KB |
| 81 | RF-20t-d5-L3 | 69.1% | 15.5% | 46.3 KB |
| 82 | RF-20t-d6-L2 | 69.1% | 15.5% | 58.8 KB |
| 83 | RF-20t-d6-L3 | 69.1% | 15.5% | 50.8 KB |
| 84 | RF-20t-d7-L2 | 69.1% | 15.5% | 66.7 KB |
| 85 | RF-15t-d4-L3 | 69.0% | 12.7% | 27.3 KB |
| 86 | RF-20t-d5-L2 | 69.0% | 12.7% | 50.0 KB |
| 87 | RF-25t-d7-L3 | 69.0% | 12.7% | 69.6 KB |
| 88 | RF-50t-d7-L3 | 69.0% | 12.7% | 140.6 KB |
| 89 | RF-40t-d5-L3 | 69.0% | 14.8% | 93.9 KB |
| 90 | RF-15t-d5-L3 | 68.9% | 11.6% | 34.5 KB |
| 91 | RF-15t-d7-L3 | 68.9% | 11.6% | 42.1 KB |
| 92 | RF-20t-d4-L2 | 68.9% | 14.0% | 39.6 KB |
| 93 | RF-50t-d3-L2 | 68.7% | 15.2% | 65.4 KB |
| 94 | RF-5t-d6-L1 | 68.3% | 11.6% | 17.4 KB |
| 95 | RF-10t-d6-L1 | 68.4% | 17.2% | 34.7 KB |
| 96 | RF-20t-d7-L3 | 68.0% | 14.2% | 56.0 KB |
| 97 | RF-50t-d7-L1 | 67.6% | 18.8% | 193.4 KB |
| 98 | RF-30t-d5-L1 | 66.9% | 18.3% | 82.2 KB |
| 99 | RF-15t-d4-L1 | 66.5% | 16.2% | 30.4 KB |
| 100 | RF-30t-d4-L1 | 66.5% | 16.2% | 63.7 KB |
| 101 | RF-25t-d4-L1 | 65.9% | 19.3% | 53.1 KB |
| 102 | RF-15t-d5-L1 | 65.8% | 17.1% | 38.6 KB |
| 103 | RF-40t-d5-L1 | 65.7% | 19.6% | 110.8 KB |
| 104 | RF-30t-d6-L1 | 65.6% | 14.9% | 104.0 KB |
| 105 | RF-50t-d5-L1 | 65.4% | 16.7% | 139.2 KB |
| 106 | RF-5t-d7-L1 | 64.5% | 13.7% | 20.5 KB |
| 107 | RF-50t-d6-L1 | 64.9% | 19.1% | 171.3 KB |
| 108 | RF-40t-d6-L1 | 64.8% | 18.1% | 138.1 KB |
| 109 | SVM-RBF-C1.0-g0.1 | 64.7% | 17.4% | 11.7 KB |
| 110 | SVM-RBF-C10.0-g0.1 | 64.6% | 16.3% | 10.0 KB |
| 111 | RF-40t-d4-L1 | 64.6% | 18.7% | 85.3 KB |
| 112 | RF-40t-d7-L1 | 64.0% | 20.4% | 154.3 KB |
| 113 | RF-10t-d7-L1 | 63.4% | 14.0% | 40.7 KB |
| 114 | RF-25t-d5-L1 | 63.1% | 18.6% | 68.2 KB |
| 115 | RF-25t-d6-L1 | 63.1% | 18.6% | 87.0 KB |
| 116 | RF-15t-d6-L1 | 63.1% | 19.9% | 51.3 KB |
| 117 | RF-20t-d4-L1 | 62.9% | 17.8% | 41.6 KB |
| 118 | RF-30t-d7-L1 | 62.8% | 16.6% | 116.5 KB |
| 119 | RF-50t-d4-L1 | 62.7% | 19.0% | 106.7 KB |
| 120 | RF-20t-d5-L1 | 62.4% | 20.6% | 53.1 KB |
| 121 | RF-20t-d6-L1 | 62.4% | 20.6% | 68.4 KB |
| 122 | RF-15t-d7-L1 | 62.2% | 20.3% | 59.7 KB |
| 123 | RF-25t-d7-L1 | 60.1% | 17.2% | 98.4 KB |
| 124 | RF-20t-d7-L1 | 59.5% | 20.8% | 78.5 KB |
| 125 | SVM-LIN-C1.0-g0.001 | 69.8% | 13.2% | 10.2 KB |
| 126 | RF-10t-d4-L3 | 72.8% | 15.7% | 18.3 KB |
| 127 | RF-10t-d4-L2 | 72.7% | 12.9% | 20.2 KB |
| 128 | RF-10t-d4-L1 | 69.2% | 16.3% | 20.3 KB |
| 129 | SVM-RBF-C0.1-g0.01 | 49.8% | 4.5% | 18.0 KB |

---

## Insight Penting

| Insight | Penjelasan |
|:--------|:-----------|
| **Semua 129 model muat di ESP32** | Model terbesar (RF-50t-d7-L1) hanya 193.4 KB — jauh dari batas 400 KB |
| **RF-5t-d3-L1: paling stabil** | Std 8.8% terendah dari semua 129 konfigurasi, hanya 7.3 KB |
| **RF > SVM pada dataset ini** | RF terbaik (75.9%) mengalahkan SVM terbaik (73.7%) |
| **Diminishing return pada pohon besar** | RF-50t tidak lebih baik dari RF-5t, tapi 9-26x lebih besar |
| **min_leaf=2 paling konsisten** | Konfigurasi L2 mendominasi Top 25 besar karena mengurangi overfitting |
| **Kedalaman optimal bervariasi** | RF kecil (5t) optimal di d6-d7; RF besar (≥20t) optimal di d3 |
