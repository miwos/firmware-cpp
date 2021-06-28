#ifndef MidiUsb_h
#define MidiUsb_h

#include "AnyMidi.h"

class AnyMidiUsb : public AnyMidi {
public:
  void update() {
    if (usbMIDI.read() && handleInput != NULL) {
      Serial.print("Handle Input!");
      byte type = usbMIDI.getType();
      byte data1 = usbMIDI.getData1();
      byte data2 = usbMIDI.getData2();
      byte channel = usbMIDI.getChannel();
      handleInput(type, data1, data2, channel);
    }
  }

  void send(byte type, byte data1, byte data2, byte channel) {
    usbMIDI.send(type, data1, data2, channel, 0);
  }
};

#endif