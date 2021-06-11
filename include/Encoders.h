#ifndef Encoders_h
#define Encoders_h

#include <LuaOnArduino.h>
#include <RangeEncoder.h>

RangeEncoder encoder1(24, 25, 0, 127);
RangeEncoder encoder2(26, 27, 0, 127);

class Encoders {
public:
  static const byte maxEncoders = 2;
  typedef void (*ChangeHandler)(byte encoderIndex, int32_t value);

private:
  LuaOnArduino *loa;
  RangeEncoder *encoders[maxEncoders] = {&encoder1, &encoder2};
  ChangeHandler handleChange;
  uint32_t lastUpdate = 0;

public:
  Encoders(LuaOnArduino *loa) { this->loa = loa; }

  RangeEncoder *getEncoder(byte index) {
    if (index >= maxEncoders) {
      loa->logger.errorBegin();
      // Increase the index to be consistent with lua's index.
      Serial.printf(F("Encoder #%d doesn't exist."), index + 1);
      loa->logger.logEnd();
      return NULL;
    }
    return encoders[index];
  }

  void update() {
    // Throttle update to once every millisecond.
    uint32_t currentTime = millis();
    if (lastUpdate == currentTime) return;

    int32_t value;
    bool changed;

    for (byte i = 0; i < 2; i++) {
      value = encoders[i]->read(changed);
      if (changed && handleChange != NULL) handleChange(i, value);
    }

    lastUpdate = currentTime;
  }

  void onChange(ChangeHandler handler) { handleChange = handler; }
};

#endif