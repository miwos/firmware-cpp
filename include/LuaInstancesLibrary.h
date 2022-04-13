#ifndef LuaInstancesLibrary_h
#define LuaInstancesLibrary_h

#include <LuaOnArduino.h>
#include <LuaWrapper.h>
#include <OSCMessage.h>

enum Direction { In, Out };
enum Signal { Midi, Trigger };

namespace LuaInstancesLibrary {
Lua *lua;
Bridge *bridge;
Logger *logger;
bool connected = false;

int _updateOutputs(lua_State *L) {
  OSCMessage message("/instances/outputs");
  message.add(lua_tostring(L, 1));
  bridge->sendMessage(message);
  return 0;
}

int updateInstance(lua_State *L) {
  byte instanceId = lua_tointeger(L, 1);
  OSCMessage message("/instances/update");
  message.add(instanceId);
  bridge->sendMessage(message);
  return 0;
}

const char *getModuleInfo(const char *moduleId) {
  if (!lua->getFunction("Modules", "getInfo")) return "";
  lua->push(moduleId);
  lua->call(1, 1);
  const char *info = lua_tostring(lua->L, -1);
  lua_pop(lua->L, 1);
  return info;
}

void install(LuaOnArduino *loa) {
  LuaInstancesLibrary::lua = &(loa->lua);
  LuaInstancesLibrary::bridge = &(loa->bridge);
  LuaInstancesLibrary::logger = &(loa->logger);

  const luaL_reg library[] = {{"_updateOutputs", _updateOutputs},
      {"updateInstance", updateInstance}, {NULL, NULL}};
  lua->registerLibrary("Instances", library);
}

} // namespace LuaInstancesLibrary

#endif