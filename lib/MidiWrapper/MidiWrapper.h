#ifndef MiwosMidiDevice_h
#define MiwosMidiDevice_h

#include <Arduino.h>
#include <Miwos.h>

class MidiWrapper {
public:
  typedef void (*NoteOnHandler)(byte note, byte velocity, byte channel);
  typedef void (*NoteOffHandler)(byte note, byte velocity, byte channel);
  NoteOnHandler noteOnHandler;
  NoteOffHandler noteOffHandler;

  virtual void sendNoteOn(byte note, byte velocity, byte channel) = 0;
  virtual void sendNoteOff(byte note, byte velocity, byte channel) = 0;
  virtual void sendControlChange(byte control, byte value, byte channel) = 0;
  virtual void sendProgramChange(byte program, byte channel) = 0;
  virtual void sendPitchBend(int value, byte channel) = 0;
  virtual void sendAfterTouch(byte pressure, byte channel) = 0;
  virtual void sendAfterTouchPoly(byte note, byte pressure, byte channel) = 0;

  inline void onNoteOn(NoteOnHandler handler) { noteOnHandler = handler; }
  inline void onNoteOff(NoteOffHandler handler) { noteOffHandler = handler; }
};

#endif