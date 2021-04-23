#ifndef CppInterface_h
#define CppInterface_h

#include <LuaWrapper.h>
#include <MiwosBridge.h>
#include "Devices.h"

namespace CppInterface {
  LuaWrapper *lua;
  MiwosBridge *bridge;

  namespace Teensy {
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

    int getTime(lua_State *L) {
      lua->push(millis());
      return 1;
    }
  }

  namespace Log {
    int warning(lua_State *L) {
      const char* text = lua_tostring(L, 1);
      bridge->warning(text);
      return 0;
    }

    int info(lua_State *L) {
      const char* text = lua_tostring(L, 1);
      bridge->info(text);
      return 0;    
    }
  }

  void begin(LuaWrapper *lua, MiwosBridge *bridge) {
    CppInterface::lua = lua;
    CppInterface::bridge = bridge;

    const luaL_reg teensyLibrary[] = {
      { "sendNoteOn", Teensy::sendNoteOn },
      { "sendNoteOff", Teensy::sendNoteOff },
      { "sendControlChange", Teensy::sendControlChange },
      { "getTime", Teensy::getTime },
      { NULL, NULL }
    };

    const luaL_reg logLibrary[] = {
      { "warning", Log::warning },
      { "info", Log::info },
      { NULL, NULL }
    };

    lua->registerLibrary("Teensy", teensyLibrary);
    lua->registerLibrary("Log", logLibrary); 
  }
};

#endif