#ifndef MidiUsb_h
#define MidiUsb_h

#include "AnyMidi.h"

class AnyMidiUsb : public AnyMidi {
public:
  AnyMidiUsb() {
    static AnyMidiUsb *self = this;

    usbMIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
      if (self->noteOnHandler != NULL)
        self->noteOnHandler(note, velocity, channel);
    });

    usbMIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
      if (self->noteOffHandler != NULL)
        self->noteOffHandler(note, velocity, channel);
    });

    usbMIDI.setHandleControlChange([](byte channel, byte control, byte value) {
      if (self->controlChangeHandler != NULL)
        self->controlChangeHandler(control, value, channel);
    });

    usbMIDI.setHandleProgramChange([](byte channel, byte program) {
      if (self->programChangeHandler != NULL)
        self->programChangeHandler(program, channel);
    });

    usbMIDI.setHandlePitchChange([](byte channel, int pitch) {
      if (self->pitchBendHandler != NULL)
        self->pitchBendHandler(pitch, channel);
    });

    usbMIDI.setHandleAfterTouch([](byte channel, byte pressure) {
      if (self->afterTouchHandler != NULL)
        self->afterTouchHandler(pressure, channel);
    });

    usbMIDI.setHandleAfterTouchPoly([](byte channel, byte note, byte pressure) {
      if (self->afterTouchPolyHandler != NULL)
        self->afterTouchPolyHandler(note, pressure, channel);
    });
  }

  void sendNoteOn(byte note, byte velocity, byte channel) {
    usbMIDI.sendNoteOn(note, velocity, channel);
  }

  void sendNoteOff(byte note, byte velocity, byte channel) {
    usbMIDI.sendNoteOff(note, velocity, channel);
  }

  void sendControlChange(byte control, byte value, byte channel) {
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