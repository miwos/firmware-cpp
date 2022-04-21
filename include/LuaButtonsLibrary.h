#ifndef LuaButtonsLibrary_h
#define LuaButtonsLibrary_h

#include <Buttons.h>
#include <LuaOnArduino.h>

namespace LuaButtonsLibrary {
Lua *lua;
Buttons *buttons;

void handleClick(byte encoderIndex, uint32_t duration) {
  if (!lua->getFunction("Buttons", "handleClick")) return;
  lua->push(encoderIndex + 1); // one-based index
  lua->push(duration); // one-based index
  lua->call(2, 0);
}

void install(LuaOnArduino *loa, Buttons *buttons) {
  LuaButtonsLibrary::lua = &(loa->lua);
  LuaButtonsLibrary::buttons = buttons;
  buttons->onClick(handleClick);
}

} // namespace LuaButtonsLibrary

#endif