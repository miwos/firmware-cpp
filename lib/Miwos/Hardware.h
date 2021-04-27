#ifndef Hardware_h
#define Hardware_h

#include <Arduino.h>
#include <LuaWrapper.h>
#include <RangeEncoder.h>

namespace Hardware {
  LuaWrapper *lua;
  uint32_t lastUpdate = 0;
  RangeEncoder encoder1(24, 25, 0, 127);
  RangeEncoder encoder2(26, 27, 0, 127);

  RangeEncoder *encoders[2] = { &encoder1, &encoder2 };

  void emitEncoderChanged(byte encoderIndex, int32_t value) {
    if (!lua->getFunction("Encoder", "handleChange")) return;
    lua->push(encoderIndex);
    lua->push(value);
    lua->call(2, 0);
  }

  void begin(LuaWrapper *lua) {
    Hardware::lua = lua;
  }

  void update() {
    // Throttle update to once every millisecond.
    uint32_t currentTime = millis();
    if (lastUpdate == currentTime) return;
    
    int32_t value;
    bool changed;

    for (byte i = 0; i < 2; i++) {
      value = encoders[i]->read(changed);
      if (changed) emitEncoderChanged(i, value);
    }

    lastUpdate = currentTime;
  }
}

#endif