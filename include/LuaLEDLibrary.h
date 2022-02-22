#ifndef LuaLEDLibrary_h
#define LuaLEDLibrary_h

#include <LEDs.h>
#include <LuaOnArduino.h>

namespace LuaLEDLibrary {
Lua *lua;
LEDs *leds;

int write(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  byte intensity = lua_tonumber(L, 2);
  leds->write(index, intensity);
  return 0;
}

void install(LuaOnArduino *loa, LEDs *leds) {
  LuaLEDLibrary::lua = &(loa->lua);
  LuaLEDLibrary::leds = leds;

  const luaL_reg library[] = {{"write", write}, {NULL, NULL}};
  lua->registerLibrary("LEDs", library);
}
} // namespace LuaLEDLibrary

#endif