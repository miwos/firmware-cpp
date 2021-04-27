#ifndef Timer_h
#define Timer_h

#include <Arduino.h>
#include <LuaWrapper.h>

namespace Timer {
  struct Event {
    uint32_t time;
    bool active;
  };

  LuaWrapper *lua;
  const uint16_t maxEvents = 256;
  Event events[maxEvents];
  uint32_t currentTime = 0;

  int16_t schedule(uint32_t time) {
    // Look for an empty slot and register the event.
    for (byte i = 0; i < maxEvents; i++) {
      if (!events[i].active) {
        events[i].active = true;
        events[i].time = time;
        return i;
      }
    }
    // There is no free slot.
    return -1;
  }

  void emit(byte index) {
    if (!lua->getFunction("Timer", "handleEvent")) return;
    lua->push(index);
    lua->call(1, 0);
  }

  void updateEvents() {
    for (int i = 0; i < maxEvents; i++) {
      if (events[i].active && events[i].time <= currentTime) {
        emit(i);
        events[i].active = false;
      }
    }
  }

  int cpp_schedule(lua_State *L) {
    uint32_t time = lua_tonumber(L, 1);
    int16_t id = schedule(time);
    lua->push(id);
    return 1;
  }

  void begin(LuaWrapper* lua) {
    Timer::lua = lua;

    luaL_Reg library[] = {
      { "cpp_schedule", cpp_schedule },
      { NULL, NULL }
    };
    lua->registerLibrary("Timer", library);
  }  

  void update() {
    uint32_t newTime = millis();
    if (currentTime != newTime) {
      currentTime = newTime;
      updateEvents();
    }
  }
}

#endif