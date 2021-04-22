#include "Miwos.h"

MidiWrapperUsb midi1;

namespace Miwos {
  SLIPSerial slipSerial(Serial);
  LuaWrapper lua(&slipSerial);
  MiwosBridge bridge(&slipSerial);
  uint32_t currentTime = 0;

  MidiWrapper *midiDevices[1] = { &midi1 };
  MidiWrapper* getDevice(byte index) {
    if (midiDevices[index] == NULL) {
      bridge.rawErrorBegin();
      Serial.printf(F("Midi device #%s doesn't exist."), index);
      bridge.rawErrorEnd();
    }
    return midiDevices[index];
  }
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

namespace Miwos { namespace TeensyInterface {
  int sendNoteOn(lua_State *L) {
    byte deviceIndex = lua_tonumber(L, 1);
    byte note = lua_tonumber(L, 2);
    byte velocity = lua_tonumber(L, 3);
    byte channel = lua_tonumber(L, 4);
    
    MidiWrapper* device = getDevice(deviceIndex);
    if (device != NULL) device->sendNoteOn(note, velocity, channel);

    return 0;
  }

  int sendNoteOff(lua_State *L) {
    byte deviceIndex = lua_tonumber(L, 1);
    byte note = lua_tonumber(L, 2);
    byte velocity = lua_tonumber(L, 3);
    byte channel = lua_tonumber(L, 4);

    MidiWrapper* device = getDevice(deviceIndex);
    if (device != NULL) device->sendNoteOff(note, velocity, channel);

    return 0;
  }

  int sendControlChange(lua_State *L) {
    byte deviceIndex = lua_tonumber(L, 1);
    byte control = lua_tonumber(L, 2);
    byte value = lua_tonumber(L, 3);
    byte channel = lua_tonumber(L, 4);

    MidiWrapper* device = getDevice(deviceIndex);
    if (device != NULL ) device->sendControlChange(control, value, channel);

    return 0;
  }

  const luaL_reg library[] = {
    { "sendNoteOn", sendNoteOn },
    { "sendNoteOff", sendNoteOff },
    { "sendControlChange", sendControlChange },
    { NULL, NULL }
  };
}};

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

    lua.onErrorBegin([]() { bridge.rawErrorBegin(); });
    lua.onErrorEnd([]() { bridge.rawErrorEnd(); });
    lua.begin();

    lua.registerLibrary("Teensy", TeensyInterface::library);
    lua.registerFunction("warning", LuaUtils::warning);
    lua.registerFunction("info", LuaUtils::info);
    lua.registerFunction("getTime", LuaUtils::getTime);

    usbMIDI.setHandleNoteOn(&LuaInterface::handleNoteOn);
    usbMIDI.setHandleNoteOff(&LuaInterface::handleNoteOff);
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

namespace Miwos { namespace LuaInterface {
  void update(uint32_t currentTime) {
    if (!lua.getFunction("Miwos", "update", false)) return;
    lua.push(currentTime);
    lua.call(1, 0);
  }

  void handleNoteOn(byte channel, byte note, byte velocity) {
    if (!lua.getFunction("Miwos", "handleNoteOn")) return;
    lua.push(note);
    lua.push(velocity);
    lua.push(channel);
    lua.call(3, 0);
  }

  void handleNoteOff(byte channel, byte note, byte velocity) {
    if (!lua.getFunction("Miwos", "handleNoteOff")) return;
    lua.push(note);
    lua.push(velocity);
    lua.push(channel);
    lua.call(3, 0);
  }  
}}