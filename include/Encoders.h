#ifndef Encoders_h
#define Encoders_h

#include <Button.h>
#include <LuaOnArduino.h>
#include <RangeEncoder.h>

RangeEncoder encoder1(24, 25, 0, 127);
RangeEncoder encoder2(26, 27, 0, 127);

Button button1(11);
Button button2(12);

class Encoders {
public:
  static const byte maxEncoders = 2;
  typedef void (*ChangeHandler)(byte encoderIndex, int32_t value);
  typedef void (*ClickHandler)(byte encoderIndex);
  RangeEncoder *encoders[maxEncoders] = {&encoder1, &encoder2};
  Button *buttons[maxEncoders] = {&button1, &button2};

private:
  LuaOnArduino *loa;
  ChangeHandler handleChange;
  ClickHandler handleClick;
  uint32_t lastUpdate = 0;

public:
  Encoders(LuaOnArduino *loa) {
    this->loa = loa;
    button1.begin();
    button2.begin();
  }

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
    bool state;
    bool changed;

    for (byte i = 0; i < maxEncoders; i++) {
      value = encoders[i]->read(changed);
      if (changed && handleChange != NULL) handleChange(i, value);

      state = buttons[i]->read(changed);
      if (changed && !state && handleClick != NULL) handleClick(i);
    }

    lastUpdate = currentTime;
  }

  void onChange(ChangeHandler handler) { handleChange = handler; }
  void onClick(ClickHandler handler) { handleClick = handler; }
};

#endif