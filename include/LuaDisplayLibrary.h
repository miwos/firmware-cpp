#ifndef LuaDisplaysLibrary_h
#define LuaDisplaysLibrary_h

#include <Displays.h>
#include <LuaOnArduino.h>

namespace LuaDisplaysLibrary {
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
  LuaDisplaysLibrary::lua = &(loa->lua);
  LuaDisplaysLibrary::displays = displays;

  const luaL_reg library[] = {{"write", write}, {NULL, NULL}};
  lua->registerLibrary("Displays", library);
}

} // namespace LuaDisplaysLibrary

#endif