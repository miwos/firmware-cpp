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

  namespace LuaInterface {
    void emitEncoderChanged(byte encoderIndex, int32_t value) {
      if (!lua->getFunction("Encoder", "handleChange")) return;
      lua->push(encoderIndex);
      lua->push(value);
      lua->call(2, 0);
    }
  }

  namespace CppInterface{
    int writeEncoder(lua_State *L) {
      byte index = lua_tonumber(L, 1);
      int32_t value = lua_tonumber(L, 2);

      RangeEncoder *encoder = encoders[index];
      if (encoder != NULL) encoder->write(value);
      
      return 0;
    }
  }

  void begin(LuaWrapper *lua) {
    Hardware::lua = lua;

    luaL_Reg library[] = {
      { "write", CppInterface::writeEncoder },
      { NULL, NULL }
    };

    lua->registerLibrary("Encoder", library);    
  }

  void update() {
    // Throttle update to once every millisecond.
    uint32_t currentTime = millis();
    if (lastUpdate == currentTime) return;
    
    int32_t value;
    bool changed;

    for (byte i = 0; i < 2; i++) {
      value = encoders[i]->read(changed);
      if (changed) LuaInterface::emitEncoderChanged(i, value);
    }

    lastUpdate = currentTime;
  }
}

#endif