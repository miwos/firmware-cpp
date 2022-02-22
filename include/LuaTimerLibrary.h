#ifndef LuaTimerLibrary_h
#define LuaTimerLibrary_h

#include <LuaOnArduino.h>
#include <Timer.h>

namespace LuaTimerLibrary {
Lua *lua;

int now(lua_State *L) {
  lua->push(millis());
  return 1;
}

void install(LuaOnArduino *loa, Timer *timer) {
  LuaTimerLibrary::lua = &(loa->lua);
  luaL_Reg library[] = {{"now", now}, {NULL, NULL}};
  lua->registerLibrary("Timer", library);
}
} // namespace LuaTimerLibrary

#endif