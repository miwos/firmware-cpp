#ifndef LuaMidiLibrary_h
#define LuaMidiLibrary_h

#include <AnyMidi.h>
#include <LuaOnArduino.h>
#include <LuaWrapper.h>
#include <MidiDevices.h>

namespace LuaMidiLibrary {
Lua *lua;
MidiDevices *midiDevices;

void handleInput(byte index, byte type, byte data1, byte data2, byte channel,
    byte cable = 0) {
  if (!lua->getFunction("Midi", "handleInput")) return;
  lua->push(index + 1); // Use one-based index.
  lua->push(type);
  lua->push(data1);
  lua->push(data2);
  lua->push(channel);
  lua->call(5, 0);
}

int send(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  byte type = lua_tonumber(L, 2);
  byte data1 = lua_tonumber(L, 3);
  byte data2 = lua_tonumber(L, 4);
  byte channel = lua_tonumber(L, 5);
  byte cable = lua_tonumber(L, 6);

  AnyMidi *device = midiDevices->getDevice(index);
  if (device != NULL) device->send(type, data1, data2, channel, cable);
  return 0;
}

void install(LuaOnArduino *loa, MidiDevices *midiDevices) {
  LuaMidiLibrary::lua = &(loa->lua);
  LuaMidiLibrary::midiDevices = midiDevices;

  const luaL_reg library[] = {{"send", send}, {NULL, NULL}};
  lua->registerLibrary("Midi", library);

  for (byte i = 0; i < MidiDevices::maxDevices; i++) {
    static auto deviceIndex = i;
    AnyMidi *device = midiDevices->getDevice(deviceIndex);

    device->onInput(
        [](byte type, byte data1, byte data2, byte channel, byte cable) {
          handleInput(deviceIndex, type, data1, data2, channel, cable);
        });
  }
}
} // namespace LuaMidiLibrary

#endif