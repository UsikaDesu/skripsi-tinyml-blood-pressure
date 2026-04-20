/*
 * ESP32 RAW Data Streamer
 *
 * Hardware: ESP32 + MAX30102 PPG Sensor
 * Purpose: Stream high-frequency RAW IR and RED values over Serial port
 * for PC-side signal processing and manual BP labeling.
 *
 * Wiring (I2C):
 * - SDA -> GPIO21
 * - SCL -> GPIO22
 * - VCC -> 3.3V
 * - GND -> GND
 */

#include "MAX30105.h"
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define STATUS_LED 2

MAX30105 particleSensor;

// Interval untuk mengatur kecepatan cetak (opsional jika ingin batas bawah 100Hz)
unsigned long lastSampleTime = 0;
const int sampleInterval = 10; // 10ms = 100Hz

void setup() {
  Serial.begin(115200);
  pinMode(STATUS_LED, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Mencoba inisialisasi sampai berhasil
  while (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("SYSTEM_STATUS: ERROR - MAX30102 not found!");
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    delay(1000);
  }
  
  Serial.println("SYSTEM_STATUS: OK");

  // ==========================================
  // RAW STREAMING CONFIGURATION
  // ==========================================
  byte ledBrightness = 0x7F; // 127
  byte sampleAverage = 1;    // No averaging = True RAW
  byte ledMode = 2;          // Red + IR
  int sampleRate = 100;      // 100Hz hardware sampling
  int pulseWidth = 411;      // 18-bit resolution
  int adcRange = 4096;       // Full range

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);

  // Ready message for python to detect
  Serial.println("SYSTEM_STATUS: READY_TO_STREAM");
  digitalWrite(STATUS_LED, HIGH);
}

void loop() {
  // Mengecek apakah ada data di FIFO buffer
  particleSensor.check(); 

  // Mengambil ketersediaan sampel
  while (particleSensor.available()) {
    long irValue = particleSensor.getFIFOIR();
    long redValue = particleSensor.getFIFORed();
    
    // Format wajib: T,IR,RED untuk mempermudah regex/parsing PC
    Serial.print(millis());
    Serial.print(",");
    Serial.print(irValue);
    Serial.print(",");
    Serial.print(redValue);
    Serial.println();

    particleSensor.nextSample(); // Pop data dari queue
  }
}
