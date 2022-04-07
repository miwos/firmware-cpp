#ifndef LuaDisplaysLibrary_h
#define LuaDisplaysLibrary_h

#include <Displays.h>
#include <LuaOnArduino.h>

namespace LuaDisplaysLibrary {
Lua *lua;
Displays *displays;

int write(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  const char *text = luaL_checkstring(L, 2);
  byte color = luaL_checknumber(L, 3);
  bool shouldDisplay = lua_toboolean(L, 4);

  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    display->setCursor(0, 17);
    display->setTextColor(color);
    display->print(text);
    if (shouldDisplay) display->display();
  }
  return 0;
}

int drawCircle(lua_State *L) {
  byte index = luaL_checknumber(L, 1) - 1; // Use zero-based index.
  byte x = luaL_checknumber(L, 2);
  byte y = luaL_checknumber(L, 3);
  byte radius = luaL_checknumber(L, 4);
  byte color = luaL_checknumber(L, 5);
  bool shouldFill = lua_toboolean(L, 6);
  bool shouldDisplay = lua_toboolean(L, 7);
  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    if (shouldFill) {
      display->fillCircle(x, y, radius, color);
    } else {
      display->drawCircle(x, y, radius, color);
    }
    if (shouldDisplay) display->display();
  }
  return 0;
}

int drawRect(lua_State *L) {
  byte index = luaL_checknumber(L, 1) - 1; // Use zero-based index.
  byte x = luaL_checknumber(L, 2);
  byte y = luaL_checknumber(L, 3);
  byte width = luaL_checknumber(L, 4);
  byte height = luaL_checknumber(L, 5);
  byte color = luaL_checknumber(L, 6);
  bool shouldFill = lua_toboolean(L, 7);
  bool shouldDisplay = lua_toboolean(L, 8);
  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    if (shouldFill) {
      display->fillRect(x, y, width, height, color);
    } else {
      display->drawRect(x, y, width, height, color);
    }
    if (shouldDisplay) display->display();
  }
  return 0;
}

int drawRoundedRect(lua_State *L) {
  byte index = luaL_checknumber(L, 1) - 1; // Use zero-based index.
  byte x = luaL_checknumber(L, 2);
  byte y = luaL_checknumber(L, 3);
  byte width = luaL_checknumber(L, 4);
  byte height = luaL_checknumber(L, 5);
  byte radius = luaL_checknumber(L, 6);
  byte color = luaL_checknumber(L, 7);
  bool shouldFill = lua_toboolean(L, 8);
  bool shouldDisplay = lua_toboolean(L, 9);
  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    if (shouldFill) {
      display->fillRoundRect(x, y, width, height, radius, color);
    } else {
      display->drawRoundRect(x, y, width, height, radius, color);
    }
    if (shouldDisplay) display->display();
  }
  return 0;
}

int drawLine(lua_State *L) {
  byte index = luaL_checknumber(L, 1) - 1; // Use zero-based index.
  byte x0 = luaL_checknumber(L, 2);
  byte y0 = luaL_checknumber(L, 3);
  byte x1 = luaL_checknumber(L, 4);
  byte y1 = luaL_checknumber(L, 5);
  byte color = luaL_checknumber(L, 6);

  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    if (x0 == x1) {
      display->drawFastVLine(x0, y0, abs(y1 - y0), color);
    } else if (y0 == y1) {
      display->drawFastHLine(x0, y0, abs(x1 - x0), color);
    } else {
      display->drawLine(x0, y0, x1, y1, color);
    }
  }

  return 0;
}

int display(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    display->display();
  }
  return 0;
}

int invert(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  bool state = lua_toboolean(L, 2);
  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    display->invertDisplay(state);
  }
  return 0;
}

int clear(lua_State *L) {
  byte index = lua_tonumber(L, 1) - 1; // Use zero-based index.
  bool shouldDisplay = lua_toboolean(L, 2);
  Displays::Display *display = displays->getDisplay(index);
  if (display != NULL) {
    display->clearDisplay();
    if (shouldDisplay) display->display();
  }
  return 0;
}

void install(LuaOnArduino *loa, Displays *displays) {
  LuaDisplaysLibrary::lua = &(loa->lua);
  LuaDisplaysLibrary::displays = displays;

  const luaL_reg library[] = {{"write", write}, {"drawCircle", drawCircle},
      {"display", display}, {"drawRect", drawRect},
      {"drawRoundedRect", drawRoundedRect}, {"drawLine", drawLine},
      {"invert", invert}, {"clear", clear}, {NULL, NULL}};
  lua->registerLibrary("Displays", library);
}

} // namespace LuaDisplaysLibrary

#endif