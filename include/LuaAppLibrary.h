#ifndef LuaAppLibrary_h
#define LuaAppLibrary_h

#include <LuaOnArduino.h>

namespace LuaAppLibrary {
Bridge *bridge;

int selectPart(lua_State *L) {
  byte partIndex = lua_tointeger(L, 1) - 1; // zero-based index
  OSCMessage message("/parts/select");
  message.add(partIndex);
  bridge->sendMessage(message);
  return 0;
}

void install(LuaOnArduino *loa) {
  LuaAppLibrary::bridge = &(loa->bridge);

  const luaL_reg library[] = {{"selectPart", selectPart}, {NULL, NULL}};
  loa->lua.registerLibrary("App", library);
}
} // namespace LuaAppLibrary

#endif