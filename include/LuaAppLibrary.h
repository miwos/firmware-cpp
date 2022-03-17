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

int sendMessage(lua_State *L) {
  int argsCount = lua_gettop(L);
  const char *name = luaL_checkstring(L, 1);
  OSCMessage message(name);

  for (int i = 2; i <= argsCount; i++) {
    if (lua_isboolean(L, i)) {
      message.add(lua_toboolean(L, i));
    } else if (lua_isnumber(L, i)) {
      message.add(lua_tonumber(L, i));
    } else if (lua_isstring(L, i)) {
      message.add(lua_tostring(L, i));
    }
  }

  bridge->sendMessage(message);
  return 0;
}

void install(LuaOnArduino *loa) {
  LuaAppLibrary::bridge = &(loa->bridge);

  const luaL_reg library[] = {
      {"selectPart", selectPart}, {"sendMessage", sendMessage}, {NULL, NULL}};
  loa->lua.registerLibrary("App", library);
}
} // namespace LuaAppLibrary

#endif