#ifndef LEDs_h
#define LEDs_h

#include <Arduino.h>

class LEDs {
public:
  static const byte ON = 255;
  static const byte OFF = 0;
  static const byte maxLEDs = 9;

private:
  LuaOnArduino *loa;
  byte pwmResolution = 8;
  float pwmFrequency = 585937.5;
  byte pins[maxLEDs] = {10, 11, 12, 14, 15, 22, 5, 6, 28};

public:
  LEDs(LuaOnArduino *loa) { this->loa = loa; }

  void begin() {
    analogWriteResolution(pwmResolution);
    for (byte i = 0; i < maxLEDs; i++) {
      pinMode(pins[i], OUTPUT);
      analogWriteFrequency(pins[i], pwmFrequency);
    }
  }

  void write(byte index, byte intensity) {
    if (index >= LEDs::maxLEDs) {
      loa->logger.errorBegin();
      // Increase the index to be consistent with lua's index.
      Serial.printf(F("LED #%d doesn't exist."), index + 1);
      loa->logger.logEnd();
      return;
    }
    analogWrite(pins[index], intensity);
  }
};

#endif