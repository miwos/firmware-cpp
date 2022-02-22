#ifndef Buttons_h
#define Buttons_h

#include <Arduino.h>
#include <Button.h>
#include <LuaOnArduino.h>

Button button1(26);
Button button2(27);
Button button3(29);
Button button4(30);
Button button5(31);
Button button6(32);

class Buttons {
public:
  static const byte maxButtons = 6;
  typedef void (*ClickHandler)(byte buttonIndex);
  Button *buttons[maxButtons] = {
      &button1, &button2, &button3, &button4, &button5, &button6};

private:
  LuaOnArduino *loa;
  ClickHandler handleClick;
  uint32_t lastUpdate = 0;

  void _handleClick(byte index) {
    if (handleClick != NULL) handleClick(index);
    OSCMessage message("/button/click");
    // Use one-based index to be consistent with lua.
    message.add(index + 1);
    loa->bridge.sendMessage(message);
  }

public:
  Buttons(LuaOnArduino *loa) {
    this->loa = loa;
    for (byte i = 0; i < maxButtons; i++) {
      buttons[i]->begin();
    }
  }

  void update() {
    // Throttle update to once every millisecond.
    uint32_t currentTime = millis();
    if (lastUpdate == currentTime) return;

    bool state;
    bool changed;

    for (byte i = 0; i < maxButtons; i++) {
      state = buttons[i]->read(changed);
      if (changed && !state) _handleClick(i);
    }

    lastUpdate = currentTime;
  }

  void onClick(ClickHandler handler) { handleClick = handler; }
};

#endif