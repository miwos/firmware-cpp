#ifndef LuaMidiLibrary_h
#define LuaMidiLibrary_h

#include <AnyMidi.h>
#include <ChordDetect.h>
#include <LuaOnArduino.h>
#include <LuaWrapper.h>
#include <MidiDevices.h>

namespace LuaMidiLibrary {
Lua *lua;
MidiDevices *midiDevices;
ChordDetect chordDetect;

void handleInput(byte index, byte type, byte data1, byte data2, byte channel,
    byte cable = 0) {
  if (!lua->getFunction("Midi", "handleInput")) return;
  lua->push(index + 1); // Use one-based index.
  lua->push(type);
  lua->push(data1);
  lua->push(data2);
  lua->push(channel);   // Channel is one-based index.
  lua->push(cable + 1); // Use one-based index.
  lua->call(6, 0);
}

int send(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  byte type = lua_tonumber(L, 2);
  byte data1 = lua_tonumber(L, 3);
  byte data2 = lua_tonumber(L, 4);
  byte channel = lua_tonumber(L, 5);   // Channel is one-based index.
  byte cable = lua_tonumber(L, 6) - 1; // Use zero-based index.

  AnyMidi *device = midiDevices->getDevice(index);
  if (device != NULL) device->send(type, data1, data2, channel, cable);
  return 0;
}

int _getNoteId(lua_State *L) {
  byte note = lua_tonumber(L, 1);
  byte channel = lua_tonumber(L, 2);
  lua->push(((channel & 0xFF) << 8) | (note & 0xFF));
  return 1;
}

int parseNoteId(lua_State *L) {
  int noteId = lua_tonumber(L, 1);
  lua->push(noteId & 0XFF);
  lua->push((noteId & 0XFF00) >> 8);
  return 2;
}

int analyzeChord(lua_State *L) {
  byte argsCount = lua_gettop(L);

  uint16_t chord = 0;
  for (byte i = 1; i <= argsCount; i++) {
    byte note = luaL_checkinteger(L, i);
    chord |= 1 << (note % 12);
  }

  byte root;
  byte mode;
  bool didMatch = chordDetect.analyze(chord, root, mode);

  lua->push(didMatch);

  if (didMatch) {
    lua->push(root);
    lua->push(mode + 1); // one-based index
    return 3;
  } else {
    return 1;
  }
}

void install(LuaOnArduino *loa, MidiDevices *midiDevices) {
  LuaMidiLibrary::lua = &(loa->lua);
  LuaMidiLibrary::midiDevices = midiDevices;

  const luaL_reg library[] = {{"send", send}, {"_getNoteId", _getNoteId},
      {"parseNoteId", parseNoteId}, {"analyzeChord", analyzeChord},
      {NULL, NULL}};
  lua->registerLibrary("Midi", library);

  for (byte i = 0; i < MidiDevices::maxDevices; i++) {
    AnyMidi *device = midiDevices->getDevice(i);
    device->onInput([](byte index, byte type, byte data1, byte data2,
                        byte channel, byte cable) {
      handleInput(index, type, data1, data2, channel, cable);
    });
  }
}
} // namespace LuaMidiLibrary

#endif