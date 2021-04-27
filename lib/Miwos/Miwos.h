#ifndef Miwos_h
#define Miwos_h

#include <SlipSerial.h>
#include <MiwosBridge.h>
#include <LuaWrapper.h>
#include <MidiWrapper.h>
#include <MidiWrapperUsb.h>

#include "Midi.h"
#include "Hardware.h"
#include "Displays.h"
#include "Timer.h"
#include "Log.h"

namespace Miwos {
  SLIPSerial slipSerial(Serial);
  LuaWrapper lua(&slipSerial);
  MiwosBridge bridge(&slipSerial);
  uint32_t currentTime = 0;

  void handleOscInput(OSCBundle &oscInput) {
    oscInput.dispatch("/lua/execute-file", [](OSCMessage &message) {
      char fileName[MiwosBridge::fileNameLength];
      message.getString(0, fileName, MiwosBridge::fileNameLength);
      
      if (lua.executeFile(fileName)) {
        OSCMessage message("/success/lua/execute-file");
        bridge.sendMessage(message);
      } else {
        bridge.error("Couldn't execute lua file");
      }
    });
  }  

  void begin() {
    slipSerial.begin(9600);
    while (!slipSerial) {}

    bridge.begin();
    bridge.onOscInput(&handleOscInput);

    lua.onErrorBegin([]() { bridge.errorBegin(); });
    lua.onErrorEnd([]() { bridge.errorEnd(); });
    lua.begin();

    Displays::begin();
    Midi::begin(&lua, &bridge);
    Timer::begin(&lua);
    Hardware::begin(&lua);
    Log::begin(&lua, &bridge);
  }

  void update() {
    bridge.update();
    usbMIDI.read();
    Timer::update();
    Hardware::update();
  }
}


#endif