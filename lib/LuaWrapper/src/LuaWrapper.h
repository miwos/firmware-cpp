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
  lua_State *L;
  typedef void (*ErrorHandler)(const char *error);
  ErrorHandler handleError;

  void begin();
  bool check(int error);
  bool execute(const char *string);
  bool executeFile(const char* fileName);

  void registerFunction(const char *name, const lua_CFunction function);
  bool getFunction(const char *name);
  void push(lua_Number number);
  void push(const char *string);
  void call(byte argsCount, byte resultsCount);

  void error(const char *text);
  void onError(ErrorHandler handler) { handleError = handler; }
  
private:
  int luaLoadModule();
};

#endif