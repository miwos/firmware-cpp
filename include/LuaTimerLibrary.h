#ifndef LuaTimerLibrary_h
#define LuaTimerLibrary_h

#include <LuaOnArduino.h>
#include <Timer.h>

namespace LuaTimerLibrary {
Lua *lua;
Timer *timer;

int schedule(lua_State *L) {
  uint32_t time = lua_tonumber(L, 1);
  int16_t id = timer->schedule(time);
  lua->push(id);
  return 1;
}

int cancel(lua_State *L) {
  byte eventId = lua_tonumber(L, 1);
  timer->cancel(eventId);
  return 0;
}

int now(lua_State *L) {
  lua->push(millis());
  return 1;
}

void install(LuaOnArduino *loa, Timer *timer) {
  LuaTimerLibrary::lua = &(loa->lua);
  LuaTimerLibrary::timer = timer;

  luaL_Reg library[] = {
      {"_schedule", schedule}, {"_cancel", cancel}, {"now", now}, {NULL, NULL}};
  lua->registerLibrary("Timer", library);
}
} // namespace LuaTimerLibrary

#endif