#include "Miwos.h"

MidiWrapperUsb midi1;

namespace Miwos {
  SLIPSerial slipSerial(Serial);
  LuaWrapper lua(&slipSerial);
  MiwosBridge bridge(&slipSerial);
  uint32_t currentTime = 0;

  const byte midiDevicesCount = 1;
  MidiWrapper *midiDevices[midiDevicesCount] = { &midi1 };
  MidiWrapper* getDevice(byte index) {
    if (index >= midiDevicesCount) {
      bridge.errorBegin();
      // Increase the index to be consistent with lua's index.
      Serial.printf(F("Midi device #%d doesn't exist."), index + 1);
      bridge.errorEnd();
      return NULL;
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

    lua.onErrorBegin([]() { bridge.errorBegin(); });
    lua.onErrorEnd([]() { bridge.errorEnd(); });
    lua.begin();

    lua.registerLibrary("Teensy", TeensyInterface::library);
    lua.registerFunction("warning", LuaUtils::warning);
    lua.registerFunction("info", LuaUtils::info);
    lua.registerFunction("getTime", LuaUtils::getTime);

    midiDevices[0]->onNoteOn([](byte note, byte velocity, byte channel) {
      LuaInterface::handleNoteOn(0, note, velocity, channel);
    });

    midiDevices[0]->onNoteOff([](byte note, byte velocity, byte channel) {
      LuaInterface::handleNoteOff(0, note, velocity, channel);
    });
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