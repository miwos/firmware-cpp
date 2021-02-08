#ifndef LuaWrapper_h
#define LuaWrapper_h

#include <Arduino.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdarg.h>
// #include <Stream.h>

#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"

class LuaWrapper {
public:
  void begin();
  bool execute(const char *string);
  void registerFunction(const char *name, const lua_CFunction function);
private:
  lua_State *L;
  bool check(int error);
};

#endif