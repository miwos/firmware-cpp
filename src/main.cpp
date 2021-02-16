#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "SerialBridge.h"

SerialBridge bridge;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Failed to initialize SD card!");
  }
}

void loop() {
  bridge.update();
}