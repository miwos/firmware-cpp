#ifndef LuaDisplayLibrary_h
#define LuaDisplayLibrary_h

#include <Displays.h>
#include <LuaOnArduino.h>

namespace LuaDisplayLibrary {
Lua *lua;
Displays *displays;

int write(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  const char *text = lua_tostring(L, 2);

  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    display->clearDisplay();
    display->setCursor(0, 16);
    display->print(text);
    display->display();
  }
  return 0;
}

void install(LuaOnArduino *loa, Displays *displays) {
  LuaDisplayLibrary::lua = &(loa->lua);
  LuaDisplayLibrary::displays = displays;

  const luaL_reg library[] = {{"write", write}, {NULL, NULL}};
  lua->registerLibrary("Display", library);
}

} // namespace LuaDisplayLibrary

#endif