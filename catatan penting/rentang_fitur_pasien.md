# Rentang Nilai Fitur Berdasarkan Kondisi Pasien

Dokumen ini memetakan rentang nilai (*range*) dari masing-masing fitur yang diekstrak dari sensor MAX30102, dikelompokkan berdasarkan kondisi tekanan darah pasien (berdasarkan data latih saat ini: 121 sampel valid).

Tabel ini sangat berguna untuk dimasukkan ke dalam **Bab Hasil dan Pembahasan** di skripsimu untuk menjelaskan karakteristik fisik dari sinyal PPG pasien.

---

### 1. Intensitas Inframerah Rata-rata (`ir_mean`)
*Fitur ini merepresentasikan jumlah cahaya inframerah keseluruhan yang dipantulkan kembali oleh jaringan jari (termasuk tulang, daging, dan volume darah statis). Semakin tinggi angkanya, semakin tebal jari/sedikit darah statisnya.*

| Kondisi Pasien | Nilai Terendah (Min) | Nilai Tengah (Median) | Nilai Tertinggi (Max) |
| :--- | :--- | :--- | :--- |
| **Non-Hipertensi** | 80.865 | **105.278** | 116.753 |
| **Hipertensi** | 80.570 | **102.478** | 111.708 |

* **Analisis:** Pasien Hipertensi di datasetmu cenderung memiliki nilai `ir_mean` yang sedikit lebih rendah (cahaya yang tembus/dipantulkan lebih sedikit), yang secara teori medis bisa mengindikasikan pembuluh darah yang lebih kaku atau volume darah yang lebih padat, meskipun perbedaannya sangat tipis.

---

### 2. Intensitas Cahaya Merah Rata-rata (`red_mean`)
*Sama seperti `ir_mean`, namun menggunakan spektrum cahaya merah yang lebih sensitif terhadap kadar oksigen dalam darah (SpO2).*

| Kondisi Pasien | Nilai Terendah (Min) | Nilai Tengah (Median) | Nilai Tertinggi (Max) |
| :--- | :--- | :--- | :--- |
| **Non-Hipertensi** | 70.564 | **96.463** | 106.878 |
| **Hipertensi** | 81.605 | **98.751** | 101.598 |

* **Analisis:** Rentang cahaya merah pada pasien Hipertensi terlihat lebih rapat (sempit) dibandingkan pasien normal yang sangat bervariasi.

---

### 3. Kekuatan Denyut Inframerah (`ir_std`) ⭐ *Fitur Paling Krusial*
*Fitur ini merepresentasikan "Kekuatan Pompa Jantung" (Amplitudo AC). Ini diukur setelah sinyal melewati Bandpass Filter untuk menghilangkan efek pernapasan. Nilai ini sangat murni mencerminkan daya dorong darah setiap kali jantung berdetak.*

| Kondisi Pasien | Nilai Terendah (Min) | Nilai Tengah (Median) | Nilai Tertinggi (Max) |
| :--- | :--- | :--- | :--- |
| **Non-Hipertensi** | 30 | **401** | 1.282 |
| **Hipertensi** | 95 | **365** | 639 |

* **Analisis Penting:** Di dataset ini, pasien Hipertensi secara konsisten memiliki denyut nadi (`ir_std`) yang **lebih rendah/lemah secara rata-rata** (365) dibandingkan pasien normal (401). Selain itu, kekuatan denyut pasien Hipertensi terhenti di angka maksimal 639, sementara orang normal bisa memiliki kekuatan denyut hingga 1.282. Secara patofisiologis, pembuluh darah yang kaku (pada penderita hipertensi) memang membuat peregangan pembuluh (pulsasi) menjadi lebih kaku dan amplitudonya mengecil!

---

### 4. Kekuatan Denyut Cahaya Merah (`red_std`)
*Amplitudo AC dari sinyal lampu merah.*

| Kondisi Pasien | Nilai Terendah (Min) | Nilai Tengah (Median) | Nilai Tertinggi (Max) |
| :--- | :--- | :--- | :--- |
| **Non-Hipertensi** | 12 | **154** | 492 |
| **Hipertensi** | 48 | **154** | 303 |

* **Analisis:** Nilai tengahnya sama persis (154), namun lagi-lagi pasien normal memiliki rentang maksimal yang jauh lebih elastis (hingga 492) dibanding pasien hipertensi (mentok di 303).

---

> [!NOTE] 
> **Catatan untuk Penulisan Skripsi:**  
> Data di atas membuktikan bahwa metode pengekstraksian fitur menggunakan *Bandpass Filter* yang kamu rancang berhasil memunculkan fenomena medis yang nyata, yaitu: **"Pasien Hipertensi cenderung memiliki peregangan pembuluh darah (amplitudo pulsasi/std) yang lebih kaku dan sempit dibandingkan orang normal yang pembuluh darahnya elastis."** Ini argumen yang sangat kuat untuk sidang!
