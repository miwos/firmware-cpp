#ifndef LEDs_h
#define LEDs_h

#include <Arduino.h>

class LEDs {
public:
  static const byte ON = 255;
  static const byte OFF = 0;
  static const byte maxLEDs = 7;

private:
  struct LED {
    byte pin;
    bool pwm;
  };

  LED leds[maxLEDs] = {
      {9, true},
      {11, true},
      {27, false},
      {14, true},
      {30, false},
      {29, true},
      {0, true},
  };

  LuaOnArduino *loa;
  byte pwmResolution = 8;
  float pwmFrequency = 585937.5;
  byte pins[maxLEDs] = {9, 11, 27, 14, 30, 29, 0};

public:
  LEDs(LuaOnArduino *loa) { this->loa = loa; }

  void begin() {
    analogWriteResolution(pwmResolution);
    for (byte i = 0; i < maxLEDs; i++) {
      byte pin = leds[i].pin;
      pinMode(pin, OUTPUT);
      if (leds[i].pwm) analogWriteFrequency(pin, pwmFrequency);
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
    byte pin = leds[index].pin;
    if (leds[index].pwm) {
      analogWrite(pin, intensity);
    } else {
      digitalWrite(pin, intensity == 0 ? LOW : HIGH);
    }
  }
};

#endif