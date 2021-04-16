#ifndef LuaWrapper_h
#define LuaWrapper_h

#include <Arduino.h>
#include <SPI.h>
#include <sdios.h>

#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"

class LuaWrapper {
public:
  lua_State *L;
  Stream* serial;

  typedef void (*Callback)();
  Callback handleErrorBegin;
  Callback handleErrorEnd;

  LuaWrapper(Stream* serial);

  void begin();
  bool check(int error);
  bool execute(const char *string);
  bool executeFile(const char* fileName);

  void registerFunction(const char *name, const lua_CFunction function);
  void registerLibrary(const char *name, const luaL_reg library[]);
  bool getFunction(const char *name);
  bool getFunction(const char *table, const char *name);
  bool getFunction(const char *name, bool logError);
  bool getFunction(const char *table, const char *name, bool logError);  
  void push(lua_Number number);
  void push(const char *string);
  void call(byte argsCount, byte resultsCount);

  void error(const char *text);
  void onErrorBegin(Callback handler) { handleErrorBegin = handler; }
  void onErrorEnd(Callback handler) { handleErrorEnd = handler; }

private:
  int luaLoadModule();
  void errorBegin();
  void errorEnd();
};

#endif