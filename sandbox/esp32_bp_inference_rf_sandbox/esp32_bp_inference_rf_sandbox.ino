/*
 * ESP32 Blood Pressure Classification sandbox (Physiological Sandbox)
 *
 * Real-time blood pressure classification using Random Forest TinyML
 * Classification: Non-Hipertensi (0) vs Hipertensi (1)
 * Features: ir_ratio (AC/DC %), red_ratio (AC/DC %), rise_time (ms), heart_rate (BPM)
 *
 * Hardware: ESP32 + MAX30102 PPG Sensor + 0.96" 128x64 OLED (SSD1306)
 *
 * I2C Wiring (shared by MAX30102 and OLED):
 *   SDA -> GPIO 21
 *   SCL -> GPIO 22
 *   VCC -> 3.3V
 *   GND -> GND
 */

#include "MAX30105.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <math.h>

// Include the sandbox Random Forest model exported in the sandbox
#include "rf_model_sandbox.h"

// I2C Pin Assignment
#define SDA_PIN 21
#define SCL_PIN 22

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1      
#define OLED_I2C_ADDR 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MAX30105 particleSensor;
Eloquent::ML::Port::RandomForest rf;

// Sensor configuration
const int SAMPLE_WINDOW = 1000;
const int SAMPLE_RATE = 100;

// Status LED
const int STATUS_LED = 2;

// Data buffers (Global Static to avoid Heap Fragmentation)
long irBuffer[SAMPLE_WINDOW];
long redBuffer[SAMPLE_WINDOW];

// DSP Buffers (Global Static)
float irFilteredGlobal[SAMPLE_WINDOW];
float redFilteredGlobal[SAMPLE_WINDOW];
float tempYBufferGlobal[SAMPLE_WINDOW];

// RAM tracking
uint32_t freeHeapAtStart = 0;

// Helper: show a full-screen OLED message
void oledMessage(const char *line1, const char *line2 = "",
                 const char *line3 = "", const char *line4 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(line1);
  display.setCursor(0, 16);
  display.println(line2);
  display.setCursor(0, 32);
  display.println(line3);
  display.setCursor(0, 48);
  display.println(line4);
  display.display();
}

// ── DSP Filters ───────────────────────────────────────────────────────

// 1. Detrending (Linear Least Squares)
void detrend_signal(float* buffer, int length) {
  double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
  for (int i = 0; i < length; i++) {
    double x = i;
    double y = buffer[i];
    sum_x += x;
    sum_y += y;
    sum_xy += x * y;
    sum_xx += x * x;
  }
  double n = length;
  double m = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
  double c = (sum_y - m * sum_x) / n;

  for (int i = 0; i < length; i++) {
    double trend = m * i + c;
    buffer[i] = (float)(buffer[i] - trend);
  }
}

// 2. IIR Filter logic (4th order)
void lfilter(float* b, float* a, float* x, float* y, int length) {
  for (int n = 0; n < length; n++) {
    float xn = x[n];
    float xn1 = (n >= 1) ? x[n-1] : 0;
    float xn2 = (n >= 2) ? x[n-2] : 0;
    float xn3 = (n >= 3) ? x[n-3] : 0;
    float xn4 = (n >= 4) ? x[n-4] : 0;

    float yn1 = (n >= 1) ? y[n-1] : 0;
    float yn2 = (n >= 2) ? y[n-2] : 0;
    float yn3 = (n >= 3) ? y[n-3] : 0;
    float yn4 = (n >= 4) ? y[n-4] : 0;

    y[n] = b[0]*xn + b[1]*xn1 + b[2]*xn2 + b[3]*xn3 + b[4]*xn4
         - a[1]*yn1 - a[2]*yn2 - a[3]*yn3 - a[4]*yn4;
  }
}

// 3. Zero-phase Bandpass Filter (0.5 - 5 Hz)
void filtfilt_bandpass(float* buffer, int length) {
  float b[5] = {0.01658193f, 0.0f, -0.03316386f, 0.0f, 0.01658193f};
  float a[5] = {1.0f, -3.5862398f, 4.8462898f, -2.9304272f, 0.6704579f};

  lfilter(b, a, buffer, tempYBufferGlobal, length);

  for (int i = 0; i < length; i++) {
    buffer[i] = tempYBufferGlobal[length - 1 - i];
  }

  lfilter(b, a, buffer, tempYBufferGlobal, length);

  for (int i = 0; i < length; i++) {
    buffer[i] = tempYBufferGlobal[length - 1 - i];
  }
}

// Helper function for Moving Average to smooth signal before peak detection
float get_smoothed_val(float* buffer, int i, int length) {
  if (i < 2 || i >= length - 2) return buffer[i];
  return (buffer[i-2] + buffer[i-1] + buffer[i] + buffer[i+1] + buffer[i+2]) / 5.0f;
}

// 4. Upgraded Peak & Trough Detection for Rise Time and HR (100% stack-safe)
float get_rise_time_and_hr(float* buffer, int length, float std_val, float& out_hr) {
  int peak_indices[100];
  int peak_count = 0;
  int min_dist = 50; // At 100Hz, min 500ms distance between beats (Refractory Period)
  int last_peak_idx = -min_dist;
  float peak_threshold = std_val * 0.5f; // Dynamic threshold based on signal amplitude
  
  // Find Peaks (Systolic Peaks) menggunakan sinyal yang dihaluskan (Moving Average)
  for (int i = 2; i < length - 2; i++) {
    float val_prev = get_smoothed_val(buffer, i-1, length);
    float val_curr = get_smoothed_val(buffer, i, length);
    float val_next = get_smoothed_val(buffer, i+1, length);
    
    // Deteksi puncak pada sinyal yang dihaluskan agar terhindar dari puncak palsu (gerigi)
    if (val_curr > val_prev && val_curr > val_next) { // Local max
      if (buffer[i] > peak_threshold) { // Strength filter
        if (i - last_peak_idx >= min_dist) { // Refractory Period lock
          peak_indices[peak_count++] = i;
          last_peak_idx = i;
          if (peak_count >= 100) break;
        }
      }
    }
  }

  if (peak_count < 2) {
    out_hr = 0;
    return 0; // Not enough cardiac cycles detected
  }

  // Find Troughs (Onsets) and calculate Systolic Rise Time
  float total_rise_time = 0;
  int valid_beats = 0;
  
  for (int c = 0; c < peak_count; c++) {
    int p_idx = peak_indices[c];
    
    // Search backward up to 45 samples (450 ms) to locate the trough
    int start_search = (p_idx - 45 > 0) ? (p_idx - 45) : 0;
    int trough_idx = p_idx;
    float min_val = buffer[p_idx];
    
    for (int j = p_idx; j >= start_search; j--) {
      if (buffer[j] < min_val) {
        min_val = buffer[j];
        trough_idx = j;
      }
    }
    
    if (p_idx > trough_idx) {
      float rise_time_ms = (p_idx - trough_idx) * 10.0f; // 1 sample = 10ms
      total_rise_time += rise_time_ms;
      valid_beats++;
    }
  }

  float avg_rise_time = (valid_beats > 0) ? (total_rise_time / valid_beats) : 0;

  // Calculate Average Heart Rate (BPM)
  float total_rr = 0;
  for (int c = 1; c < peak_count; c++) {
    total_rr += (peak_indices[c] - peak_indices[c-1]);
  }
  float avg_rr_samples = total_rr / (peak_count - 1);
  float avg_rr_seconds = avg_rr_samples / 100.0f; // 100 Hz
  out_hr = 60.0f / avg_rr_seconds;

  return avg_rise_time;
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(STATUS_LED, OUTPUT);
  freeHeapAtStart = ESP.getFreeHeap();

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("ERROR: SSD1306 OLED not found!");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 0);
    display.println("BP Sandbox sandbox");
    display.setCursor(4, 12);
    display.println("Physiological RF");
    display.setCursor(0, 28);
    display.println("Sensor : MAX30102");
    display.setCursor(0, 40);
    display.println("Model  : 50-tree RF");
    display.setCursor(0, 54);
    display.println("Initializing...");
    display.display();
  }

  Serial.println("====================================");
  Serial.println("ESP32 Blood Pressure Classifier sandbox");
  Serial.println("Physiological Model (Subject-Wise)");
  Serial.println("====================================");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("   ERROR: MAX30102 not found!");
    while (1) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  }

  byte ledBrightness = 0x1F;  
  byte sampleAverage = 1;     
  byte ledMode = 2;           
  int sampleRate = 100;       
  int pulseWidth = 411;       
  int adcRange = 4096;        

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);
  Serial.println("   OK - Sensor initialized");

  Serial.println("\n2. RF Model sandbox Loaded (Physiological Features)");
  Serial.println("   Features : ir_ratio, red_ratio, rise_time_ms, heart_rate");
  Serial.println("   Classes  : Non-Hipertensi (0), Hipertensi (1)");

  delay(1500);
  digitalWrite(STATUS_LED, HIGH);
  oledMessage("  >> READY <<", "", "Place finger on", "the sensor...");
  delay(1000);
}

void loop() {
  oledMessage("  >> READY <<", "", "Place finger on", "the sensor...");
  Serial.println("Waiting for finger...");

  while (true) {
    while (!particleSensor.available()) {
      particleSensor.check();
      yield(); // Anti-Blocking untuk RTOS
    }
    long irCheck = particleSensor.getIR();
    particleSensor.getRed();  
    particleSensor.nextSample();

    if (irCheck > 50000) {
      Serial.println("Finger detected!");
      break;  
    }
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    delay(50);
  }

  oledMessage(" Stabilizing...", "", "Keep finger still", "Wait 1 sec...");
  Serial.println("Settling time: Discarding first 100 samples (1 second)...");
  
  for (int i = 0; i < 100; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
      yield(); // Anti-Blocking untuk RTOS
    }
    particleSensor.getIR();
    particleSensor.getRed();
    particleSensor.nextSample();
  }
  Serial.println("Sensor stabilized. Starting data collection...");

  oledMessage(" Collecting...", "", "Keep finger still", "~10 seconds");
  Serial.println("Collecting 1000 samples (10 seconds)...");
  digitalWrite(STATUS_LED, LOW);

  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
      yield(); // Anti-Blocking untuk RTOS
    }

    irBuffer[i] = particleSensor.getIR();
    redBuffer[i] = particleSensor.getRed();
    particleSensor.nextSample();

    if (i % 200 == 0) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      int progress = (i * 100) / SAMPLE_WINDOW;
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(4, 0);
      display.println(" Collecting...");
      display.setCursor(0, 16);
      display.printf("Progress: %d%%", progress);
      display.drawRect(0, 30, SCREEN_WIDTH, 10, SSD1306_WHITE);
      display.fillRect(2, 32, (SCREEN_WIDTH - 4) * progress / 100, 6, SSD1306_WHITE);
      display.setCursor(0, 48);
      display.println("Keep finger still!");
      display.display();
    }
  }
  digitalWrite(STATUS_LED, HIGH);
  Serial.println("Data collected! Processing DSP Filters...");

  // 1. Calculate RAW Means (DC baseline components)
  float irSum = 0, redSum = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irSum += irBuffer[i];
    redSum += redBuffer[i];
  }
  float irMean = irSum / SAMPLE_WINDOW;
  float redMean = redSum / SAMPLE_WINDOW;

  // 2. Prepare float buffers for filter processing (Using pre-allocated Global Arrays)
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irFilteredGlobal[i] = (float)irBuffer[i];
    redFilteredGlobal[i] = (float)redBuffer[i];
  }

  // 3. Linear Detrending
  detrend_signal(irFilteredGlobal, SAMPLE_WINDOW);
  detrend_signal(redFilteredGlobal, SAMPLE_WINDOW);

  // 4. Zero-phase Bandpass Filter (0.5 - 5Hz)
  filtfilt_bandpass(irFilteredGlobal, SAMPLE_WINDOW);
  filtfilt_bandpass(redFilteredGlobal, SAMPLE_WINDOW);

  // 5. Calculate AC Standard Deviations (from clean signal)
  float irFiltMean = 0, redFiltMean = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irFiltMean += irFilteredGlobal[i];
    redFiltMean += redFilteredGlobal[i];
  }
  irFiltMean /= SAMPLE_WINDOW;
  redFiltMean /= SAMPLE_WINDOW;

  float irVar = 0, redVar = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irVar += (irFilteredGlobal[i] - irFiltMean) * (irFilteredGlobal[i] - irFiltMean);
    redVar += (redFilteredGlobal[i] - redFiltMean) * (redFilteredGlobal[i] - redFiltMean);
  }
  float irStd = sqrt(irVar / SAMPLE_WINDOW);
  float redStd = sqrt(redVar / SAMPLE_WINDOW);

  // 6. Upgraded sandbox Feature Extraction: Systolic Rise Time & Heart Rate
  float heartRate = 0;
  float riseTimeMs = get_rise_time_and_hr(irFilteredGlobal, SAMPLE_WINDOW, irStd, heartRate);

  // 7. Calculate sandbox Upgraded Features: AC/DC Ratio % (Perfusion Index)
  float irRatio = (irMean > 0) ? ((irStd / irMean) * 100.0f) : 0;
  float redRatio = (redMean > 0) ? ((redStd / redMean) * 100.0f) : 0;

  Serial.println("Upgraded Features sandbox:");
  Serial.printf("  ir_ratio     = %.4f %%\n", irRatio);
  Serial.printf("  red_ratio    = %.4f %%\n", redRatio);
  Serial.printf("  rise_time_ms = %.2f ms\n", riseTimeMs);
  Serial.printf("  heart_rate   = %.2f BPM\n", heartRate);

  // ── SQA / Signal Validation ──────────────────────────────────────────
  // Check if heart rate and rise time calculations are physiologically sound
  if (heartRate < 40.0f || heartRate > 180.0f || riseTimeMs <= 0.0f) {
    Serial.println("\n[SQA REJECTED] Sinyal sandbox tidak valid!");
    Serial.println("Penyebab: Jari bergerak secara ekstrem atau penempatan sensor kurang presisi.");
    oledMessage("  >> ERROR <<", "", " Sinyal Rusak!", " Coba Lagi...");
    delay(3500);
    return; 
  }

  // Feature array order in sandbox RF: [ir_ratio, red_ratio, rise_time_ms, heart_rate]
  float features[4] = {irRatio, redRatio, riseTimeMs, heartRate};

  // ── Inference ────────────────────────────────────────────────────────
  unsigned long inferStart = micros();
  int prediction = rf.predict(features);
  unsigned long inferTime = micros() - inferStart;
  
  uint32_t heapAfterInfer = ESP.getFreeHeap();
  uint32_t totalRamUsed = freeHeapAtStart - heapAfterInfer;

  const char *category = (prediction == 0) ? "NON-HIPERTENSI" : "HIPERTENSI";

  Serial.println("========================================");
  Serial.printf("Klasifikasi sandbox  : %s\n", category);
  Serial.printf("Waktu Inferensi : %lu us (%.2f ms)\n", inferTime, inferTime / 1000.0f);
  Serial.printf("RAM Total       : %u B used\n", totalRamUsed);
  Serial.println("========================================\n");

  // OLED Display Result
  display.clearDisplay();
  display.fillRect(0, 0, SCREEN_WIDTH, 11, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(14, 2);
  display.print("BP Sandbox sandbox");

  display.setTextColor(SSD1306_WHITE);
  int16_t bx, by;
  uint16_t bw, bh;
  display.getTextBounds(category, 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH - bw) / 2, 15);
  display.print(category);

  display.drawFastHLine(0, 26, SCREEN_WIDTH, SSD1306_WHITE);

  display.setCursor(0, 30);
  display.printf("BPM : %.1f | RT: %.0f ms", heartRate, riseTimeMs);

  display.setCursor(0, 42);
  display.printf("IR Ratio : %.3f %%", irRatio);

  display.setCursor(0, 54);
  display.printf("Infer: %lu us (%d B)", inferTime, totalRamUsed);

  display.display();
  delay(5000); 
}
