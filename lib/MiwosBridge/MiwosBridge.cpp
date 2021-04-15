#include <Arduino.h>
#include "MiwosBridge.h"

MiwosBridge::MiwosBridge(SLIPSerial *slipSerial) {
  this->slipSerial = slipSerial;
}

void MiwosBridge::begin() {
  if (!sd.begin(SdioConfig(FIFO_SDIO))) {
    error("SD card initialization failed.");
  }
}

/**
 * Send an SLIP packet containing an OSC message and empty the message
 * afterwards.
 * @param message The OSC message.
 */
void MiwosBridge::sendMessage(OSCMessage &message) {
  slipSerial->beginPacket();
  message.send(*slipSerial);
  slipSerial->endPacket();
  message.empty();
}

/**
 * Dispatch incoming OSC messages.
 */
void MiwosBridge::handleOscInput(OSCBundle &oscInput) {
  if (oscInput.hasError()) return;

  static MiwosBridge* that = this;

  oscInput.dispatch("/read-file", [](OSCMessage &message) {
    char fileName[that->fileNameLength];
    message.getString(0, fileName, fileNameLength);
    that->readFile(fileName);
  });

  oscInput.dispatch("/file", [](OSCMessage &message) {
    message.getString(0, that->fileWriteName, fileNameLength);
    that->startWriteFile();
  });

  if (oscInputHandler != NULL) oscInputHandler(oscInput);
}

/**
 * Read and send a file from the SD card with an accompanying message.
 */
void MiwosBridge::readFile(char *fileName) {
  if (!fileRead.open(fileName, O_READ)) {
    error("Couldn't open file.");
    return;
  }

  OSCMessage message("/file");
  message.add(fileName);
  sendMessage(message);

  slipSerial->beginPacket();
  while (fileRead.available()) slipSerial->write(fileRead.read());
  slipSerial->endPacket();
  
  fileRead.close();
}

/**
 * Open and empty a file on the SD card for writing.
 */
void MiwosBridge::startWriteFile() {
  readSerialMode = ReadSerialModeFile;
  // Always override old content with new content.
  if (sd.exists(fileWriteName)) sd.remove(fileWriteName);
  fileWrite = sd.open(fileWriteName, FILE_WRITE);
}

/**
 * Close the file which has been written to and send a success message.
 */
void MiwosBridge::endWriteFile() {
  readSerialMode = ReadSerialModeOSC;
  OSCMessage message("/success/file");
  message.add(fileWriteName);
  sendMessage(message);
  fileWrite.close();
}

/**
 * Read from the serial and, depending on which `serialReadMode` we're in, treat
 * the received data as an OSC input or a file.
 */
void MiwosBridge::update() {
  OSCBundle oscInput;

  if (slipSerial->available()) {
    while (!slipSerial->endOfPacket()) {
      while (slipSerial->available()) {
        int c = slipSerial->read();
        if (readSerialMode == ReadSerialModeOSC) {
          oscInput.fill(c);
        } else if (readSerialMode == ReadSerialModeFile) {
          fileWrite.write(c);
        }
      }
    }
  }

  if (readSerialMode == ReadSerialModeOSC) {
    handleOscInput(oscInput);
  } else if (readSerialMode == ReadSerialModeFile) {
    endWriteFile();
  }
}

/**
 * Return the OSC address for a log type. 
 */
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

/**
 * Send an OSC "/log/{type}" message.
 */
void MiwosBridge::log(LogType type, const char* text) {
  OSCMessage message(getLogAddress(type));
  message.add(text);
  sendMessage(message);
}

/**
 * Send an OSC "/log/info" message.
 */
void MiwosBridge::info(const char* text) {
  log(LogTypeInfo, text);
}

/**
 * Send an OSC "/log/warning" message.
 */
void MiwosBridge::warning(const char* text) {
  log(LogTypeWarning, text);
}

/**
 * Send an OSC "/log/error" message.
 */
void MiwosBridge::error(const char* text) {
  log(LogTypeError, text);
}

void MiwosBridge::rawErrorBegin() {
  OSCMessage message("/log/raw/error");
  sendMessage(message);
  slipSerial->beginPacket();
}

void MiwosBridge::rawErrorEnd() {
  slipSerial->endPacket();
}