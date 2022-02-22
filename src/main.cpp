#include <Arduino.h>
#include <Miwos.h>

extern unsigned long _heap_start;
extern unsigned long _heap_end;
extern char *__brkval;

int freeRam() { return (char *)&_heap_end - __brkval; }

void setup() {
  // Serial.begin(9600);
  while (!Serial) {
    ; // Wait for Serial.
  }

  Serial.print(freeRam());

  Miwos::begin();

  // Press the first encoder button while startup to prevent the execution of
  // the lua code (note: buttons use `Active Low` scheme). This is useful for
  // debugging. If there is an error like an infinite loop in the lua code we
  // dont't want to auto-execute the code until we fixed the error.
  // if (Miwos::encoders.buttons[0]->read()) {
  Miwos::lua->runFile("lua/init.lua");
  // }
}

void loop() { Miwos::update(); }
