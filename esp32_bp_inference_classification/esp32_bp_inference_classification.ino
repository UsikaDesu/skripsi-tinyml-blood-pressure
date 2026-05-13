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
    oledMessage("ERROR:", "MAX30102", "not found!", "Check wiring.");
    while (1) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  }

  particleSensor.setup(LED_BRIGHTNESS, 4, 2, SAMPLE_RATE, 411, 4096);
  particleSensor.setPulseAmplitudeRed(0x0A);
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
  // Show measuring prompt with progress
  oledMessage(" Collecting...", "", "Keep finger still", "~10 seconds");

  // Collect sensor data (10 seconds @ 100Hz)
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

  // Check if finger is present (basic check)
  float irMeanCheck = 0;
  for (int i = 0; i < 100; i++) {
    irMeanCheck += irBuffer[i];
  }
  irMeanCheck /= 100.0;

  if (irMeanCheck < 50000) {
    Serial.println("No finger detected!");
    oledMessage(" No Signal!", "", "Check finger", "placement.");
    delay(2000);
    return;
  }

  // Calculate 4 features (matching Python pipeline)
  float irSum = 0, redSum = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irSum += irBuffer[i];
    redSum += redBuffer[i];
  }
  float irMean = irSum / SAMPLE_WINDOW;
  float redMean = redSum / SAMPLE_WINDOW;

  float irVar = 0, redVar = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irVar += (irBuffer[i] - irMean) * (irBuffer[i] - irMean);
    redVar += (redBuffer[i] - redMean) * (redBuffer[i] - redMean);
  }
  float irStd = sqrt(irVar / SAMPLE_WINDOW);
  float redStd = sqrt(redVar / SAMPLE_WINDOW);

  // Feature array: [ir_mean, red_mean, ir_std, red_std]
  float features[4] = {irMean, redMean, irStd, redStd};

  Serial.println("Features:");
  Serial.printf("  ir_mean  = %.1f\n", irMean);
  Serial.printf("  red_mean = %.1f\n", redMean);
  Serial.printf("  ir_std   = %.1f\n", irStd);
  Serial.printf("  red_std  = %.1f\n", redStd);

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
