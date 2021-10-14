#ifndef AnyMidi_h
#define AnyMidi_h

#include <Arduino.h>

class AnyMidi {
public:
  typedef void (*InputHandler)(
      byte type, byte data1, byte data2, byte channel, byte cable);
  InputHandler handleInput;

  virtual void update() = 0;

  virtual void send(
      byte type, byte data1, byte data2, byte channel, byte cable) = 0;

  void onInput(InputHandler handler) { handleInput = handler; }
};

#endif