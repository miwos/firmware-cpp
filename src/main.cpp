#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "SerialBridge.h"
#include "FileTransfer.h"

SerialBridge bridge;

enum Commands {
  CommandWrite,
  CommandRead,
  CommandWriteFinished,
  CommandFile
};

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Failed to initialize SD card!");
  }

  bridge.onCommand(handleCommand);

  fileTransfer::onWriteFinished([&](char* fileName) {
    bridge.sendCommand(CommandWriteFinished, fileName);
    Serial.write(END_OF_TRANSMISSION);
  });
}

void handleCommand(int command, const char* argument) {
  if (command == CommandWrite) {
    fileTransfer::beginWrite(argument);
  } else if (command == CommandRead) {
    bridge.sendCommand(CommandFile, argument, fileTransfer::openForRead);
    // bridge.startContent();
    // fileTransfer::read(argument);
    // bridge.endContent();
  }
}

void loop() {
  bridge.update();
  fileTransfer::update();
}