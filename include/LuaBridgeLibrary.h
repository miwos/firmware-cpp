#ifndef LuaBridgeLibrary_h
#define LuaBridgeLibrary_h

#include <LuaOnArduino.h>
#include <LuaWrapper.h>
#include <OSCMessage.h>

enum Direction { In, Out };
enum Signal { Midi, Trigger };

namespace LuaBridgeLibrary {
Lua *lua;
Bridge *bridge;
bool connected = false;

int sendProp(lua_State *L) {
  byte instanceId = lua_tonumber(L, 1);
  const char *name = lua_tostring(L, 2);
  float value = lua_tonumber(L, 3);

  OSCMessage message("/instance/prop");
  message.add(instanceId);
  message.add(name);
  message.add(value);
  bridge->sendMessage(message);
  return 0;
}

int sendActiveOutputs(lua_State *L) {
  OSCMessage message("/bridge/active-outputs");
  message.add(lua_tostring(L, 1));
  bridge->sendMessage(message);
  return 0;
}

int sendInputOutput(lua_State *L) {
  byte signal = lua_tointeger(L, 1) - 1;    // zero-based index
  byte direction = lua_tointeger(L, 2) - 1; // zero-based index
  byte instanceId = lua_tointeger(L, 3);
  byte index = lua_tointeger(L, 4);

  OSCMessage message("/instance/in-out");
  message.add(signal);
  message.add(direction);
  message.add(instanceId);
  message.add(index);

  if (signal == Signal::Midi) {
    message.add(lua_tointeger(L, 5)); // Status
    message.add(lua_tointeger(L, 6)); // data1
    message.add(lua_tointeger(L, 7)); // data2
  }

  bridge->sendMessage(message);

  return 0;
}

int sendSelectMappingPage(lua_State *L) {
  byte pageIndex = lua_tonumber(L, 1);
  OSCMessage message("/mapping/page");
  message.add(pageIndex);
  bridge->sendMessage(message);
  return 0;
}

void install(LuaOnArduino *loa) {
  LuaBridgeLibrary::lua = &(loa->lua);
  LuaBridgeLibrary::bridge = &(loa->bridge);

  const luaL_reg library[] = {{"sendProp", sendProp},
      {"_sendInputOutput", sendInputOutput},
      {"_sendActiveOutputs", sendActiveOutputs},
      {"sendSelectMappingPage", sendSelectMappingPage}, {NULL, NULL}};
  lua->registerLibrary("Bridge", library);
}

} // namespace LuaBridgeLibrary

#endif