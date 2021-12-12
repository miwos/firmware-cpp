#ifndef Timer_h
#define Timer_h

#include <LuaOnArduino.h>

class Timer {
private:
  LuaOnArduino *loa;
  Lua *lua;

public:
  typedef byte EventId;
  struct Event {
    uint32_t time;
    bool active;
  };

  static const uint16_t maxEvents = 256;
  Event events[maxEvents];
  uint32_t currentTime = 0;

  Timer(LuaOnArduino *loa) {
    this->loa = loa;
    lua = &(loa->lua);
  }

  /**
   * Register an event (if there is an an empty slot).
   * @param time The event time.
   * @returns The event id or -1 if the event couldn't be registered.
   */
  int16_t schedule(uint32_t time) {
    for (int i = 0; i < maxEvents; i++) {
      if (!events[i].active) {
        events[i].active = true;
        events[i].time = time;
        return i;
      }
    }
    loa->logger.warn("Couldn't register event: no timer slot available");
    return -1;
  }

  /**
   * Cancel an event.
   */
  void cancel(Timer::EventId eventId) { events[eventId].active = false; }

  /**
   * Emit an event.
   */
  void emit(Timer::EventId eventId) {
    if (!lua->getFunction("Timer", "handleEmit")) return;
    lua->push(eventId);
    lua->call(1, 0);
  }

  void update() {
    uint32_t newTime = millis();
    if (currentTime != newTime) {
      currentTime = newTime;
      updateEvents();
    }
  }

private:
  void updateEvents() {
    for (int i = 0; i < maxEvents; i++) {
      if (events[i].active && events[i].time <= currentTime) {
        emit(i);
        events[i].active = false;
      }
    }
  }
};

#endif