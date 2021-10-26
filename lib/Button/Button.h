#ifndef Button_h
#define Button_h

class Button {
private:
  bool state = false;
  byte pin;

public:
  Button(byte pin) { this->pin = pin; }

  void begin() { pinMode(pin, INPUT_PULLUP); }

  bool read(bool &changed) {
    bool oldState = state;
    state = digitalRead(pin);
    changed = state != oldState;
    return state;
  }
};

#endif