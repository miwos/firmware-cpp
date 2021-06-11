#ifndef LuaMidiLibrary_h
#define LuaMidiLibrary_h

#include <AnyMidi.h>
#include <LuaOnArduino.h>
#include <LuaWrapper.h>
#include <MidiDevices.h>

namespace LuaMidiLibrary {
Lua *lua;
MidiDevices *midiDevices;

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

int sendNoteOn(lua_State *L) {
  byte deviceIndex = lua_tonumber(L, 1) - 1; // Use zero-based index.
  byte note = lua_tonumber(L, 2);
  byte velocity = lua_tonumber(L, 3);
  byte channel = lua_tonumber(L, 4);

  AnyMidi *device = midiDevices->getDevice(deviceIndex);
  if (device != NULL) device->sendNoteOn(note, velocity, channel);
  return 0;
}

int sendNoteOff(lua_State *L) {
  byte deviceIndex = lua_tonumber(L, 1) - 1; // Use zero-based index.
  byte note = lua_tonumber(L, 2);
  byte velocity = lua_tonumber(L, 3);
  byte channel = lua_tonumber(L, 4);

  AnyMidi *device = midiDevices->getDevice(deviceIndex);
  if (device != NULL) device->sendNoteOff(note, velocity, channel);
  return 0;
}

void install(LuaOnArduino *loa, MidiDevices *midiDevices) {
  LuaMidiLibrary::lua = &(loa->lua);
  LuaMidiLibrary::midiDevices = midiDevices;

  const luaL_reg library[] = {
      {"sendNoteOn", sendNoteOn}, {"sendNoteOff", sendNoteOff}, {NULL, NULL}};
  lua->registerLibrary("Midi", library);

  for (byte i = 0; i < MidiDevices::maxDevices; i++) {
    static auto deviceIndex = i;
    AnyMidi *device = midiDevices->getDevice(deviceIndex);

    device->onNoteOn([](byte note, byte velocity, byte channel) {
      handleNoteOn(deviceIndex, note, velocity, channel);
    });

    device->onNoteOff([](byte note, byte velocity, byte channel) {
      handleNoteOff(deviceIndex, note, velocity, channel);
    });
  }
}
} // namespace LuaMidiLibrary

#endif