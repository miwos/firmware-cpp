#ifndef Miwos_h
#define Miwos_h

#include <SlipSerial.h>
#include <MiwosBridge.h>
#include <LuaWrapper.h>
#include <MidiWrapper.h>
#include <MidiWrapperUsb.h>

#include "CppInterface.h"
#include "LuaInterface.h"
#include "Devices.h"
#include "Displays.h"
#include "Timer.h"

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

    Devices::begin();
    Displays::begin();
    Timer::begin(&lua);
    CppInterface::begin(&lua, &bridge);
    LuaInterface::begin(&lua);
  }

  void update() {
    bridge.update();
    usbMIDI.read();
    Timer::update();
  }
}


#endif