#ifndef AnyMidi_h
#define AnyMidi_h

#include <Arduino.h>

class AnyMidi {
public:
  typedef void (*NoteOnHandler)(byte note, byte velocity, byte channel);
  typedef void (*NoteOffHandler)(byte note, byte velocity, byte channel);
  typedef void (*ControlChangeHandler)(byte control, byte value, byte channel);
  typedef void (*ProgramChangeHandler)(byte program, byte channel);
  typedef void (*PitchBendHandler)(int pitch, byte channel);
  typedef void (*AfterTouchHandler)(byte pressure, byte channel);
  typedef void (*AfterTouchPolyHandler)(byte note, byte pressure, byte channel);

  NoteOnHandler noteOnHandler;
  NoteOffHandler noteOffHandler;
  ControlChangeHandler controlChangeHandler;
  ProgramChangeHandler programChangeHandler;
  PitchBendHandler pitchBendHandler;
  AfterTouchHandler afterTouchHandler;
  AfterTouchPolyHandler afterTouchPolyHandler;

  virtual void sendNoteOn(byte note, byte velocity, byte channel) = 0;
  virtual void sendNoteOff(byte note, byte velocity, byte channel) = 0;
  virtual void sendControlChange(byte control, byte value, byte channel) = 0;
  virtual void sendProgramChange(byte program, byte channel) = 0;
  virtual void sendPitchBend(int value, byte channel) = 0;
  virtual void sendAfterTouch(byte pressure, byte channel) = 0;
  virtual void sendAfterTouchPoly(byte note, byte pressure, byte channel) = 0;

  void onNoteOn(NoteOnHandler handler) { noteOnHandler = handler; }
  void onNoteOff(NoteOffHandler handler) { noteOffHandler = handler; }
  void onControlChange(ControlChangeHandler handler) {
    controlChangeHandler = handler;
  }
  void onProgramChange(ProgramChangeHandler handler) {
    programChangeHandler = handler;
  }
  void onPitchBend(PitchBendHandler handler) { pitchBendHandler = handler; }
  void onAfterTouch(AfterTouchHandler handler) { afterTouchHandler = handler; }
  void onAfterTouchPoly(AfterTouchHandler handler) {
    afterTouchHandler = handler;
  }
};

#endif