/*
 * ESP32 Data Collector for Blood Pressure Estimation
 *
 * Hardware: ESP32 + MAX30102 PPG Sensor
 * Purpose: Collect heart rate, SpO2, and PPG waveform data for ML training
 *
 * MAX30102 Wiring (I2C):
 * - MAX30102 SDA -> GPIO21
 * - MAX30102 SCL -> GPIO22
 * - MAX30102 INT -> GPIO19 (optional)
 * - VCC -> 3.3V
 * - GND -> GND
 * 
 * FIX LOG:
 * - Increased LED brightness to 0x1F for stronger signal
 * - Changed sampleAverage to 1 (no averaging) so checkForBeat gets raw pulses
 * - Added warm-up phase (10 seconds) to let checkForBeat build baseline
 * - Added debug mode to help diagnose issues
 */

#include "MAX30105.h"
#include "heartRate.h"
#include <Wire.h>

// I2C Pin Assignment
#define SDA_PIN 21
#define SCL_PIN 22

MAX30105 particleSensor;

// Data collection parameters
const int SAMPLE_WINDOW = 300;  // 300 samples = ~3 seconds at 100Hz (more time to catch beats)
const int SAMPLE_RATE = 100;    // Samples per second

// Data buffers
long irBuffer[300];
long redBuffer[300];

// Heart rate tracking (using SparkFun checkForBeat)
const byte HR_AVG_SIZE = 4;
float hrHistory[HR_AVG_SIZE];
byte hrSpot = 0;
long lastBeatTime = 0;
float currentBPM = 0;
float avgBPM = 0;
int beatCount = 0;

// Status LED
const int STATUS_LED = 2;

// Debug mode - set to true to see raw values for troubleshooting
const bool DEBUG_MODE = true;

void setup() {
  Serial.begin(115200);
  pinMode(STATUS_LED, OUTPUT);

  // Initialize I2C with custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("ESP32 Blood Pressure Data Collector");
  Serial.println("====================================");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("ERROR: MAX30102 not found!");
    Serial.println("Check wiring: SDA->21, SCL->22, VCC->3.3V, GND->GND");
    while (1) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      delay(100);
    }
  }

  Serial.println("Sensor found!");

  // ==========================================
  // CRITICAL SENSOR CONFIGURATION
  // ==========================================
  // These settings are tuned for reliable checkForBeat() detection:
  //
  // ledBrightness = 0x1F (31) - bright enough for good signal, not too bright
  // sampleAverage = 1   - NO averaging! checkForBeat needs raw pulse waveform
  // ledMode = 2         - Red + IR mode
  // sampleRate = 100    - 100 samples per second
  // pulseWidth = 411    - 18-bit resolution (best accuracy)
  // adcRange = 4096     - Full range
  //
  byte ledBrightness = 0x7F;  // Increased brightness for more reliable beat detection
  byte sampleAverage = 1;     // MUST BE 1 for checkForBeat! No averaging!
  byte ledMode = 2;           // 1=Red only, 2=Red+IR, 3=Red+IR+Green
  int sampleRate = 100;       // 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;       // 69, 118, 215, 411 (higher = more accurate)
  int adcRange = 4096;        // 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  // Also set the IR LED separately - this is what checkForBeat uses
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);

  Serial.println("Sensor configured!");
  Serial.println();

  // ==========================================
  // WARM-UP PHASE (CRITICAL!)
  // ==========================================
  // checkForBeat() needs time to build its internal baseline.
  // We must read samples continuously for ~10 seconds before
  // it can reliably detect beats.
  Serial.println(">> Warming up sensor (10 seconds)...");
  Serial.println(">> Place your finger GENTLY on the sensor NOW");
  Serial.println(">> Don't press too hard - light contact is best!");
  Serial.println();

  unsigned long warmupStart = millis();
  int warmupBeats = 0;

  while (millis() - warmupStart < 10000) {
    // Read sample
    long irValue = particleSensor.getIR();
    long redValue = particleSensor.getRed();

    // Feed to beat detector
    if (checkForBeat(irValue)) {
      long delta = millis() - lastBeatTime;
      lastBeatTime = millis();

      float bpm = 60.0 / (delta / 1000.0);
      if (bpm > 20 && bpm < 255) {
        currentBPM = bpm;
        warmupBeats++;
        hrHistory[hrSpot++ % HR_AVG_SIZE] = bpm;
      }
    }

    // Show progress
    int elapsed = (millis() - warmupStart) / 1000;
    static int lastPrint = -1;
    if (elapsed != lastPrint) {
      lastPrint = elapsed;
      Serial.printf("  Warming up... %d/10s | IR: %ld | Beats: %d",
                     elapsed, irValue, warmupBeats);
      if (irValue < 50000) {
        Serial.print(" | !! NO FINGER DETECTED - place finger on sensor!");
      }
      Serial.println();
    }
  }

  // Calculate initial average BPM
  float sum = 0;
  int count = 0;
  for (byte j = 0; j < HR_AVG_SIZE; j++) {
    if (hrHistory[j] > 0) {
      sum += hrHistory[j];
      count++;
    }
  }
  if (count > 0) avgBPM = sum / count;

  Serial.println();
  if (warmupBeats >= 5) {
    Serial.printf(">> Warmup complete! Detected %d beats. Avg HR: %.0f bpm\n", warmupBeats, avgBPM);
    Serial.println(">> Sensor is working correctly!");
  } else if (warmupBeats > 0) {
    Serial.printf(">> Warmup complete. Only %d beats detected.\n", warmupBeats);
    Serial.println(">> Try adjusting finger position for better contact.");
  } else {
    Serial.println(">> WARNING: No beats detected during warmup!");
    Serial.println(">> Possible fixes:");
    Serial.println(">>   1. Place finger more gently (don't press hard)");
    Serial.println(">>   2. Make sure finger covers the sensor completely");
    Serial.println(">>   3. Try a different finger (index finger works best)");
    Serial.println(">>   4. Warm your hands first if they're cold");
    Serial.println(">> Continuing anyway - data collection will start...");
  }

  Serial.println();
  Serial.println("====================================");
  Serial.println("Starting data collection...");
  Serial.println("====================================");
  delay(1000);

  // Print CSV header
  Serial.println("timestamp,ir_mean,red_mean,ir_std,red_std,heart_rate,spo2,valid");

  digitalWrite(STATUS_LED, HIGH);
}

void loop() {
  // Collect samples and detect beats simultaneously
  beatCount = 0;

  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    // The sensor runs at SAMPLE_RATE Hz internally.
    // We just read samples as fast as available.
    long irValue = particleSensor.getIR();
    long redValue = particleSensor.getRed();

    irBuffer[i] = irValue;
    redBuffer[i] = redValue;

    // Run beat detection on EVERY sample (this is how checkForBeat works)
    if (checkForBeat(irValue)) {
      long delta = millis() - lastBeatTime;
      lastBeatTime = millis();

      float bpm = 60.0 / (delta / 1000.0);

      if (bpm > 20 && bpm < 255) {
        currentBPM = bpm;
        hrHistory[hrSpot++ % HR_AVG_SIZE] = bpm;
        beatCount++;

        // Calculate rolling average
        float sum = 0;
        int count = 0;
        for (byte j = 0; j < HR_AVG_SIZE; j++) {
          if (hrHistory[j] > 0) {
            sum += hrHistory[j];
            count++;
          }
        }
        if (count > 0) avgBPM = sum / count;
      }
    }

    // Blink LED during collection
    if (i % 50 == 0) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    }
  }

  digitalWrite(STATUS_LED, HIGH);

  // Calculate PPG statistics
  double irSum = 0, redSum = 0;  // Use double to avoid overflow
  long irMin = irBuffer[0], irMax = irBuffer[0];
  long redMin = redBuffer[0], redMax = redBuffer[0];

  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irSum += irBuffer[i];
    redSum += redBuffer[i];

    if (irBuffer[i] < irMin) irMin = irBuffer[i];
    if (irBuffer[i] > irMax) irMax = irBuffer[i];
    if (redBuffer[i] < redMin) redMin = redBuffer[i];
    if (redBuffer[i] > redMax) redMax = redBuffer[i];
  }

  float irMean = irSum / (float)SAMPLE_WINDOW;
  float redMean = redSum / (float)SAMPLE_WINDOW;

  // Calculate standard deviation
  float irVariance = 0, redVariance = 0;
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    irVariance += pow(irBuffer[i] - irMean, 2);
    redVariance += pow(redBuffer[i] - redMean, 2);
  }
  float irStd = sqrt(irVariance / SAMPLE_WINDOW);
  float redStd = sqrt(redVariance / SAMPLE_WINDOW);

  // Use heart rate from checkForBeat
  float heartRate = avgBPM;

  // If no beat detected for 5 seconds, reset
  if (millis() - lastBeatTime > 5000) {
    heartRate = 0;
    avgBPM = 0;
    currentBPM = 0;
    // Clear history
    for (byte j = 0; j < HR_AVG_SIZE; j++) hrHistory[j] = 0;
  }

  // Calculate SpO2 (simplified ratio-of-ratios method)
  float spo2 = calculateSpO2(redBuffer, irBuffer, SAMPLE_WINDOW);

  // Data validity check - finger must be present, HR detected, and SpO2 valid
  bool fingerPresent = (irMean > 50000);
  bool valid = fingerPresent && (heartRate > 40) && (heartRate < 180) && (spo2 > 80.0);

  // Output CSV data
  Serial.print(millis());
  Serial.print(",");
  Serial.print(irMean, 2);
  Serial.print(",");
  Serial.print(redMean, 2);
  Serial.print(",");
  Serial.print(irStd, 2);
  Serial.print(",");
  Serial.print(redStd, 2);
  Serial.print(",");
  Serial.print(heartRate, 2);
  Serial.print(",");
  Serial.print(spo2, 2);
  Serial.print(",");
  Serial.println(valid ? "1" : "0");

  // Debug output (only print occasionally to avoid flooding)
  if (DEBUG_MODE) {
    static int debugCounter = 0;
    debugCounter++;
    if (debugCounter % 5 == 0) {  // Print debug every 5th reading
      Serial.print("  [DEBUG] Finger: ");
      Serial.print(fingerPresent ? "YES" : "NO");
      Serial.print(" | Beats this window: ");
      Serial.print(beatCount);
      Serial.print(" | Current BPM: ");
      Serial.print(currentBPM, 1);
      Serial.print(" | Avg BPM: ");
      Serial.print(avgBPM, 1);
      Serial.print(" | IR range: ");
      Serial.print(irMax - irMin);
      Serial.println();
    }
  }

  delay(100);  // Small delay between readings
}

// Calculate SpO2 using ratio-of-ratios method
float calculateSpO2(long *redBuf, long *irBuf, int length) {
  long redSum = 0, irSum = 0;
  long redMin = redBuf[0], redMax = redBuf[0];
  long irMin = irBuf[0], irMax = irBuf[0];

  for (int i = 0; i < length; i++) {
    redSum += redBuf[i];
    irSum += irBuf[i];

    if (redBuf[i] < redMin) redMin = redBuf[i];
    if (redBuf[i] > redMax) redMax = redBuf[i];
    if (irBuf[i] < irMin) irMin = irBuf[i];
    if (irBuf[i] > irMax) irMax = irBuf[i];
  }

  float redDC = redSum / (float)length;
  float irDC = irSum / (float)length;
  float redAC = (redMax - redMin) / 2.0;
  float irAC = (irMax - irMin) / 2.0;

  if (redDC == 0 || irAC == 0) {
    return 0;
  }

  float R = (redAC / redDC) / (irAC / irDC);
  float spo2 = 110.0 - 25.0 * R;

  if (spo2 < 70 || spo2 > 100) {
    return 0;
  }

  return spo2;
}
