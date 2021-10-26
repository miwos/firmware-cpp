#ifndef LuaEncodersLibrary_h
#define LuaEncodersLibrary_h

#include <Encoders.h>
#include <LuaOnArduino.h>

namespace LuaEncodersLibrary {
Lua *lua;
Encoders *encoders;

void handleChange(byte encoderIndex, int32_t value) {
  if (!lua->getFunction("Encoders", "handleChange")) return;
  lua->push(encoderIndex + 1); // Use one-based index.
  lua->push(value);
  lua->call(2, 0);
}

void handleClick(byte encoderIndex) {
  if (!lua->getFunction("Encoders", "handleClick")) return;
  lua->push(encoderIndex + 1); // Use one-based index.
  lua->call(1, 0);
}

int write(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  int32_t value = lua_tonumber(L, 2);

  RangeEncoder *encoder = encoders->getEncoder(index);
  if (encoder != NULL) encoder->write(value);
  return 0;
}

void install(LuaOnArduino *loa, Encoders *encoders) {
  LuaEncodersLibrary::lua = &(loa->lua);
  LuaEncodersLibrary::encoders = encoders;

  const luaL_reg library[] = {{"write", write}, {NULL, NULL}};
  lua->registerLibrary("Encoders", library);

  encoders->onChange(handleChange);
  encoders->onClick(handleClick);
}

} // namespace LuaEncodersLibrary

#endif