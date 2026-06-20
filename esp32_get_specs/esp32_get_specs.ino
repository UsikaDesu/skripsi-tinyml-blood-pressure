void setup() {
  Serial.begin(115200);
  delay(1000); // Tunggu Serial Monitor siap
  
  Serial.println("\n\n=================================");
  Serial.println("   ESP32 System Specifications");
  Serial.println("=================================");

  // 1. Informasi Chip (CPU)
  Serial.println("\n--- Chip Info ---");
  Serial.printf("Model Chip       : %s\n", ESP.getChipModel());
  Serial.printf("Revisi Chip      : %d\n", ESP.getChipRevision());
  Serial.printf("Jumlah Core      : %d\n", ESP.getChipCores());
  Serial.printf("Frekuensi CPU    : %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Versi SDK        : %s\n", ESP.getSdkVersion());

  // 2. Informasi Memori (RAM / Heap)
  Serial.println("\n--- Memory (RAM) ---");
  Serial.printf("Total Heap       : %d Bytes (%.2f KB)\n", ESP.getHeapSize(), ESP.getHeapSize() / 1024.0);
  Serial.printf("Free Heap        : %d Bytes (%.2f KB)\n", ESP.getFreeHeap(), ESP.getFreeHeap() / 1024.0);
  
  // Jika menggunakan PSRAM (bisa jadi 0 jika tidak ada)
  Serial.printf("Total PSRAM      : %d Bytes (%.2f MB)\n", ESP.getPsramSize(), ESP.getPsramSize() / 1048576.0);
  Serial.printf("Free PSRAM       : %d Bytes (%.2f MB)\n", ESP.getFreePsram(), ESP.getFreePsram() / 1048576.0);

  // 3. Informasi Flash Memory
  Serial.println("\n--- Flash Memory ---");
  Serial.printf("Ukuran Flash     : %d Bytes (%.2f MB)\n", ESP.getFlashChipSize(), ESP.getFlashChipSize() / 1048576.0);
  Serial.printf("Kecepatan Flash  : %d MHz\n", ESP.getFlashChipSpeed() / 1000000);
  
  // Mode Flash (QIO, QOUT, DIO, DOUT)
  FlashMode_t ideMode = ESP.getFlashChipMode();
  String modeDesc = (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN");
  Serial.printf("Mode Flash       : %s\n", modeDesc.c_str());

  // 4. Informasi Sketch (Program berjalan)
  Serial.println("\n--- Sketch Info ---");
  Serial.printf("Ukuran Sketch    : %d Bytes (%.2f KB)\n", ESP.getSketchSize(), ESP.getSketchSize() / 1024.0);
  Serial.printf("Sisa Ruang Sketch: %d Bytes (%.2f KB)\n", ESP.getFreeSketchSpace(), ESP.getFreeSketchSpace() / 1024.0);
  
  Serial.println("\n=================================");
}

void loop() {
  // Tidak ada loop
}
