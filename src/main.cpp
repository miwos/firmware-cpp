#include <Arduino.h>
#include <Miwos.h>
#include <Wire.h>

void setup() {
  while (!Serial) {
    ; // Wait for Serial.
  }
  Miwos::begin();

  // Press the first encoder button while startup to prevent the execution of
  // the lua code (note: buttons use `Active Low` scheme). This is useful for
  // debugging. If there is an error like an infinite loop in the lua code we
  // dont't want to auto-execute the code until we fixed the error.
  if (Miwos::encoders.buttons[0]->read()) {
    Miwos::lua->runFile("lua/init.lua");
  }
}

void loop() { Miwos::update(); }
