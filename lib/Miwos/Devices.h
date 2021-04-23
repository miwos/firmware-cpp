#ifndef Devices_h
#define Devices_h

#include <MidiWrapper.h>
#include <MidiWrapperUsb.h>
#include <MiwosBridge.h>

MidiWrapperUsb midi1;

namespace Devices {
  MiwosBridge* bridge;
  const byte maxDevices = 1;

  MidiWrapper *devices[maxDevices] = { &midi1 };
  MidiWrapper* getDevice(byte index) {
    if (index >= maxDevices) {
      bridge->errorBegin();
      // Increase the index to be consistent with lua's index.
      Serial.printf(F("Midi device #%d doesn't exist."), index + 1);
      bridge->errorEnd();
      return NULL;
    }
    return devices[index];
  }

  void begin() {
    // devices[0]->onNoteOn([](byte note, byte velocity, byte channel) {
    //   LuaInterface::handleNoteOn(0, note, velocity, channel);
    // });

    // devices[0]->onNoteOff([](byte note, byte velocity, byte channel) {
    //   LuaInterface::handleNoteOff(0, note, velocity, channel);
    // });    
  }
};

#endif