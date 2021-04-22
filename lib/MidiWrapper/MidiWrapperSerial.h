#ifndef MidiWrapperSerial_h
#define MidiWrapperSerial_h

#include "MidiWrapper.h"
#include <MIDI.h>

class MidiWrapperSerial : MidiWrapper {
public:
  
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