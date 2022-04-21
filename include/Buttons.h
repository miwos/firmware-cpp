#ifndef Buttons_h
#define Buttons_h

#include <Arduino.h>
#include <Button.h>
#include <LuaOnArduino.h>

// Blue buttons
Button button1(10);
Button button2(26);
Button button3(22);

// Green buttons
Button button4(15);
Button button5(31);
Button button6(32);

// Encoder buttons
Button button7(23);
Button button8(6);
Button button9(35);

class Buttons {
public:
  uint32_t debounceInterval = 10; // ms
  static const byte numButtons = 9;
  typedef void (*ClickHandler)(byte buttonIndex, uint32_t duration);
  Button *buttons[numButtons] = {&button1, &button2, &button3, &button4,
      &button5, &button6, &button7, &button8, &button9};

private:
  LuaOnArduino *loa;
  ClickHandler handleClick;
  uint32_t lastUpdate = 0;

  void _handleClick(byte index, uint32_t duration) {
    if (handleClick != NULL) handleClick(index, duration);
  }

public:
  Buttons(LuaOnArduino *loa) { this->loa = loa; }

  void begin() {
    for (byte i = 0; i < numButtons; i++) {
      buttons[i]->begin();
    }
  }

  void update() {
    uint32_t currentTime = millis();
    if (currentTime - lastUpdate < debounceInterval) return;

    bool clicked;
    uint32_t duration;
    for (byte i = 0; i < numButtons; i++) {
      clicked = buttons[i]->clicked(duration);
      if (clicked && handleClick != NULL) handleClick(i, duration);
    }

    lastUpdate = currentTime;
  }

  void onClick(ClickHandler handler) { handleClick = handler; }
};

#endif