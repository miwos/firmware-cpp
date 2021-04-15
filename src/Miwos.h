#ifndef Miwos_h
#define Miwos_h

#include <SlipSerial.h>
#include <MiwosBridge.h>
#include <LuaWrapper.h>

uint32_t currentTime = 0;

namespace Miwos { namespace LuaLib {
  static int sendNoteOn(lua_State *L) {
    byte note = lua_tonumber(L, 1);
    byte velocity = lua_tonumber(L, 2);
    byte channel = lua_tonumber(L, 1);
    usbMIDI.sendNoteOn(note, velocity, channel);
    return 0;
  }

  static int sendNoteOff(lua_State *L) {
    byte note = lua_tonumber(L, 1);
    byte velocity = lua_tonumber(L, 2);
    byte channel = lua_tonumber(L, 1);
    usbMIDI.sendNoteOff(note, velocity, channel);
    return 0;
  }

  static const luaL_reg library[] = {
    { "sendNoteOn", sendNoteOn },
    { "sendNoteOff", sendNoteOff },
    { NULL, NULL }
  };
}};

namespace Miwos {
  static SLIPSerial slipSerial(Serial);
  static LuaWrapper lua(&slipSerial);
  static MiwosBridge bridge(&slipSerial);

  void handleOscInput(OSCBundle &oscInput);
  void begin();
  void update();
  void updateTime();
};

namespace Miwos { namespace LuaInterface {
  void update(uint32_t argh);
}};


#endif