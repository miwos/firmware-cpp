#ifndef LuaEncoderLibrary_h
#define LuaEncoderLibrary_h

#include <Encoders.h>
#include <LuaOnArduino.h>

namespace LuaEncoderLibrary {
Lua *lua;
Encoders *encoders;

void handleChange(byte encoderIndex, int32_t value) {
  if (!lua->getFunction("Encoder", "handleChange")) return;
  lua->push(encoderIndex + 1); // Use one-based index.
  lua->push(value);
  lua->call(2, 0);
}

int write(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  int32_t value = lua_tonumber(L, 2);

  RangeEncoder *encoder = encoders->getEncoder(index);
  if (encoder != NULL) encoder->write(value);
  return 0;
}

void install(LuaOnArduino *loa, Encoders *encoders) {
  LuaEncoderLibrary::lua = &(loa->lua);
  LuaEncoderLibrary::encoders = encoders;

  const luaL_reg library[] = {{"write", write}, {NULL, NULL}};
  lua->registerLibrary("Encoder", library);

  encoders->onChange(handleChange);
}

} // namespace LuaEncoderLibrary

#endif