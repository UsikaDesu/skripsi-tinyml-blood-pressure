/*
 * ESP32 Blood Pressure Inference
 *
 * Real-time blood pressure estimation using TensorFlow Lite Micro
 * Hardware: ESP32 + MAX30102 PPG Sensor + 0.96" 128x64 OLED (SSD1306)
 *
 * Requirements:
 * - Install "TensorFlowLite_ESP32" library from Library Manager
 * - Install "SparkFun MAX3010x Pulse and Proximity Sensor" library
 * - Install "Adafruit SSD1306" library from Library Manager
 * - Install "Adafruit GFX Library" from Library Manager
 *
 * I2C Wiring (shared by MAX30102 and OLED):
 *   SDA -> GPIO 21   (both MAX30102 and OLED share this bus)
 *   SCL -> GPIO 22
 *   VCC -> 3.3V
 *   GND -> GND
 *   INT -> GPIO 19   (optional)
 *
 * Usage:
 * 1. Place finger on sensor
 * 2. Wait for stable reading
 * 3. BP estimate will be displayed on OLED and Serial
 */

#include "MAX30105.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TensorFlowLite_ESP32.h>
#include <Wire.h>

// I2C Pin Assignment
#define SDA_PIN 21
#define SCL_PIN 22

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1      // Reset pin not used
#define OLED_I2C_ADDR 0x3C // Common address; try 0x3D if not found

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "model_data.h" // Generated from convert_to_tflite.py

MAX30105 particleSensor;

// TensorFlow Lite objects
namespace {
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;

// Memory arena for TFLite (adjust if needed)
constexpr int kTensorArenaSize = 10 * 1024; // 10KB
uint8_t tensor_arena[kTensorArenaSize];
} // namespace

// Feature normalization parameters (from training)
// These will be calculated from your training data
// UPDATE THESE after training!
float feature_mean[6] = {80000, 40000, 5000,
                         2000,  75,    97};               // Placeholder values
float feature_std[6] = {20000, 10000, 2000, 1000, 15, 3}; // Placeholder values

// Sensor configuration
const int SAMPLE_WINDOW = 100;
const int SAMPLE_RATE = 100;
const int LED_BRIGHTNESS = 60;

// Status LED
const int STATUS_LED = 2;
const int ERROR_LED = 4; // Optional second LED

// Data buffers
long irBuffer[SAMPLE_WINDOW];
long redBuffer[SAMPLE_WINDOW];

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
  pinMode(ERROR_LED, OUTPUT);

  // Initialize I2C with custom pins (shared by MAX30102 and OLED)
  Wire.begin(SDA_PIN, SCL_PIN);

  // ── Initialize OLED ─────────────────────────────────────────────────
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("ERROR: SSD1306 OLED not found!");
    // Continue without OLED – Serial still works
  } else {
    // Splash screen
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println("BP Estimator");
    display.setCursor(4, 12);
    display.println("TinyML + ESP32");
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Sensor: MAX30102");
    display.setCursor(0, 42);
    display.println("Display: SSD1306");
    display.setCursor(0, 54);
    display.println("Initializing...");
    display.display();
  }

  Serial.println("====================================");
  Serial.println("ESP32 Blood Pressure Estimator");
  Serial.println("TinyML with TensorFlow Lite");
  Serial.println("====================================");

  // Initialize sensor
  Serial.println("\n1. Initializing MAX30102 sensor...");
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("   ERROR: MAX30102 not found!");
    oledMessage("ERROR:", "MAX30102", "not found!", "Check wiring.");
    while (1) {
      digitalWrite(ERROR_LED, HIGH);
      delay(100);
      digitalWrite(ERROR_LED, LOW);
      delay(100);
    }
  }

  particleSensor.setup(LED_BRIGHTNESS, 4, 2, SAMPLE_RATE, 411, 4096);
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
  Serial.println("   ✓ Sensor initialized");

  // Initialize TensorFlow Lite
  Serial.println("\n2. Initializing TensorFlow Lite...");

  // Set up logging
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Load model
  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.printf("   ERROR: Model version %d doesn't match schema %d\n",
                  model->version(), TFLITE_SCHEMA_VERSION);
    oledMessage("ERROR:", "Model schema", "mismatch!", "");
    while (1) {
      digitalWrite(ERROR_LED, HIGH);
      delay(500);
      digitalWrite(ERROR_LED, LOW);
      delay(500);
    }
  }
  Serial.println("   ✓ Model loaded");

  // Set up ops resolver
  static tflite::AllOpsResolver resolver;

  // Build interpreter
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate tensors
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    Serial.println("   ERROR: AllocateTensors() failed");
    oledMessage("ERROR:", "AllocateTensors", "failed!", "");
    while (1)
      ;
  }
  Serial.println("   ✓ Tensors allocated");

  // Get input/output tensors
  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.printf("   Input shape: [%d, %d]\n", input->dims->data[0],
                input->dims->data[1]);
  Serial.printf("   Output shape: [%d, %d]\n", output->dims->data[0],
                output->dims->data[1]);

  // Print memory usage
  size_t used_bytes = interpreter->arena_used_bytes();
  Serial.printf("   Arena used: %d / %d bytes (%.1f%%)\n", used_bytes,
                kTensorArenaSize, 100.0 * used_bytes / kTensorArenaSize);

  Serial.println("\n✓ System ready!");
  Serial.println("\nPlace your finger on the sensor...\n");

  delay(1500);
  digitalWrite(STATUS_LED, HIGH);

  // Ready prompt on OLED
  oledMessage("  >> READY <<", "", "Place finger on", "the sensor...");
  delay(1000);
}

void loop() {
  // Show measuring prompt
  oledMessage(" Measuring...", "", "Keep finger", "still on sensor.");

  // Collect sensor data
  digitalWrite(STATUS_LED, LOW);
  for (int i = 0; i < SAMPLE_WINDOW; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
    }

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();

    if (i % 20 == 0) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    }
  }
  digitalWrite(STATUS_LED, HIGH);

  // Calculate features
  float features[6];
  bool valid = calculateFeatures(irBuffer, redBuffer, SAMPLE_WINDOW, features);

  if (!valid) {
    Serial.println("? Invalid reading - check sensor contact");
    oledMessage(" No Signal!", "", "Check finger", "placement.");
    delay(2000);
    return;
  }

  // Normalize features
  for (int i = 0; i < 6; i++) {
    features[i] = (features[i] - feature_mean[i]) / feature_std[i];
  }

  // Copy to input tensor
  for (int i = 0; i < 6; i++) {
    input->data.f[i] = features[i];
  }

  // Run inference
  unsigned long inference_start = micros();
  TfLiteStatus invoke_status = interpreter->Invoke();
  unsigned long inference_time = micros() - inference_start;

  if (invoke_status != kTfLiteOk) {
    Serial.println("ERROR: Invoke failed");
    oledMessage("ERROR:", "Inference", "failed!", "");
    return;
  }

  // Get predictions (3-Class Probabilities)
  float p_hypo = output->data.f[0];
  float p_normal = output->data.f[1];
  float p_hyper = output->data.f[2];
  
  // Find highest probability (argmax)
  int predictedClass = 1; // Default to Normal
  float maxProb = p_normal;
  
  if (p_hypo > maxProb) {
    predictedClass = 0;
    maxProb = p_hypo;
  }
  if (p_hyper > maxProb) {
    predictedClass = 2;
    maxProb = p_hyper;
  }

  // Recover un-normalised HR and SpO2 for display
  float heartRate = features[4] * feature_std[4] + feature_mean[4];
  float spo2 = features[5] * feature_std[5] + feature_mean[5];

  // BP category string
  const char *category;
  if (predictedClass == 0)      category = "HYPOTENSION";
  else if (predictedClass == 1) category = "NORMAL";
  else                          category = "HYPERTENSION";

  // Serial output
  Serial.println("----------------------------------------");
  Serial.printf("Classification : %s\n", category);
  Serial.printf("Confidence     : %.1f %%\n", maxProb * 100.0);
  Serial.printf("Heart Rate     : %.1f bpm\n", heartRate);
  Serial.printf("SpO2           : %.1f %%\n", spo2);
  Serial.printf("Infer time : %lu us (%.2f ms)\n", inference_time, inference_time / 1000.0);
  Serial.println("----------------------------------------\n");

  // OLED output
  display.clearDisplay();

  // Title bar
  display.fillRect(0, 0, SCREEN_WIDTH, 12, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(18, 2);
  display.print("BP Analysis");

  // Large Classification text
  display.setTextColor(SSD1306_WHITE);
  
  // Dynamically shrink text if "HYPOTENSION" or "HYPERTENSION" is too long
  if (predictedClass == 1) {
      display.setTextSize(2); // Normal
  } else {
      display.setTextSize(1); // Hypertension/Hypotension (long word)
  }
  
  // Centre the text horizontally
  int16_t bx, by;
  uint16_t bw, bh;
  display.getTextBounds(category, 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH - bw) / 2, 20);
  display.print(category);

  // Confidence
  display.setTextSize(1);
  char confStr[20];
  snprintf(confStr, sizeof(confStr), "Conf: %.0f%%", maxProb * 100.0);
  display.getTextBounds(confStr, 0, 0, &bx, &by, &bw, &bh);
  display.setCursor((SCREEN_WIDTH - bw) / 2, 36);
  display.print(confStr);

  // Divider
  display.drawFastHLine(0, 48, SCREEN_WIDTH, SSD1306_WHITE);

  // HR | SpO2
  display.setCursor(0, 52);
  display.printf("HR:%.0f  SpO2:%.0f%%", heartRate, spo2);

  display.display();

  delay(4000); // Show result for 4 seconds
}

bool calculateFeatures(long *irBuf, long *redBuf, int length, float *features) {
  // Calculate statistics
  long irSum = 0, redSum = 0;
  long irMin = irBuf[0], irMax = irBuf[0];
  long redMin = redBuf[0], redMax = redBuf[0];

  for (int i = 0; i < length; i++) {
    irSum += irBuf[i];
    redSum += redBuf[i];

    if (irBuf[i] < irMin)
      irMin = irBuf[i];
    if (irBuf[i] > irMax)
      irMax = irBuf[i];
    if (redBuf[i] < redMin)
      redMin = redBuf[i];
    if (redBuf[i] > redMax)
      redMax = redBuf[i];
  }

  float irMean = irSum / (float)length;
  float redMean = redSum / (float)length;

  // Calculate standard deviation
  float irVariance = 0, redVariance = 0;
  for (int i = 0; i < length; i++) {
    irVariance += pow(irBuf[i] - irMean, 2);
    redVariance += pow(redBuf[i] - redMean, 2);
  }
  float irStd = sqrt(irVariance / length);
  float redStd = sqrt(redVariance / length);

  // Calculate heart rate
  float heartRate = calculateHeartRate(irBuf, length);

  // Calculate SpO2
  float spo2 = calculateSpO2(redBuf, irBuf, length);

  // Check validity
  bool valid =
      (irMean > 50000) && (heartRate > 40) && (heartRate < 180) && (spo2 > 85);

  // Fill features array
  features[0] = irMean;
  features[1] = redMean;
  features[2] = irStd;
  features[3] = redStd;
  features[4] = heartRate;
  features[5] = spo2;

  return valid;
}

float calculateHeartRate(long *buffer, int length) {
  // Find average for threshold
  long sum = 0;
  for (int i = 0; i < length; i++) {
    sum += buffer[i];
  }
  long threshold = sum / length;

  // Count peaks
  int peakCount = 0;
  bool inPeak = false;

  for (int i = 1; i < length - 1; i++) {
    if (!inPeak && buffer[i] > threshold && buffer[i] > buffer[i - 1] &&
        buffer[i] > buffer[i + 1]) {
      peakCount++;
      inPeak = true;
    } else if (buffer[i] < threshold) {
      inPeak = false;
    }
  }

  // Calculate BPM
  float timePeriod = (float)length / SAMPLE_RATE;
  float bpm = (peakCount / timePeriod) * 60.0;

  return constrain(bpm, 40, 200);
}

float calculateSpO2(long *redBuffer, long *irBuffer, int length) {
  long redSum = 0, irSum = 0;
  long redMin = redBuffer[0], redMax = redBuffer[0];
  long irMin = irBuffer[0], irMax = irBuffer[0];

  for (int i = 0; i < length; i++) {
    redSum += redBuffer[i];
    irSum += irBuffer[i];

    if (redBuffer[i] < redMin)
      redMin = redBuffer[i];
    if (redBuffer[i] > redMax)
      redMax = redBuffer[i];
    if (irBuffer[i] < irMin)
      irMin = irBuffer[i];
    if (irBuffer[i] > irMax)
      irMax = irBuffer[i];
  }

  float redDC = redSum / (float)length;
  float irDC = irSum / (float)length;
  float redAC = (redMax - redMin) / 2.0;
  float irAC = (irMax - irMin) / 2.0;

  if (redDC == 0 || irAC == 0)
    return 0;

  float R = (redAC / redDC) / (irAC / irDC);
  float spo2 = 110.0 - 25.0 * R;

  return constrain(spo2, 70, 100);
}
