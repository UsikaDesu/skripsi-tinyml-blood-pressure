/*
 * ESP32 Blood Pressure Classification V4 (Ultimate Pipeline)
 * RTOS DUAL-CORE + HARDWARE DUAL I2C BUS EDITION (ZERO LATENCY)
 * 10-Second Truncated Window, Balanced Class Model
 *
 * Real-time blood pressure classification using Random Forest TinyML
 * Classification: Non-Hipertensi (0) vs Hipertensi (1)
 * Features: ir_ratio, red_ratio, crest_time_ratio, heart_rate
 *
 * Hardware: ESP32 + MAX30102 PPG Sensor + 0.96" 128x64 OLED (SSD1306)
 *
 * [ULTIMATE I2C ARCHITECTURE]
 * I2C0 (Wire)  -> OLED (SDA 21, SCL 22) -> Controlled by Core 0
 * I2C1 (Wire1) -> MAX30102 (SDA 32, SCL 33) -> Controlled by Core 1
 */

#include "MAX30105.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <math.h>

// Include the V4 Random Forest model
#include "rf_model_v4.h"

// I2C0 Pin Assignment (OLED)
#define OLED_SDA 21
#define OLED_SCL 22

// I2C1 Pin Assignment (MAX30102 Sensor)
#define SENSOR_SDA 32
#define SENSOR_SCL 33

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1      
#define OLED_I2C_ADDR 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;
Eloquent::ML::Port::RandomForest rf;

// Z-Score Normalization DIBUANG: Random Forest tidak membutuhkannya!
// Menghemat siklus komputasi pembagian float secara drastis.

// Sensor configuration
const int SAMPLE_WINDOW = 1000; // EXACTLY 10 seconds at 100Hz
const int SAMPLE_RATE = 100;
const int STATUS_LED = 2;

// Data buffers (Global Static to avoid Heap Fragmentation)
long irBuffer[SAMPLE_WINDOW];
long redBuffer[SAMPLE_WINDOW];
float irFilteredGlobal[SAMPLE_WINDOW];
float redFilteredGlobal[SAMPLE_WINDOW];
float tempYBufferGlobal[SAMPLE_WINDOW];

// State Machine (Komunikasi antar Core)
enum AppState {
  STATE_INIT,
  STATE_WAITING_FINGER,
  STATE_SETTLING,
  STATE_COLLECTING,
  STATE_INFERENCING,
  STATE_RESULT,
  STATE_ERROR
};

volatile AppState currentState = STATE_INIT;
volatile int collectionProgress = 0;

// Variabel Global untuk ditampilkan di Layar (diisi oleh Core 1, dibaca oleh Core 0)
volatile float g_heartRate = 0;
volatile float g_ctr = 0;
volatile float g_irRatio = 0;
volatile unsigned long g_inferTime = 0;
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

void lfilter(float* b, float* a, float* x, float* y, int length) {
  for (int n = 0; n < length; n++) {
    float xn = x[n];
    float xn1 = (n >= 1) ? x[n-1] : 0; float xn2 = (n >= 2) ? x[n-2] : 0;
    float xn3 = (n >= 3) ? x[n-3] : 0; float xn4 = (n >= 4) ? x[n-4] : 0;
    float yn1 = (n >= 1) ? y[n-1] : 0; float yn2 = (n >= 2) ? y[n-2] : 0;
    float yn3 = (n >= 3) ? y[n-3] : 0; float yn4 = (n >= 4) ? y[n-4] : 0;
    y[n] = b[0]*xn + b[1]*xn1 + b[2]*xn2 + b[3]*xn3 + b[4]*xn4 - a[1]*yn1 - a[2]*yn2 - a[3]*yn3 - a[4]*yn4;
  }
}

void filtfilt_bandpass(float* buffer, int length) {
  float b[5] = {0.01658193f, 0.0f, -0.03316386f, 0.0f, 0.01658193f};
  float a[5] = {1.0f, -3.5862398f, 4.8462898f, -2.9304272f, 0.6704579f};
  lfilter(b, a, buffer, tempYBufferGlobal, length);
  for (int i = 0; i < length; i++) buffer[i] = tempYBufferGlobal[length - 1 - i];
  lfilter(b, a, buffer, tempYBufferGlobal, length);
  for (int i = 0; i < length; i++) buffer[i] = tempYBufferGlobal[length - 1 - i];
}

float get_smoothed_val(float* buffer, int i, int length) {
  if (i < 2 || i >= length - 2) return buffer[i];
  return (buffer[i-2] + buffer[i-1] + buffer[i] + buffer[i+1] + buffer[i+2]) / 5.0f;
}

// Menghitung Crest Time Ratio, Heart Rate, serta metrik Kualitas Sinyal (Time CV & Amp CV)
float get_ctr_and_hr(float* buffer, int length, float std_val, volatile float& out_hr, volatile float& out_time_cv, volatile float& out_amp_cv) {
  int peak_indices[100];
  int peak_count = 0;
  int min_dist = 50; 
  int last_peak_idx = -min_dist;
  float peak_threshold = std_val * 0.5f; 
  
  for (int i = 2; i < length - 2; i++) {
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
    float min_val = buffer[p_idx];
    for (int j = p_idx; j >= start_search; j--) {
      if (buffer[j] < min_val) { min_val = buffer[j]; trough_idx = j; }
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

// ── TUGAS CORE 0: MENGGAMBAR KE OLED (EKSKLUSIF DI I2C0) ──────────────
void oledTask(void *pvParameters) {
  for (;;) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    switch(currentState) {
      case STATE_WAITING_FINGER:
        display.setCursor(0, 0); display.println("  >> READY V4 <<");
        display.setCursor(0, 32); display.println("Place finger on");
        display.setCursor(0, 48); display.println("the sensor...");
        break;

      case STATE_SETTLING:
        display.setCursor(0, 0); display.println(" Stabilizing...");
        display.setCursor(0, 32); display.println("Keep finger still");
        display.setCursor(0, 48); display.println("Wait 1 sec...");
        break;

      case STATE_COLLECTING:
        display.setCursor(4, 0); display.println(" Collecting (10s)");
        display.setCursor(0, 16); display.printf("Progress: %d%%", collectionProgress);
        display.drawRect(0, 30, SCREEN_WIDTH, 10, SSD1306_WHITE);
        display.fillRect(2, 32, (SCREEN_WIDTH - 4) * collectionProgress / 100, 6, SSD1306_WHITE);
        display.setCursor(0, 48); display.println("Zero Latency Mode");
        break;

      case STATE_INFERENCING:
        display.setCursor(0, 0); display.println("  >> INFERENCING <<");
        display.setCursor(0, 32); display.println("Applying V4 RF Model");
        display.setCursor(0, 48); display.println("Balanced Dataset...");
        break;

      case STATE_ERROR:
        display.setCursor(0, 0); display.println("  >> ERROR <<");
        display.setCursor(0, 32); display.println(" Sinyal Rusak!");
        display.setCursor(0, 48); display.println(" Coba Lagi...");
        break;

      case STATE_RESULT:
        display.fillRect(0, 0, SCREEN_WIDTH, 11, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
        display.setCursor(14, 2); display.print("BP Pipeline V4");

        display.setTextColor(SSD1306_WHITE);
        int16_t bx, by; uint16_t bw, bh;
        display.getTextBounds(g_category, 0, 0, &bx, &by, &bw, &bh);
        display.setCursor((SCREEN_WIDTH - bw) / 2, 15); display.print(g_category);

        display.drawFastHLine(0, 26, SCREEN_WIDTH, SSD1306_WHITE);
        display.setCursor(0, 30); display.printf("BPM : %.1f | CTR: %.2f", g_heartRate, g_ctr);
        display.setCursor(0, 42); display.printf("IR Ratio : %.3f %%", g_irRatio);
        display.setCursor(0, 54); display.printf("Infer: %lu us", g_inferTime);
        break;
    }
    display.display();
    
    vTaskDelay(pdMS_TO_TICKS(50)); // Refresh rate OLED lebih cepat di V4 (20 FPS)
  }
}

// ── SETUP ────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(STATUS_LED, OUTPUT);

  // 1. Inisialisasi I2C0 (Wire) HANYA untuk OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  Wire.setClock(400000); 

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("ERROR: SSD1306 OLED not found on I2C0!");
  }

  display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.setCursor(4, 0); display.println("BP Pipeline V4");
  display.setCursor(0, 28); display.println("Dual-I2C + 10s ML");
  display.display();

  Serial.println("====================================");
  Serial.println("ESP32 Blood Pressure Classifier V4");
  Serial.println("ULTIMATE ARCHITECTURE: DUAL CORE & DUAL I2C");
  Serial.println("====================================");

  // 2. Inisialisasi I2C1 (Wire1) HANYA untuk SENSOR MAX30102
  Wire1.begin(SENSOR_SDA, SENSOR_SCL);
  Wire1.setClock(400000);

  if (!particleSensor.begin(Wire1, I2C_SPEED_FAST)) {
    Serial.println("   ERROR: MAX30102 not found on I2C1!");
    while (1) { digitalWrite(STATUS_LED, HIGH); delay(100); digitalWrite(STATUS_LED, LOW); delay(100); }
  }

  particleSensor.setup(0x1F, 1, 2, 100, 411, 4096);
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);
  Serial.println("   OK - Sensor initialized on I2C1");

  // 3. Jalankan Task OLED di Core 0
  xTaskCreatePinnedToCore(
    oledTask,     /* Fungsi task */
    "OLED_Task",  /* Nama task */
    4096,         /* Ukuran stack (byte) */
    NULL,         /* Parameter */
    1,            /* Prioritas (1) */
    NULL,         /* Handle */
    0             /* Di-pin ke Core 0 */
  );

  delay(1500); // Biarkan logo terbaca bentar
}

// ── TUGAS CORE 1: MEMBACA SENSOR (EKSKLUSIF DI I2C1) ──────────────────
void loop() {
  currentState = STATE_WAITING_FINGER;
  Serial.println("Waiting for finger...");

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
      Serial.println("Finger detected!");
      break;  
    }
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    vTaskDelay(pdMS_TO_TICKS(50));
  }

  currentState = STATE_SETTLING;
  Serial.println("Settling time: Discarding first 100 samples (1 second)...");
  
  for (int i = 0; i < 100; i++) {
    particleSensor.check();
    if (particleSensor.available()) {
      particleSensor.getIR(); particleSensor.getRed(); particleSensor.nextSample();
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  currentState = STATE_COLLECTING;
  collectionProgress = 0;
  Serial.println("Collecting 1000 samples (10 seconds) ZERO LATENCY...");
  digitalWrite(STATUS_LED, LOW);

  // PENGAMBILAN DATA KETAT (100 Hz / 10ms Timer) - NO I2C CONFLICT
  unsigned long nextSampleTime = micros();
  
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    particleSensor.check();
    if (particleSensor.available()) {
      irBuffer[i] = particleSensor.getIR();
      redBuffer[i] = particleSensor.getRed();
      particleSensor.nextSample();
    } else {
      irBuffer[i] = (i>0) ? irBuffer[i-1] : 0; 
      redBuffer[i] = (i>0) ? redBuffer[i-1] : 0;
    }

    if (i % 50 == 0) collectionProgress = (i * 100) / SAMPLE_WINDOW;

    while (micros() - nextSampleTime < 10000) { yield(); }
    nextSampleTime += 10000;
  }

  digitalWrite(STATUS_LED, HIGH);
  currentState = STATE_INFERENCING;
  Serial.println("Data collected! Processing DSP Filters...");

  // (DSP dan Machine Learning sepenuhnya dilakukan di Core 1)
  float irSum = 0, redSum = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) { irSum += irBuffer[i]; redSum += redBuffer[i]; }
  float irMean = irSum / SAMPLE_WINDOW; float redMean = redSum / SAMPLE_WINDOW;

  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irFilteredGlobal[i] = (float)irBuffer[i]; redFilteredGlobal[i] = (float)redBuffer[i];
  }

  detrend_signal(irFilteredGlobal, SAMPLE_WINDOW); detrend_signal(redFilteredGlobal, SAMPLE_WINDOW);
  filtfilt_bandpass(irFilteredGlobal, SAMPLE_WINDOW); filtfilt_bandpass(redFilteredGlobal, SAMPLE_WINDOW);

  float irFiltMean = 0, redFiltMean = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) { irFiltMean += irFilteredGlobal[i]; redFiltMean += redFilteredGlobal[i]; }
  irFiltMean /= SAMPLE_WINDOW; redFiltMean /= SAMPLE_WINDOW;

  float irVar = 0, redVar = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irVar += (irFilteredGlobal[i] - irFiltMean) * (irFilteredGlobal[i] - irFiltMean);
    redVar += (redFilteredGlobal[i] - redFiltMean) * (redFilteredGlobal[i] - redFiltMean);
  }
  float irStd = sqrt(irVar / SAMPLE_WINDOW); float redStd = sqrt(redVar / SAMPLE_WINDOW);

  volatile float g_time_cv = 0;
  volatile float g_amp_cv = 0;
  g_ctr = get_ctr_and_hr(irFilteredGlobal, SAMPLE_WINDOW, irStd, g_heartRate, g_time_cv, g_amp_cv);
  g_irRatio = (irMean > 0) ? ((irStd / irMean) * 100.0f) : 0;
  float redRatio = (redMean > 0) ? ((redStd / redMean) * 100.0f) : 0;

  // ── SQA / Signal Validation ──────────────────────────────────────────
  if (g_heartRate < 40.0f || g_heartRate > 180.0f || g_ctr <= 0.0f || g_time_cv > 10.0f || g_amp_cv > 0.37f) {
    currentState = STATE_ERROR;
    Serial.printf("❌ SQA REJECTED! HR: %.1f, TimeCV: %.1f%%, AmpCV: %.3f\n", g_heartRate, g_time_cv, g_amp_cv);
    vTaskDelay(pdMS_TO_TICKS(3500));
    return; 
  }
  Serial.printf("✅ SQA PASSED! HR: %.1f, TimeCV: %.1f%%, AmpCV: %.3f\n", g_heartRate, g_time_cv, g_amp_cv);

  // Random Forest tidak memerlukan Z-Score Normalization!
  // Umpankan data mentah langsung untuk memangkas latensi mikrodetik CPU
  float features[4] = {g_irRatio, redRatio, g_ctr, g_heartRate};

  unsigned long inferStart = micros();
  int prediction = rf.predict(features);
  g_inferTime = micros() - inferStart;
  
  strcpy(g_category, (prediction == 0) ? "NON-HIPERTENSI" : "HIPERTENSI");

  currentState = STATE_RESULT;
  Serial.printf("Klasifikasi V4  : %s\n", g_category);
  
  vTaskDelay(pdMS_TO_TICKS(7000)); // Biarkan layar Result tampil 7 detik
}
