#ifndef Encoders_h
#define Encoders_h

#include <Button.h>
#include <LuaOnArduino.h>
#include <RangeEncoder.h>

RangeEncoder encoder1(3, 2, 0, 127);
RangeEncoder encoder2(36, 37, 0, 127);
RangeEncoder encoder3(33, 34, 0, 127);

Button encoder1Button(23);
Button encoder2Button(38);
Button encoder3Button(35);

class Encoders {
public:
  static const byte maxEncoders = 3;
  typedef void (*ChangeHandler)(byte encoderIndex, int32_t value);
  typedef void (*ClickHandler)(byte encoderIndex);
  RangeEncoder *encoders[maxEncoders] = {&encoder1, &encoder2, &encoder3};
  Button *buttons[maxEncoders] = {
      &encoder1Button, &encoder2Button, &encoder3Button};

private:
  LuaOnArduino *loa;
  ChangeHandler handleChange;
  ClickHandler handleClick;
  uint32_t lastUpdate = 0;

  void _handleChange(byte index, int32_t value) {
    if (handleChange != NULL) handleChange(index, value);
    OSCMessage message("/encoder/value");
    // Use one-based index to be consistent with lua.
    message.add(index + 1);
    message.add(value);
    loa->bridge.sendMessage(message);
  }

  void _handleClick(byte index) {
    if (handleClick != NULL) handleClick(index);
    OSCMessage message("/encoder/click");
    // Use one-based index to be consistent with lua.
    message.add(index + 1);
    loa->bridge.sendMessage(message);
  }

public:
  Encoders(LuaOnArduino *loa) {
    this->loa = loa;
    for (byte i = 0; i < maxEncoders; i++) {
      buttons[i]->begin();
    }
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
      if (changed) _handleChange(i, value);

      state = buttons[i]->read(changed);
      if (changed && !state) _handleClick(i);
    }

    lastUpdate = currentTime;
  }

  void onChange(ChangeHandler handler) { handleChange = handler; }
  void onClick(ClickHandler handler) { handleClick = handler; }
};

#endif