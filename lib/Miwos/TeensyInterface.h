#ifndef TeensyInterface_h
#define TeensyInterface_h

#include <LuaWrapper.h>
#include "Devices.h"

namespace TeensyInterface {
  int sendNoteOn(lua_State *L) {
    byte deviceIndex = lua_tonumber(L, 1);
    byte note = lua_tonumber(L, 2);
    byte velocity = lua_tonumber(L, 3);
    byte channel = lua_tonumber(L, 4);
    
    MidiWrapper* device = Devices::getDevice(deviceIndex);
    if (device != NULL) device->sendNoteOn(note, velocity, channel);

    return 0;
  }

  int sendNoteOff(lua_State *L) {
    byte deviceIndex = lua_tonumber(L, 1);
    byte note = lua_tonumber(L, 2);
    byte velocity = lua_tonumber(L, 3);
    byte channel = lua_tonumber(L, 4);

    MidiWrapper* device = Devices::getDevice(deviceIndex);
    if (device != NULL) device->sendNoteOff(note, velocity, channel);

    return 0;
  }

  int sendControlChange(lua_State *L) {
    byte deviceIndex = lua_tonumber(L, 1);
    byte control = lua_tonumber(L, 2);
    byte value = lua_tonumber(L, 3);
    byte channel = lua_tonumber(L, 4);

    MidiWrapper* device = Devices::getDevice(deviceIndex);
    if (device != NULL ) device->sendControlChange(control, value, channel);

    return 0;
  }

  void begin(LuaWrapper *lua) {
    const luaL_reg library[] = {
      { "sendNoteOn", sendNoteOn },
      { "sendNoteOff", sendNoteOff },
      { "sendControlChange", sendControlChange },
      { NULL, NULL }
    };

    lua->registerLibrary("Teensy", library);
  }
};

#endif