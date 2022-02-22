#ifndef Timer_h
#define Timer_h

#include <LuaOnArduino.h>

class Timer {
private:
  LuaOnArduino *loa;
  Lua *lua;

public:
  uint32_t currentTime = 0;

  Timer(LuaOnArduino *loa) {
    this->loa = loa;
    lua = &(loa->lua);
  }

  void update() {
    uint32_t newTime = millis();
    if (currentTime != newTime) {
      currentTime = newTime;
      if (!lua->getFunction("Timer", "update")) return;
      lua->push(currentTime);
      lua->call(1, 0);
    }
  }
};

#endif