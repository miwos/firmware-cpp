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

void MiwosBridge::handleOscInput(OSCBundle &oscInput) {
  // oscInput.dispatch("/read-file", handleReadFile);
  oscInput.dispatch("/file", startWriteFile);  
}

void MiwosBridge::startWriteFile(OSCMessage &fileMessage) {
  readSerialMode = ReadSerialModeFile;
  fileMessage.getString(0, fileNameWrite, fileNameLength);
  // Always override old content with new content.
  if (SD.exists(fileNameWrite)) SD.remove(fileNameWrite);
  fileWrite = SD.open(fileNameWrite, FILE_WRITE);
}

void MiwosBridge::endWriteFile() {
  readSerialMode = ReadSerialModeOSC;
  fileWrite.close();
  OSCMessage message("/success/file");
  message.add(fileNameWrite);
  sendMessage(message);
}

void MiwosBridge::update() {
  OSCBundle oscInput;
  bool receivedData = false;

  if (slipSerial->available()) {
    while (!slipSerial->endOfPacket()) {
      while (slipSerial->available()) {
        int c = slipSerial->read();
        receivedData = true;
        if (readSerialMode == ReadSerialModeOSC) {
          oscInput.fill(c);
        } else if (readSerialMode == ReadSerialModeFile) {
          fileWrite.write(c);
        }
      }
    }
  }

  if (receivedData && readSerialMode == ReadSerialModeOSC) {
    handleOscInput(oscInput);
  } else if (receivedData && readSerialMode == ReadSerialModeFile) {
    endWriteFile();
  }
}

const char* MiwosBridge::getLogAddress(LogType type) {
  switch (type) {
    case LogTypeInfo:
      return "/log/info";
    case LogTypeWarning:
      return "/log/warning";
    case LogTypeError:
      return "/log/error";
    default:
      return "/undefined";
  }
}

void MiwosBridge::log(LogType type, const char* text) {
  OSCMessage message(getLogAddress(type));
  message.add(text);
  sendMessage(message);
}

void MiwosBridge::info(const char* text) {
  log(LogTypeInfo, text);
}

void MiwosBridge::warning(const char* text) {
  log(LogTypeWarning, text);
}

void MiwosBridge::error(const char* text) {
  log(LogTypeError, text);
}  