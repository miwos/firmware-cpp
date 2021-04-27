#include <Arduino.h>
#include <Miwos.h>

void setup() {
  Miwos::begin();
}

void loop() {
  Miwos::update();
}
