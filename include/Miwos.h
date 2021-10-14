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
Lua *lua = &loa.lua;

void handleOscInput(OSCMessage &oscInput) {
  static char name[LuaOnArduino::maxFileNameLength];

  oscInput.dispatch("/update-patch", [](OSCMessage &message) {
    uint16_t responseId = message.getInt(0);
    message.getString(1, name, LuaOnArduino::maxFileNameLength);
    if (lua->getFunction("Miwos", "updatePatch")) {
      lua->push(name);
      lua->call(1, 0);
    }
    // TODO: check if `updatePatch` actually was successful.
    loa.bridge.sendResponse(Bridge::ResponseSuccess, responseId);
  });
}

void begin() {
  loa.onInstall([]() {
    LuaMidiLibrary::install(&loa, &midiDevices);
    LuaEncoderLibrary::install(&loa, &encoders);
    LuaDisplayLibrary::install(&loa, &displays);
    LuaTimerLibrary::install(&loa, &timer);
  });

  loa.beforeReset([]() {
    if (lua->getFunction("Miwos", "destroy", false)) {
      lua->call(0, 0);
    }
  });

  loa.onOscInput(handleOscInput);

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