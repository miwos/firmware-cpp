#include "MiwosBridge.h"

MiwosBridge::MiwosBridge(SLIPSerial *slipSerial) {
  this->slipSerial = slipSerial;
}

void MiwosBridge::sendMessage(OSCMessage &message) {
  slipSerial->beginPacket();
  message.send(*slipSerial);
  slipSerial->endPacket();
  message.empty();
}
