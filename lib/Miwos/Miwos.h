#ifndef Miwos_h
#define Miwos_h

#include <SlipSerial.h>
#include <MiwosBridge.h>
#include <LuaWrapper.h>
#include <MidiWrapper.h>
#include <MidiWrapperUsb.h>
#include "TeensyInterface.h"
#include "Devices.h"

namespace Miwos {
  SLIPSerial slipSerial(Serial);
  LuaWrapper lua(&slipSerial);
  MiwosBridge bridge(&slipSerial);
  uint32_t currentTime = 0;
}

namespace Miwos { namespace LuaUtils {
  int warning(lua_State *L) {
    const char* text = lua_tostring(L, 1);
    bridge.warning(text);
    return 0;    
  }

  int info(lua_State *L) {
    const char* text = lua_tostring(L, 1);
    bridge.info(text);
    return 0;    
  }

  int getTime(lua_State *L) {
    lua.push(currentTime);
    return 1;
  }
}}

namespace Miwos { namespace LuaInterface {
  void update(uint32_t currentTime) {
    if (!lua.getFunction("Miwos", "update", false)) return;
    lua.push(currentTime);
    lua.call(1, 0);
  }

  void handleNoteOn(byte input, byte note, byte velocity, byte channel) {
    if (!lua.getFunction("Miwos", "handleNoteOn")) return;
    lua.push(input);
    lua.push(note);
    lua.push(velocity);
    lua.push(channel);
    lua.call(4, 0);
  }

  void handleNoteOff(byte input, byte note, byte velocity, byte channel) {
    if (!lua.getFunction("Miwos", "handleNoteOff")) return;
    lua.push(input);
    lua.push(note);
    lua.push(velocity);
    lua.push(channel);
    lua.call(4, 0);
  }  
}}

namespace Miwos {
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
    TeensyInterface::begin(&lua);

    // lua.registerLibrary("Teensy", TeensyInterface::library);
    lua.registerFunction("warning", LuaUtils::warning);
    lua.registerFunction("info", LuaUtils::info);
    lua.registerFunction("getTime", LuaUtils::getTime);
  }

  void update() {
    bridge.update();
    usbMIDI.read();

    uint32_t newTime = millis();
    if (currentTime != newTime) {
      currentTime = newTime;
      if (currentTime % 100 == 0) {
        LuaInterface::update(currentTime);
      }
    }
  }
}


#endif