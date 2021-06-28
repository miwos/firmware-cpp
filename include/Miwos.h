#ifndef Miwos_h
#define Miwos_h

#include <Displays.h>
#include <Encoders.h>
#include <LuaOnArduino.h>
#include <MidiDevices.h>
#include <SLIPSerial.h>
#include <Timer.h>

#include <LuaDisplayLibrary.h>
#include <LuaEncoderLibrary.h>
#include <LuaMidiLibrary.h>
#include <LuaTimerLibrary.h>

namespace Miwos {
SLIPSerial slipSerial(Serial);
LuaOnArduino loa(&slipSerial);
MidiDevices midiDevices(&loa);
Encoders encoders(&loa);
Displays displays(&loa);
Timer timer(&loa);

void begin() {
  loa.onInstall([]() {
    LuaMidiLibrary::install(&loa, &midiDevices);
    LuaEncoderLibrary::install(&loa, &encoders);
    LuaDisplayLibrary::install(&loa, &displays);
    LuaTimerLibrary::install(&loa, &timer);
  });
  loa.beforeReset([]() {
    if (loa.lua.getFunction("Miwos", "destroy", false)) {
      loa.lua.call(0, 0);
    }
  });
  loa.begin();
  displays.begin();
}

void update() {
  loa.update();
  encoders.update();
  timer.update();
  midiDevices.update();
}

} // namespace Miwos

#endif