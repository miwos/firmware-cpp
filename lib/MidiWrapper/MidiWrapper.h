#ifndef MidiWrapper_h
#define MidiWrapper_h

#include <Arduino.h>
#include <Miwos.h>

class MidiWrapper {
public:  
  virtual void sendNoteOn(byte note, byte velocity, byte channel) = 0;
  virtual void sendNoteOff(byte note, byte velocity, byte channel) = 0;
  virtual void sendControlChange(byte control, byte value, byte channel) = 0;
  virtual void sendProgramChange(byte program, byte channel) = 0;
  virtual void sendPitchBend(int value, byte channel) = 0;
  virtual void sendAfterTouch(byte pressure, byte channel) = 0;
  virtual void sendAfterTouchPoly(byte note, byte pressure, byte channel) = 0;
};

#endif