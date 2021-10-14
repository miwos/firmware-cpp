#ifndef LuaBridgeLibrary_h
#define LuaBridgeLibrary_h

#include <LuaOnArduino.h>
#include <LuaWrapper.h>
#include <OSCMessage.h>

namespace LuaBridgeLibrary {
Lua *lua;
Bridge *bridge;

int sendPropChange(lua_State *L) {
  byte moduleId = lua_tonumber(L, 1);
  const char *propName = lua_tostring(L, 2);
  float value = lua_tonumber(L, 3);

  OSCMessage message("/patch/prop");
  message.add(moduleId);
  message.add(propName);
  message.add(value);
  bridge->sendMessage(message);
  return 0;
}

void install(LuaOnArduino *loa) {
  LuaBridgeLibrary::lua = &(loa->lua);
  LuaBridgeLibrary::bridge = &(loa->bridge);

  const luaL_reg library[] = {{"sendPropChange", sendPropChange}, {NULL, NULL}};
  lua->registerLibrary("Bridge", library);
}
} // namespace LuaBridgeLibrary

#endif