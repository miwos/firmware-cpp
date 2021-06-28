#ifndef MidiDevices_h
#define MidiDevices_h

#include <AnyMidi.h>
#include <AnyMidiUsb.h>
#include <LuaOnArduino.h>

AnyMidiUsb midiDevice1;

class MidiDevices {
public:
  static const byte maxDevices = 1;

private:
  LuaOnArduino *loa;
  AnyMidi *devices[maxDevices] = {&midiDevice1};

public:
  MidiDevices(LuaOnArduino *loa) { this->loa = loa; }

  AnyMidi *getDevice(byte index) {
    if (index >= maxDevices) {
      loa->logger.errorBegin();
      // Increase the index to be consistent with lua's index.
      Serial.printf(F("Midi device #%d doesn't exist."), index + 1);
      loa->logger.logEnd();
      return NULL;
    }
    return devices[index];
  }

  void update() {
    for (byte i = 0; i < maxDevices; i++) {
      devices[i]->update();
    }
  }
};

#endif