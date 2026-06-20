/*
 * ESP32 Blood Pressure Classification (Random Forest)
 *
 * Real-time blood pressure classification using Random Forest TinyML
 * Classification: Non-Hipertensi (0) vs Hipertensi (1)
 * Features: ir_mean, red_mean, ir_std, red_std
 *
 * Hardware: ESP32 + MAX30102 PPG Sensor + 0.96" 128x64 OLED (SSD1306)
 *
 * Requirements:
 * - Install "SparkFun MAX3010x Pulse and Proximity Sensor" library
 * - Install "Adafruit SSD1306" library from Library Manager
 * - Install "Adafruit GFX Library" from Library Manager
 *
 * I2C Wiring (shared by MAX30102 and OLED):
 *   SDA -> GPIO 21
 *   SCL -> GPIO 22
 *   VCC -> 3.3V
 *   GND -> GND
 *
 * Usage:
 * 1. Place finger on sensor
 * 2. Wait ~25 seconds for data collection
 * 3. Classification result will be displayed on OLED and Serial
 */

#include "MAX30105.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <math.h>

// Include the Random Forest model exported by micromlgen
#include "rf_model.h"

// I2C Pin Assignment
#define SDA_PIN 21
#define SCL_PIN 22

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1      // Reset pin not used
#define OLED_I2C_ADDR 0x3C // Common address; try 0x3D if not found

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MAX30105 particleSensor;
Eloquent::ML::Port::RandomForest rf;

// Sensor configuration
// 1000 samples @ 100 Hz = 10 seconds (match Python pipeline window)
const int SAMPLE_WINDOW = 1000;
const int SAMPLE_RATE = 100;
const int LED_BRIGHTNESS = 60;

// Status LED
const int STATUS_LED = 2;

// Data buffers
long irBuffer[SAMPLE_WINDOW];
long redBuffer[SAMPLE_WINDOW];

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
  // Coefficients derived from scipy.signal.butter(2, [0.5/50, 5.0/50], btype='band')
  float b[5] = {0.01658193f, 0.0f, -0.03316386f, 0.0f, 0.01658193f};
  float a[5] = {1.0f, -3.5862398f, 4.8462898f, -2.9304272f, 0.6704579f};

  float* temp_y = new float[length];
  
  // Forward filter
  lfilter(b, a, buffer, temp_y, length);

  // Reverse temp_y into buffer
  for (int i = 0; i < length; i++) {
    buffer[i] = temp_y[length - 1 - i];
  }

  // Forward filter again (backward relative to original)
  lfilter(b, a, buffer, temp_y, length);

  // Reverse temp_y back into buffer
  for (int i = 0; i < length; i++) {
    buffer[i] = temp_y[length - 1 - i];
  }

  delete[] temp_y;
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(STATUS_LED, OUTPUT);

  // Record initial free heap
  freeHeapAtStart = ESP.getFreeHeap();

  // Initialize I2C with custom pins (shared by MAX30102 and OLED)
  Wire.begin(SDA_PIN, SCL_PIN);

  // ── Initialize OLED ─────────────────────────────────────────────────
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("ERROR: SSD1306 OLED not found!");
  } else {
    // Splash screen
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 0);
    display.println("BP Classifier v2");
    display.setCursor(4, 12);
    display.println("Random Forest");
    display.setCursor(0, 28);
    display.println("Sensor : MAX30102");
    display.setCursor(0, 40);
    display.println("Model  : 30-tree RF");
    display.setCursor(0, 54);
    display.println("Initializing...");
    display.display();
  }

  Serial.println("====================================");
  Serial.println("ESP32 Blood Pressure Classifier");
  Serial.println("Random Forest (2-Class)");
  Serial.println("====================================");
  Serial.printf("Free Heap: %u bytes\n", freeHeapAtStart);

  // Initialize sensor
  Serial.println("\n1. Initializing MAX30102 sensor...");
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("   ERROR: MAX30102 not found!");
    while (1) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  }

  // SINKRONISASI KONFIGURASI DENGAN esp32_data_collector.ino
  // Ini SANGAT KRUSIAL agar data saat testing sama persis dengan data saat training
  byte ledBrightness = 0x1F;  // Konsisten dengan setPulseAmplitude
  byte sampleAverage = 1;     // Sesuai data collector (TIDAK BOLEH 4!)
  byte ledMode = 2;           // Red + IR
  int sampleRate = 100;       
  int pulseWidth = 411;       
  int adcRange = 4096;        

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);
  Serial.println("   OK - Sensor initialized");

  Serial.println("\n2. Random Forest model loaded (compiled in rf_model.h)");
  Serial.println("   Features : ir_mean, red_mean, ir_std, red_std");
  Serial.println("   Classes  : Non-Hipertensi (0), Hipertensi (1)");
  Serial.println("   Trees    : 30");

  uint32_t heapAfterInit = ESP.getFreeHeap();
  Serial.printf("   RAM used : %u bytes\n", freeHeapAtStart - heapAfterInit);
  Serial.printf("   RAM free : %u bytes\n", heapAfterInit);

  Serial.println("\nSystem ready!");
  Serial.println("Place your finger on the sensor...\n");

  delay(1500);
  digitalWrite(STATUS_LED, HIGH);

  // Ready prompt on OLED
  oledMessage("  >> READY <<", "", "Place finger on", "the sensor...");
  delay(1000);
}

void loop() {
  // ── Menunggu Jari Terdeteksi ─────────────────────────────────────────
  oledMessage("  >> READY <<", "", "Place finger on", "the sensor...");
  Serial.println("Waiting for finger...");

  // Polling: Cek terus sampai jari terdeteksi
  while (true) {
    while (!particleSensor.available()) {
      particleSensor.check();
    }
    long irCheck = particleSensor.getIR();
    particleSensor.getRed();  // Baca red juga agar buffer sensor tidak penuh
    particleSensor.nextSample();

    if (irCheck > 50000) {
      Serial.println("Finger detected!");
      break;  // Jari terdeteksi, lanjut ke tahap berikutnya
    }
    // LED berkedip pelan saat menunggu
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    delay(50);
  }

  // ── Settling Time: Buang 100 sampel pertama (1 detik) ────────────────
  // Sensor MAX30102 membutuhkan waktu stabilisasi saat jari pertama kali
  // menyentuh permukaan sensor. Data 1 detik pertama mengandung noise
  // transien ekstrem akibat penyesuaian komponen optik internal.
  oledMessage(" Stabilizing...", "", "Keep finger still", "Wait 1 sec...");
  Serial.println("Settling time: Discarding first 100 samples (1 second)...");
  
  for (int i = 0; i < 100; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
    }
    // Baca tapi BUANG (tidak disimpan ke buffer)
    particleSensor.getIR();
    particleSensor.getRed();
    particleSensor.nextSample();
  }
  Serial.println("Sensor stabilized. Starting data collection...");

  // ── Collect sensor data (10 seconds @ 100Hz) ────────────────────────
  oledMessage(" Collecting...", "", "Keep finger still", "~10 seconds");
  Serial.println("Collecting 1000 samples (10 seconds)...");
  digitalWrite(STATUS_LED, LOW);

  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
    }

    irBuffer[i] = particleSensor.getIR();
    redBuffer[i] = particleSensor.getRed();
    particleSensor.nextSample();

    // Update progress on OLED every 200 samples (2 seconds)
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

      // Progress bar
      display.drawRect(0, 30, SCREEN_WIDTH, 10, SSD1306_WHITE);
      display.fillRect(2, 32, (SCREEN_WIDTH - 4) * progress / 100, 6, SSD1306_WHITE);

      display.setCursor(0, 48);
      display.println("Keep finger still!");
      display.display();
    }
  }
  digitalWrite(STATUS_LED, HIGH);

  Serial.println("Data collected! Processing...");

  // Calculate 4 features (matching Python pipeline)
  // 1. RAW Mean (Sumbu X)
  float irSum = 0, redSum = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irSum += irBuffer[i];
    redSum += redBuffer[i];
  }
  float irMean = irSum / SAMPLE_WINDOW;
  float redMean = redSum / SAMPLE_WINDOW;

  // 2. Siapkan buffer float untuk difilter
  float* irFiltered = new float[SAMPLE_WINDOW];
  float* redFiltered = new float[SAMPLE_WINDOW];
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irFiltered[i] = (float)irBuffer[i];
    redFiltered[i] = (float)redBuffer[i];
  }

  // 3. Lakukan Detrending
  detrend_signal(irFiltered, SAMPLE_WINDOW);
  detrend_signal(redFiltered, SAMPLE_WINDOW);

  // 4. Lakukan Bandpass Filter (FiltFilt 0.5 - 5Hz)
  filtfilt_bandpass(irFiltered, SAMPLE_WINDOW);
  filtfilt_bandpass(redFiltered, SAMPLE_WINDOW);

  // 5. Hitung Standar Deviasi (std) dari data BERSIH
  float irFiltMean = 0, redFiltMean = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irFiltMean += irFiltered[i];
    redFiltMean += redFiltered[i];
  }
  irFiltMean /= SAMPLE_WINDOW;
  redFiltMean /= SAMPLE_WINDOW;

  float irVar = 0, redVar = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irVar += (irFiltered[i] - irFiltMean) * (irFiltered[i] - irFiltMean);
    redVar += (redFiltered[i] - redFiltMean) * (redFiltered[i] - redFiltMean);
  }
  float irStd = sqrt(irVar / SAMPLE_WINDOW);
  float redStd = sqrt(redVar / SAMPLE_WINDOW);

  // Bersihkan RAM setelah selesai
  delete[] irFiltered;
  delete[] redFiltered;

  // Feature array: [ir_mean, red_mean, ir_std, red_std]
  float features[4] = {irMean, redMean, irStd, redStd};

  Serial.println("Features:");
  Serial.printf("  ir_mean  = %.1f\n", irMean);
  Serial.printf("  red_mean = %.1f\n", redMean);
  Serial.printf("  ir_std   = %.1f\n", irStd);
  Serial.printf("  red_std  = %.1f\n", redStd);

  // ── SQA (Signal Quality) / Motion Artifact Rejection ─────────────────
  // Memblokir prediksi jika ir_std di luar batas normal (goyang atau mati)
  if (irStd < 100.0 || irStd > 500.0) {
    Serial.println("\n[SQA REJECTED] Sinyal tidak valid!");
    Serial.println("Penyebab: Jari bergoyang (Motion) atau menekan terlalu kuat (Blanching).");
    
    oledMessage("  >> ERROR <<", "", " Sinyal Rusak!", " Jari Goyang?");
    delay(3500);
    return; // Batalkan prediksi, ulangi pengukuran dari awal
  }

  // ── Run Random Forest Inference ──────────────────────────────────────
  uint32_t heapBeforeInfer = ESP.getFreeHeap();
  unsigned long inferStart = micros();

  int prediction = rf.predict(features);

  unsigned long inferTime = micros() - inferStart;
  uint32_t heapAfterInfer = ESP.getFreeHeap();
  uint32_t ramUsedByInfer = (heapBeforeInfer > heapAfterInfer)
                                ? (heapBeforeInfer - heapAfterInfer)
                                : 0;
  uint32_t totalRamUsed = freeHeapAtStart - heapAfterInfer;

  // Classification result
  const char *category;
  if (prediction == 0) {
    category = "NON-HIPERTENSI";
  } else {
    category = "HIPERTENSI";
  }

  // Serial output
  Serial.println("========================================");
  Serial.printf("Klasifikasi  : %s\n", category);
  Serial.printf("Waktu Inferensi : %lu us (%.2f ms)\n", inferTime,
                inferTime / 1000.0);
  Serial.printf("RAM Total    : %u bytes used\n", totalRamUsed);
  Serial.printf("RAM Free     : %u bytes\n", heapAfterInfer);
  Serial.println("========================================\n");

  // ── OLED Display Result ──────────────────────────────────────────────
  // Layout: 128x64 pixels, font size 1 = 6x8px per char (max 21 char/line)
  display.clearDisplay();

  // Title bar (inverted) — Y=0 to Y=10 (11px)
  display.fillRect(0, 0, SCREEN_WIDTH, 11, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(7, 2);
  display.print("BP Classification");

  // Classification result — Y=15 (8px) → ends Y=23
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  int16_t bx, by;
  uint16_t bw, bh;
  display.getTextBounds(category, 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH - bw) / 2, 15);
  display.print(category);

  // Divider — Y=26
  display.drawFastHLine(0, 26, SCREEN_WIDTH, SSD1306_WHITE);

  // Inference time — Y=30 (8px) → ends Y=38
  display.setCursor(0, 30);
  display.printf("Infer: %lu us", inferTime);

  // RAM usage — Y=42 (8px) → ends Y=50
  display.setCursor(0, 42);
  display.printf("RAM used: %u B", totalRamUsed);

  // Free RAM — Y=54 (8px) → ends Y=62 ✅ (fits in 64px)
  display.setCursor(0, 54);
  display.printf("RAM free: %u B", heapAfterInfer);

  display.display();

  delay(5000); // Show result for 5 seconds
}
