#ifndef LuaEncodersLibrary_h
#define LuaEncodersLibrary_h

#include <Encoders.h>
#include <LuaOnArduino.h>

namespace LuaEncodersLibrary {
Lua *lua;
Encoders *encoders;
Bridge *bridge;

int write(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  int32_t value = lua_tonumber(L, 2);

  RangeEncoder *encoder = encoders->getEncoder(index);
  if (encoder != NULL) encoder->write(value);
  return 0;
}

// TODO: move to LuaAppLibrary
int selectPage(lua_State *L) {
  byte pageIndex = lua_tointeger(L, 1) - 1; // zero-based index
  OSCMessage message("/encoders/page");
  message.add(pageIndex);
  bridge->sendMessage(message);
  return 0;
}

void handleChange(byte encoderIndex, int32_t value) {
  if (!lua->getFunction("Encoders", "handleChange")) return;
  lua->push(encoderIndex + 1); // one-based index.
  lua->push(value);
  lua->call(2, 0);
}

void handleClick(byte encoderIndex) {
  if (!lua->getFunction("Encoders", "handleClick")) return;
  lua->push(encoderIndex + 1); // one-based index.
  lua->call(1, 0);
}

void install(LuaOnArduino *loa, Encoders *encoders) {
  LuaEncodersLibrary::lua = &(loa->lua);
  LuaEncodersLibrary::encoders = encoders;
  LuaEncodersLibrary::bridge = &(loa->bridge);

  const luaL_reg library[] = {
      {"write", write}, {"selectPage", selectPage}, {NULL, NULL}};
  lua->registerLibrary("Encoders", library);

  encoders->onChange(handleChange);
  encoders->onClick(handleClick);
}

} // namespace LuaEncodersLibrary

#endif