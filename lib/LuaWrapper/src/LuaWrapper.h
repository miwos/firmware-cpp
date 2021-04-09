#ifndef LuaWrapper_h
#define LuaWrapper_h

#include <Arduino.h>
#include <SPI.h>
#include <sdios.h>

// #include <string.h>
// #include <stdarg.h>
// #include <Stream.h>

#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"

class LuaWrapper {
public:
  typedef void (*ErrorHandler)(const char *error);
  ErrorHandler handleError;

  void begin();
  bool execute(const char *string);
  bool executeFile(const char* fileName);
  void registerFunction(const char *name, const lua_CFunction function);

  void onError(ErrorHandler handler) { handleError = handler; }
private:
  lua_State *L;
  bool check(int error);
};

#endif