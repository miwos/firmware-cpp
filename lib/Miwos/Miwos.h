#ifndef Miwos_h
#define Miwos_h

#include <SlipSerial.h>
#include <MiwosBridge.h>
#include <LuaWrapper.h>
#include <MidiWrapper.h>
#include <MidiWrapperUsb.h>

namespace Miwos { namespace LuaLib {
  int sendNoteOn(lua_State *L);
  int sendNoteOff(lua_State *L);
}};

namespace Miwos { namespace LuaInterface {
  void update(uint32_t currentTime);
  void handleNoteOn(byte note, byte velocity, byte channel);
  void handleNoteOff(byte note, byte velocity, byte channel);
}};

namespace Miwos {
  void begin();
  void update();
  void updateTime();
  
  MidiWrapper* getDevice(byte index);

  void handleOscInput(OSCBundle &oscInput);
  void handleNoteOn(byte note, byte velocity, byte channel);
};


#endif