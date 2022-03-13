#ifndef Miwos_h
#define Miwos_h

#include <Buttons.h>
#include <Displays.h>
#include <Encoders.h>
#include <LEDs.h>
#include <LuaOnArduino.h>
#include <MidiDevices.h>
#include <SLIPSerial.h>
#include <Timer.h>

#include <LuaButtonsLibrary.h>
#include <LuaDisplaysLibrary.h>
#include <LuaEncodersLibrary.h>
#include <LuaInstancesLibrary.h>
#include <LuaLEDLibrary.h>
#include <LuaMidiLibrary.h>
#include <LuaTimerLibrary.h>

namespace Miwos {
SLIPSerial slipSerial(Serial);
LuaOnArduino loa(&slipSerial);
MidiDevices midiDevices(&loa);
Encoders encoders(&loa);
Displays displays(&loa);
Buttons buttons(&loa);
LEDs leds(&loa);
Timer timer(&loa);
Lua *lua = &loa.lua;
uint32_t currentTime = 0;

void handleOscInput(OSCMessage &oscInput) {
  static char name[LuaOnArduino::maxFileNameLength];

  oscInput.dispatch("/bridge/connect",
      [](OSCMessage &message) { LuaInstancesLibrary::connected = true; });

  oscInput.dispatch("/bridge/disconnect",
      [](OSCMessage &message) { LuaInstancesLibrary::connected = false; });

  oscInput.dispatch("/info/memory-usage", [](OSCMessage &message) {
    uint16_t responseId = message.getInt(0);

    if (!lua->getFunction("Miwos", "getMemoryUsage")) {
      loa.bridge.sendResponse(Bridge::ResponseError, responseId);
      return;
    }

    lua->push(name);
    lua->call(1, 1);
    int usedMemory = (int)lua_tointeger(lua->L, -1);
    lua_pop(lua->L, 1);

    loa.bridge.sendResponse(Bridge::ResponseSuccess, responseId, usedMemory);
  });

  oscInput.dispatch("/module/info", [](OSCMessage &message) {
    uint16_t responseId = message.getInt(0);
    message.getString(1, name, LuaOnArduino::maxFileNameLength);
    const char *info = LuaInstancesLibrary::getModuleInfo(name);
    loa.bridge.sendResponse(Bridge::ResponseSuccess, responseId, info);
  });

  oscInput.dispatch("/patch/update", [](OSCMessage &message) {
    uint16_t responseId = message.getInt(0);
    message.getString(1, name, LuaOnArduino::maxFileNameLength);
    if (lua->getFunction("Patches", "update")) {
      lua->push(name);
      lua->call(1, 0);
    }
    // TODO: check if `updatePatch` actually was successful.
    loa.bridge.sendResponse(Bridge::ResponseSuccess, responseId);
  });

  oscInput.dispatch("/patch/prop", [](OSCMessage &message) {
    int instanceId = message.getInt(0);
    message.getString(1, name, LuaOnArduino::maxFileNameLength);

    float value = 0;
    if (message.isFloat(2)) {
      value = message.getFloat(2);
    } else if (message.isInt(2)) {
      value = message.getInt(2);
    }

    if (lua->getFunction("Patches", "handlePropChange")) {
      lua->push(instanceId);
      lua->push(name);
      lua->push(value);
      lua->call(3, 0);
    }
  });

  oscInput.dispatch("/mapping/page", [](OSCMessage &message) {
    byte pageIndex = message.getInt(0);
    if (lua->getFunction("Interface", "selectPage")) {
      // Use a one-based index to be consistent with lua.
      lua->push(pageIndex + 1);
      lua->call(1, 0);
    }
  });
}

void begin() {
  loa.onInstall([]() {
    LuaMidiLibrary::install(&loa, &midiDevices);
    LuaEncodersLibrary::install(&loa, &encoders);
    LuaDisplaysLibrary::install(&loa, &displays);
    LuaButtonsLibrary::install(&loa, &buttons);
    LuaLEDLibrary::install(&loa, &leds);
    LuaTimerLibrary::install(&loa, &timer);
    LuaInstancesLibrary::install(&loa);
  });

  loa.beforeReset([]() {
    if (lua->getFunction("Miwos", "destroy", false)) {
      lua->call(0, 0);
    }
  });

  loa.onOscInput(handleOscInput);

  loa.begin();
  displays.begin();
  leds.begin();
  midiDevices.begin();
}

void update() {
  loa.update();
  encoders.update();
  buttons.update();
  timer.update();
  midiDevices.update();
}

} // namespace Miwos

#endif