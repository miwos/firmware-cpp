#include <Arduino.h>


SLIPSerial slipSerial(Serial);
LuaOnArduino loa(&slipSerial);

void setup() {
  while (!Serial) {}
  loa.begin();
}

void loop() {
  loa.update();
}