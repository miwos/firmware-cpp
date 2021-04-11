#include <Arduino.h>
#include <SlipSerial.h>
#include <OSCMessage.h>
#include <MiwosBridge.h>
#include <RangeEncoder.h>
#include <LuaWrapper.h>
#include <SdFat.h>

SLIPSerial slipSerial(Serial);
MiwosBridge bridge(&slipSerial);
RangeEncoder knob(26, 27, 0, 127);
LuaWrapper lua;

int luaLogInfo(lua_State *L) {
  const char *text = lua_tostring(L, 1);
  bridge.info(text);
  return 0;
}

int luaGetTime(lua_State *L) {
  uint32_t currentTime = millis();
  lua.push(currentTime);
  return 1;
}

int luaSendNoteOn(lua_State *L) {
  byte note = lua_tonumber(L, 1);
  byte velocity = lua_tonumber(L, 2);
  byte channel = lua_tonumber(L, 3);

  usbMIDI.sendNoteOn(note, velocity, channel);
  return 0;
}

int luaSendNoteOff(lua_State* L) {
  byte note = lua_tonumber(L, 1);
  byte velocity = lua_tonumber(L, 2);
  byte channel = lua_tonumber(L, 3);

  usbMIDI.sendNoteOff(note, velocity, channel);
  return 0;  
}

void handleOscInput(OSCBundle &oscInput) {
  oscInput.dispatch("/lua/execute-file", [](OSCMessage &message) {
    char fileName[MiwosBridge::fileNameLength];
    message.getString(0, fileName, MiwosBridge::fileNameLength);
    
    if (lua.executeFile(fileName)) {
      OSCMessage message("/success/lua/execute-file");
      bridge.sendMessage(message);
    } else {
      bridge.error("Couldn't execute lua file");
    }
  });
}

void updateKnob() {
  bool changed;
  byte value = knob.read(changed);
  if (changed) {
    OSCMessage message("/knob");
    message.add(value);
    bridge.sendMessage(message);
  }
}

void setup() {
  slipSerial.begin(9600);
  while (!Serial) {}

  bridge.begin();
  bridge.onOscInput(&handleOscInput);

  lua.onError([](const char *error) {
    bridge.error(error);
  });
  lua.begin();

  lua.registerFunction("getTime", luaGetTime);
  lua.registerFunction("info", luaLogInfo);
  lua.registerFunction("sendNoteOn", luaSendNoteOn);
  lua.registerFunction("sendNoteOff", luaSendNoteOff);
  // lua.executeFile("lua/index.lua");

  usbMIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
    if (!lua.getFunction("handleNoteOn")) return;
    // Note we use `channel` as last argument to be consistent with the
    // `sendNoteOn` function.
    lua.push(note);
    lua.push(velocity);
    lua.push(channel);
    lua.call(3, 0);
  });

  usbMIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
    if (!lua.getFunction("handleNoteOff")) return;
    lua.push(note);
    lua.push(velocity);
    lua.push(channel);
    lua.call(3, 0);
  });  
}


uint32_t currentTime = 0;
void luaSendTime() {
  uint32_t newTime = millis();
  if (currentTime != newTime) {
    currentTime = newTime;
    if (!lua.getFunction("handleTime")) return;
    lua.push(currentTime);
    lua.call(1, 0);
  }
}

void loop() {
  luaSendTime();
  bridge.update();
  updateKnob();
  usbMIDI.read();
}
