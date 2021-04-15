#include <Miwos.h>

void Miwos::LuaInterface::update(uint32_t argh) {
  if (!lua.getFunction("Miwos", "update", false)) return;
  lua.push(argh);
  lua.call(1, 0);
}

void Miwos::handleOscInput(OSCBundle &oscInput) {
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

void Miwos::begin() {
  slipSerial.begin(9600);
  while (!slipSerial) {}

  bridge.begin();
  bridge.onOscInput(&handleOscInput);

  lua.onErrorBegin([]() { bridge.rawErrorBegin(); });
  lua.onErrorEnd([]() { bridge.rawErrorEnd(); });
  lua.begin();
  lua.registerLibrary("Miwos", LuaLib::library);
}

void Miwos::updateTime() {
  uint32_t newTime = millis();
  if (currentTime != newTime) {
    Serial.println(newTime);
    // currentTime = newTime;
    // if (currentTime % 1000 == 0) {
    //   LuaInterface::update(currentTime);
    // }
  }
}

void Miwos::update() {
  bridge.update();
  updateTime();
}