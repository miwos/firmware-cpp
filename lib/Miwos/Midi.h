#ifndef Midi_h
#define Midi_h

#include <Arduino.h>
#include <MidiWrapper.h>
#include <MidiWrapperUsb.h>
#include <LuaWrapper.h>
#include <MiwosBridge.h>

MidiWrapperUsb midi1;

namespace Midi {
  LuaWrapper *lua;
  MiwosBridge *bridge;

  const byte maxDevices = 1;
  MidiWrapper *devices[maxDevices] = { &midi1 };

  /**
   * Get a midi device.
   */
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

  namespace LuaInterface {
    void handleNoteOn(byte input, byte note, byte velocity, byte channel) {
      if (!lua->getFunction("Midi", "handleNoteOn")) return;
      lua->push(input);
      lua->push(note);
      lua->push(velocity);
      lua->push(channel);
      lua->call(4, 0);
    }

    void handleNoteOff(byte input, byte note, byte velocity, byte channel) {
      if (!lua->getFunction("Midi", "handleNoteOff")) return;
      lua->push(input);
      lua->push(note);
      lua->push(velocity);
      lua->push(channel);
      lua->call(4, 0);
    }    
  }

  namespace CppInterface {
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

  }

  /**
   * Register all midi devices.
   */
  void begin(LuaWrapper *lua, MiwosBridge *bridge) {
    Midi::lua = lua;
    Midi::bridge = bridge;

    const luaL_reg library[] = {
      { "sendNoteOn", CppInterface::sendNoteOn },
      { "sendNoteOff", CppInterface::sendNoteOff },
      { "sendControlChange", CppInterface::sendControlChange },
      { NULL, NULL }
    };
    lua->registerLibrary("Midi", library);

    for (byte i = 0; i < maxDevices; i++) {
      devices[i]->onNoteOn([](byte note, byte velocity, byte channel) {
        LuaInterface::handleNoteOn(0, note, velocity, channel);
      });

      devices[i]->onNoteOff([](byte note, byte velocity, byte channel) {
        LuaInterface::handleNoteOff(0, note, velocity, channel);
      });
    }
  }
};

#endif