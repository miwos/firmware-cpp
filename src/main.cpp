#include <Arduino.h>
#include <Miwos.h>
#include <Wire.h>

void setup() {
  while (!Serial) {
    ; // Wait for Serial.
  }
  Miwos::begin();
}

void loop() { Miwos::update(); }