/*
 * ESP32 Blood Pressure Classification V4 (NO OLED VERSION)
 * 10-Second Truncated Window, Balanced Class Model
 *
 * Real-time blood pressure classification using Random Forest TinyML
 * Classification: Non-Hipertensi (0) vs Hipertensi (1)
 * Features: ir_ratio, red_ratio, crest_time_ratio, heart_rate
 *
 * Hardware: ESP32 + MAX30102 PPG Sensor
 *
 * [I2C ARCHITECTURE]
 * I2C1 (Wire1) -> MAX30102 (SDA 21, SCL 22) 
 */

#include "MAX30105.h"
#include <Wire.h>
#include "sqa_thresholds.h"
#include <math.h>

// Include the V4 Random Forest model
#include "rf_model_v4.h"

// I2C Pin Assignment (MAX30102 Sensor)
#define SENSOR_SDA 21
#define SENSOR_SCL 22

// Instansiasi perangkat keras
MAX30105 particleSensor;
Eloquent::ML::Port::RandomForest rf;

// Sensor configuration
const int SAMPLE_WINDOW = 1000; // EXACTLY 10 seconds at 100Hz
const int STATUS_LED = 2;

// Data buffers (Global Static to avoid Heap Fragmentation)
long irBuffer[SAMPLE_WINDOW];
long redBuffer[SAMPLE_WINDOW];
float irFilteredGlobal[SAMPLE_WINDOW];
float redFilteredGlobal[SAMPLE_WINDOW];
static double padXBufferGlobal[SAMPLE_WINDOW + 30];
static double padYBufferGlobal[SAMPLE_WINDOW + 30];

// Variabel Global
float g_heartRate = 0;
float g_ctr = 0;
float g_irRatio = 0;
unsigned long g_inferTime = 0;
char g_category[20] = "";


// ── DSP Filters ───────────────────────────────────────────────────────
void detrend_signal(float* buffer, int length) {
  double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
  for (int i = 0; i < length; i++) {
    double x = i; double y = buffer[i];
    sum_x += x; sum_y += y; sum_xy += x * y; sum_xx += x * x;
  }
  double n = length;
  double m = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
  double c = (sum_y - m * sum_x) / n;
  for (int i = 0; i < length; i++) buffer[i] = (float)(buffer[i] - (m * i + c));
}

// Filter Bandpass Butterworth Orde 2 (0.5 - 5.0 Hz pada 100 Hz)
// Implementasi Direct Form II Transposed (DF2T) dengan:
// 1. Gustafsson's Initial Conditions (zi)
// 2. 15-Sample Odd Extension Padding (scipy.signal.filtfilt parity 100%)
// 3. 64-bit double accumulator (zero roundoff / quantization error)
void filtfilt_bandpass(float* buffer, int length) {
  const int PADLEN = 15;
  const int TOTAL_LEN = length + 2 * PADLEN;
  
  // 1. Odd extension padding (refleksi ganjil di batas)
  for (int i = 0; i < PADLEN; i++) {
    padXBufferGlobal[i] = 2.0 * (double)buffer[0] - (double)buffer[PADLEN - i];
  }
  for (int i = 0; i < length; i++) {
    padXBufferGlobal[PADLEN + i] = (double)buffer[i];
  }
  for (int i = 0; i < PADLEN; i++) {
    padXBufferGlobal[PADLEN + length + i] = 2.0 * (double)buffer[length - 1] - (double)buffer[length - 2 - i];
  }
  
  // Koefisien Butterworth Bandpass Orde 2 (0.5 - 5.0 Hz pada 100 Hz)
  const double b[5] = {0.01658193, 0.0, -0.03316386, 0.0, 0.01658193};
  const double a[5] = {1.0, -3.58623981, 4.84628980, -2.93042722, 0.67045791};
  const double zi[4] = {-0.01658193, -0.01658193, 0.01658193, 0.01658193};
  
  // 2. Forward Pass (DF2T) dengan inisialisasi state zi * x_pad[0]
  double z[4];
  for (int k = 0; k < 4; k++) z[k] = zi[k] * padXBufferGlobal[0];
  for (int i = 0; i < TOTAL_LEN; i++) {
    double xn = padXBufferGlobal[i];
    double yn = b[0] * xn + z[0];
    z[0] = b[1] * xn - a[1] * yn + z[1];
    z[1] = b[2] * xn - a[2] * yn + z[2];
    z[2] = b[3] * xn - a[3] * yn + z[3];
    z[3] = b[4] * xn - a[4] * yn;
    padYBufferGlobal[i] = yn;
  }
  
  // 3. Backward Pass (DF2T) dengan inisialisasi state zi * y_pad[end]
  for (int k = 0; k < 4; k++) z[k] = zi[k] * padYBufferGlobal[TOTAL_LEN - 1];
  for (int i = TOTAL_LEN - 1; i >= 0; i--) {
    double xn = padYBufferGlobal[i];
    double yn = b[0] * xn + z[0];
    z[0] = b[1] * xn - a[1] * yn + z[1];
    z[1] = b[2] * xn - a[2] * yn + z[2];
    z[2] = b[3] * xn - a[3] * yn + z[3];
    z[3] = b[4] * xn - a[4] * yn;
    padYBufferGlobal[i] = yn;
  }
  
  // 4. Salin kembali dan inversi polaritas (-filtfilt) agar 100% selaras dengan Python
  for (int i = 0; i < length; i++) {
    buffer[i] = -(float)padYBufferGlobal[PADLEN + i];
  }
}

float get_smoothed_val(float* buffer, int i, int length) {
  if (i < 2 || i >= length - 2) return buffer[i];
  return (buffer[i-2] + buffer[i-1] + buffer[i] + buffer[i+1] + buffer[i+2]) / 5.0f;
}

// Menghitung Crest Time Ratio, Heart Rate, serta metrik Kualitas Sinyal (Time CV & Amp CV)
float get_ctr_and_hr(float* buffer, int length, float std_val, float& out_hr, float& out_time_cv, float& out_amp_cv) {
  int peak_indices[100];
  int peak_count = 0;
  int min_dist = 35; // Selaras dengan Python distance=35 (mampu deteksi HR hingga 171 BPM)
  int last_peak_idx = -min_dist;
  float peak_threshold = std_val * 0.5f; 
  
  // Exclude 20 sampel di awal dan akhir (0.2s) untuk mencegah border artifact
  for (int i = 20; i < length - 20; i++) {
    float val_prev = get_smoothed_val(buffer, i-1, length);
    float val_curr = get_smoothed_val(buffer, i, length);
    float val_next = get_smoothed_val(buffer, i+1, length);
    
    if (val_curr > val_prev && val_curr > val_next) { 
      if (buffer[i] > peak_threshold) { 
        if (i - last_peak_idx >= min_dist) { 
          peak_indices[peak_count++] = i;
          last_peak_idx = i;
          if (peak_count >= 100) break;
        }
      }
    }
  }

  if (peak_count < 2) {
    out_hr = 0;
    return 0; 
  }

  float total_rise_time = 0;
  int valid_beats = 0;
  
  for (int c = 0; c < peak_count; c++) {
    int p_idx = peak_indices[c];
    int start_search = (p_idx - 45 > 0) ? (p_idx - 45) : 0;
    int trough_idx = p_idx;
    for (int j = p_idx - 1; j >= start_search; j--) {
      if (buffer[j] <= buffer[trough_idx]) {
        trough_idx = j;
      } else {
        break;
      }
    }
    if (p_idx > trough_idx) {
      float rise_time_ms = (p_idx - trough_idx) * 10.0f; 
      total_rise_time += rise_time_ms;
      valid_beats++;
    }
  }

  float avg_rise_time = (valid_beats > 0) ? (total_rise_time / valid_beats) : 0;

  float total_rr = 0;
  for (int c = 1; c < peak_count; c++) total_rr += (peak_indices[c] - peak_indices[c-1]);
  
  float avg_rr_samples = total_rr / (peak_count - 1);
  float avg_rr_seconds = avg_rr_samples / 100.0f; 
  out_hr = 60.0f / avg_rr_seconds;

  // SQA 1: Kalkulasi Time CV (Jitter RR interval)
  float var_rr = 0;
  for (int c = 1; c < peak_count; c++) {
    float rr = (float)(peak_indices[c] - peak_indices[c-1]);
    var_rr += (rr - avg_rr_samples) * (rr - avg_rr_samples);
  }
  float std_rr = sqrt(var_rr / (peak_count - 1));
  out_time_cv = (avg_rr_samples > 0) ? ((std_rr / avg_rr_samples) * 100.0f) : 0;

  // SQA 2: Kalkulasi Amp CV (Pressure Shift)
  float total_amp = 0;
  for (int c = 0; c < peak_count; c++) {
    total_amp += buffer[peak_indices[c]];
  }
  float avg_amp = total_amp / peak_count;
  
  float var_amp = 0;
  for (int c = 0; c < peak_count; c++) {
    float amp = buffer[peak_indices[c]];
    var_amp += (amp - avg_amp) * (amp - avg_amp);
  }
  float std_amp = sqrt(var_amp / peak_count);
  out_amp_cv = (avg_amp > 0) ? (std_amp / avg_amp) : 0;

  // Hitung Crest Time Ratio (CTR)
  float ctr = 0;
  if (out_hr > 0) {
      ctr = avg_rise_time / (60000.0f / out_hr);
  }

  return ctr;
}

// ── SETUP ────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  unsigned long startSerial = millis();
  while (!Serial && millis() - startSerial < 1000); // Timeout 1s agar aman running standalone tanpa PC

  pinMode(STATUS_LED, OUTPUT);

  Serial.println("\n\n====================================");
  Serial.println("ESP32 Blood Pressure Classifier V4");
  Serial.println("RANDOM FOREST - NO-OLED EDITION");
  Serial.println("====================================");

  // Inisialisasi I2C (Wire) untuk SENSOR MAX30102
  Wire.begin(SENSOR_SDA, SENSOR_SCL);
  Wire.setClock(400000);

  while (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("   ERROR: MAX30102 not found on I2C! Retrying...");
    digitalWrite(STATUS_LED, HIGH); delay(500); digitalWrite(STATUS_LED, LOW); delay(500);
  }

  particleSensor.setup(0x1F, 1, 2, 100, 411, 4096);
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);
  Serial.println("   OK - Sensor initialized on I2C\n");
}

// ── TUGAS UTAMA: MEMBACA SENSOR DAN INFERENSI ──────────────────────────
void loop() {
  Serial.println("\n[STATE] Waiting for finger...");

  while (true) {
    long irCheck = 0;
    if (particleSensor.available()) {
      irCheck = particleSensor.getIR();
      particleSensor.getRed();  
      particleSensor.nextSample();
    } else {
      particleSensor.check();
    }
    
    if (irCheck > 50000) {
      Serial.println("[STATE] Finger detected!");
      break;  
    }
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    delay(50);
  }

  Serial.println("[STATE] Settling time: Discarding first 100 samples (1 second)...");
  for (int i = 0; i < 100; i++) {
    particleSensor.check();
    if (particleSensor.available()) {
      particleSensor.getIR(); particleSensor.getRed(); particleSensor.nextSample();
    }
    delay(10);
  }

  Serial.println("[STATE] Collecting 1000 samples (10 seconds) ZERO LATENCY...");
  digitalWrite(STATUS_LED, LOW);

  // PENGAMBILAN DATA BERBASIS HARDWARE FIFO SENSOR (100 Hz MURNI - ZERO DUPLICATE)
  int samplesCollected = 0;
  unsigned long collectionStart = millis();

  while (samplesCollected < SAMPLE_WINDOW) {
    particleSensor.check();

    while (particleSensor.available() && samplesCollected < SAMPLE_WINDOW) {
      irBuffer[samplesCollected] = particleSensor.getFIFOIR();
      redBuffer[samplesCollected] = particleSensor.getFIFORed();
      particleSensor.nextSample();

      // EARLY ABORT: Jari terlepas
      if (irBuffer[samplesCollected] < 40000) {
        Serial.println("\n❌ ABORTED: Jari terlepas dari sensor di tengah jalan!");
        Serial.println("   Silakan letakkan jari kembali untuk mengulang.\n");
        for (int k = 0; k < 5; k++) {
          digitalWrite(STATUS_LED, HIGH); delay(100);
          digitalWrite(STATUS_LED, LOW); delay(100);
        }
        return; // Kembali ke awal loop()
      }

      samplesCollected++;
      if (samplesCollected > 0 && samplesCollected % 200 == 0) {
        Serial.printf("        Progress: %d%%\n", (samplesCollected * 100) / SAMPLE_WINDOW);
      }
    }

    // Safety timeout: jika dalam 13 detik belum terkumpul 1000 sampel (sensor macet / I2C error)
    if (millis() - collectionStart > 13000) {
      Serial.println("\n❌ TIMEOUT: Sensor terhenti atau pengumpulan data macet!");
      delay(2000);
      return;
    }

    delayMicroseconds(500); // Polling cepat 0.5ms tanpa membebani CPU
  }
  Serial.println("        Progress: 100%");

  digitalWrite(STATUS_LED, HIGH);
  Serial.println("[STATE] Data collected! Processing DSP Filters...");

  float irSum = 0, redSum = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) { irSum += irBuffer[i]; redSum += redBuffer[i]; }
  float irMean = irSum / SAMPLE_WINDOW; float redMean = redSum / SAMPLE_WINDOW;

  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irFilteredGlobal[i] = (float)irBuffer[i]; redFilteredGlobal[i] = (float)redBuffer[i];
  }

  detrend_signal(irFilteredGlobal, SAMPLE_WINDOW); detrend_signal(redFilteredGlobal, SAMPLE_WINDOW);
  filtfilt_bandpass(irFilteredGlobal, SAMPLE_WINDOW); filtfilt_bandpass(redFilteredGlobal, SAMPLE_WINDOW);

  // Inversi polaritas sinyal (-filtfilt) sudah dilakukan langsung di dalam filtfilt_bandpass

  float irFiltMean = 0, redFiltMean = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) { irFiltMean += irFilteredGlobal[i]; redFiltMean += redFilteredGlobal[i]; }
  irFiltMean /= SAMPLE_WINDOW; redFiltMean /= SAMPLE_WINDOW;

  float irVar = 0, redVar = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irVar += (irFilteredGlobal[i] - irFiltMean) * (irFilteredGlobal[i] - irFiltMean);
    redVar += (redFilteredGlobal[i] - redFiltMean) * (redFilteredGlobal[i] - redFiltMean);
  }
  float irStd = sqrt(irVar / SAMPLE_WINDOW); float redStd = sqrt(redVar / SAMPLE_WINDOW);

  float g_time_cv = 0;
  float g_amp_cv = 0;
  g_ctr = get_ctr_and_hr(irFilteredGlobal, SAMPLE_WINDOW, irStd, g_heartRate, g_time_cv, g_amp_cv);
  g_irRatio = (irMean > 0) ? ((irStd / irMean) * 100.0f) : 0;
  float redRatio = (redMean > 0) ? ((redStd / redMean) * 100.0f) : 0;

  // ── SQA / Signal Validation ──────────────────────────────────────────
  if (g_heartRate < 40.0f || g_heartRate > 180.0f || g_ctr <= 0.0f || g_time_cv > MAX_TIME_CV || g_amp_cv > MAX_AMP_CV) {
    Serial.printf("\n==============================================\n");
    Serial.printf("❌ SQA REJECTED! SIGNAL TOO NOISY\n");
    Serial.printf("   HR     : %.1f BPM\n", g_heartRate);
    Serial.printf("   TimeCV : %.1f %%\n", g_time_cv);
    Serial.printf("   AmpCV  : %.3f\n", g_amp_cv);
    Serial.printf("   Action : Please keep your finger still.\n");
    Serial.printf("==============================================\n\n");
    delay(3500);
    return; 
  }

  Serial.printf("\n✅ SQA PASSED! Signal Quality is Good.\n");
  Serial.printf("   TimeCV : %.1f %%\n", g_time_cv);
  Serial.printf("   AmpCV  : %.3f\n", g_amp_cv);

  // --- KLASIFIKASI TINYML (RANDOM FOREST) ---
  // Catatan: RF tidak memerlukan normalisasi Z-Score!
  float raw_features[4] = { g_irRatio, redRatio, g_ctr, g_heartRate };
  
  unsigned long mlStart = micros();
  int prediction = rf.predict(raw_features);
  g_inferTime = micros() - mlStart;
  
  strcpy(g_category, (prediction == 0) ? "NON-HIPERTENSI" : "HIPERTENSI");

  Serial.printf("\n==============================================\n");
  Serial.printf("   > FEATURES EXTRACTED <\n");
  Serial.printf("   BPM       : %.2f\n", g_heartRate);
  Serial.printf("   CTR       : %.4f\n", g_ctr);
  Serial.printf("   IR Ratio  : %.4f\n", g_irRatio);
  Serial.printf("   Red Ratio : %.4f\n", redRatio);
  Serial.printf("   Time CV   : %.2f %%\n", g_time_cv);
  Serial.printf("   Amp CV    : %.3f\n", g_amp_cv);
  Serial.printf("----------------------------------------------\n");
  Serial.printf("   > CLASSIFICATION V4 (RF) <\n");
  Serial.printf("   Result    : %s (kelas %d)\n", g_category, prediction);
  Serial.printf("   Infer Time: %lu us\n", g_inferTime);
  Serial.printf("==============================================\n");
  // Format CSV (bisa di-copy langsung ke spreadsheet):
  // ir_ratio, red_ratio, crest_time_ratio, heart_rate, time_cv, amp_cv, prediksi
  Serial.printf("CSV: %.4f,%.4f,%.4f,%.2f,%.2f,%.3f,%d\n\n", g_irRatio, redRatio, g_ctr, g_heartRate, g_time_cv, g_amp_cv, prediction);
  
  delay(5000); 
}
