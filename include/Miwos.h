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
  loa.begin();
  loa.install([]() {
    LuaMidiLibrary::install(&loa, &midiDevices);
    LuaEncoderLibrary::install(&loa, &encoders);
    LuaDisplayLibrary::install(&loa, &displays);
    LuaTimerLibrary::install(&loa, &timer);
  });
  displays.begin();
}

void update() {
  loa.update();
  encoders.update();
  timer.update();
}

} // namespace Miwos

#endif