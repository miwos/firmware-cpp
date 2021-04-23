#ifndef MidiWrapperUsb_h
#define MidiWrapperUsb_h

#include "MidiWrapper.h"

class MidiWrapperUsb : public MidiWrapper {
public:
  inline MidiWrapperUsb() {
    static MidiWrapper* self = this;

    usbMIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
      if (self->noteOnHandler != NULL) self->noteOnHandler(note, velocity, channel);
    });

    usbMIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
      if (self->noteOnHandler != NULL) self->noteOffHandler(note, velocity, channel);
    });
  }

  void inline sendNoteOn(byte note, byte velocity, byte channel) {
    usbMIDI.sendNoteOn(note, velocity, channel);
  }

  void inline sendNoteOff(byte note, byte velocity, byte channel) {
    usbMIDI.sendNoteOff(note, velocity, channel);
  }

  void inline sendControlChange(byte control, byte value, byte channel) {
    usbMIDI.sendControlChange(control, value, channel);
  }

  void sendProgramChange(byte program, byte channel) {
    usbMIDI.sendProgramChange(program, channel);
  }

  void sendPitchBend(int value, byte channel) {
    usbMIDI.sendPitchBend(value, channel);
  }

  void sendAfterTouch(byte pressure, byte channel) {
    usbMIDI.sendAfterTouch(pressure, channel);
  }

  void sendAfterTouchPoly(byte note, byte pressure, byte channel) {
    usbMIDI.sendAfterTouchPoly(note, pressure, channel);
  }
};

#endif