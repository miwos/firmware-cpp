#ifndef LuaInterface_h
#define LuaInterface_h

#include <LuaWrapper.h>

namespace LuaInterface {
  LuaWrapper *lua;

  void handleNoteOn(byte input, byte note, byte velocity, byte channel) {
    if (!lua->getFunction("Miwos", "handleNoteOn")) return;
    lua->push(input);
    lua->push(note);
    lua->push(velocity);
    lua->push(channel);
    lua->call(4, 0);
  }

  void handleNoteOff(byte input, byte note, byte velocity, byte channel) {
    if (!lua->getFunction("Miwos", "handleNoteOff")) return;
    lua->push(input);
    lua->push(note);
    lua->push(velocity);
    lua->push(channel);
    lua->call(4, 0);
  }

  void begin(LuaWrapper *lua) {
    LuaInterface::lua = lua;
  }

  void update(uint32_t currentTime) {
    if (!lua->getFunction("Miwos", "update", false)) return;
    lua->push(currentTime);
    lua->call(1, 0);
  }    
}

#endif