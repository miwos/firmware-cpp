#include <Arduino.h>
#include <Miwos.h>

uint32_t currentTime = 0;

void setup() {
  Miwos::begin();
}

void loop() {
  Miwos::update();
}


// #include <Arduino.h>
// #include <SlipSerial.h>
// #include <OSCMessage.h>
// #include <MiwosBridge.h>
// #include <RangeEncoder.h>
// #include <LuaWrapper.h>
// #include <SdFat.h>

// SLIPSerial slipSerial(Serial);
// MiwosBridge bridge(&slipSerial);
// LuaWrapper lua;

// int luaLogInfo(lua_State *L) {
//   const char *text = lua_tostring(L, 1);
//   bridge.info(text);
//   return 0;
// }

// int luaGetTime(lua_State *L) {
//   uint32_t currentTime = millis();
//   lua.push(currentTime);
//   return 1;
// }

// void handleOscInput(OSCBundle &oscInput) {
//   oscInput.dispatch("/lua/execute-file", [](OSCMessage &message) {
//     char fileName[MiwosBridge::fileNameLength];
//     message.getString(0, fileName, MiwosBridge::fileNameLength);
    
//     if (lua.executeFile(fileName)) {
//       OSCMessage message("/success/lua/execute-file");
//       bridge.sendMessage(message);
//     } else {
//       bridge.error("Couldn't execute lua file");
//     }
//   });
// }

// void setup() {
//   slipSerial.begin(9600);
//   while (!Serial) {}

//   bridge.begin();
//   bridge.onOscInput(&handleOscInput);

//   lua.onError([](const char *error) {
//     bridge.error(error);
//   });
//   lua.begin();

//   lua.registerFunction("getTime", luaGetTime);
//   lua.registerFunction("info", luaLogInfo);
//   lua.executeFile("lua/index.lua");
// }

// uint32_t currentTime = 0;
// void luaUpdate() {
//   uint32_t newTime = millis();
//   if (currentTime != newTime) {
//     currentTime = newTime;
//     if (!lua.getFunction("Miwos", "update")) return;     
//     lua.push(currentTime);
//     lua.call(1, 0);
//   }
// }

// void loop() {
//   if (millis() % 100 == 0) luaUpdate();
//   bridge.update();
// }
