#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <SlipSerial.h>
#include <OSCLogger.h>

#include <OSCMessage.h>
#include <OSCBundle.h>

#include <Encoder.h>
#include <RangeEncoder.h>

SLIPSerial slipSerial(Serial);
OSCLogger logger(&slipSerial);

File file;
const byte fileNameLength = 50;
char fileName[fileNameLength];

RangeEncoder knob(26, 27, 0, 127);

enum ReadSerialMode {
  ReadSerialModeOSC,
  ReadSerialModeFile
};

ReadSerialMode readSerialMode = ReadSerialModeOSC;

void readFile(const char *fileName) {
  file = SD.open(fileName);
  if (file) {
    while (file.available()) {
    	Serial.write(file.read());
    }
    file.close();
  } else {
    Serial.printf("[Error] can't open file '%s'\n", fileName);
  }  
}

void handleReadFile(OSCMessage &message) {
  char fileName[fileNameLength];
  message.getString(0, fileName, fileNameLength);

  OSCMessage answerMessage("/file");
  answerMessage.add(fileName);

  slipSerial.beginPacket();
  answerMessage.send(slipSerial);
  slipSerial.endPacket();
  answerMessage.empty();

  slipSerial.beginPacket();
  readFile(fileName);
  slipSerial.endPacket();
}

void handleFile(OSCMessage &message) {
  message.getString(0, fileName, fileNameLength);
  readSerialMode = ReadSerialModeFile;
  if (SD.exists(fileName)) SD.remove(fileName);
  file = SD.open(fileName, FILE_WRITE);
}

void handleOscInput(OSCBundle &oscBundleInput) {
  oscBundleInput.dispatch("/read-file", handleReadFile);
  oscBundleInput.dispatch("/file", handleFile);
}

void readKnob() {
  bool changed;
  byte value = knob.read(changed);

  if (changed) {
    OSCMessage message("/knob");
    message.add(value);

    slipSerial.beginPacket();
    message.send(slipSerial);
    slipSerial.endPacket();

    message.empty();
  }
}

void readSerial() {
  OSCBundle oscBundleInput;

  bool receivedOsc = false;
  bool receivedFile = false;
  if (slipSerial.available()) {
    while (!slipSerial.endOfPacket()) {
      while (slipSerial.available()) {
        if (readSerialMode == ReadSerialModeOSC) {
          receivedOsc = true;
          oscBundleInput.fill(slipSerial.read());
        } else if (readSerialMode == ReadSerialModeFile) {
          file.write(slipSerial.read());
          receivedFile = true;
        }
      }
    }
  }

  if (receivedOsc && !oscBundleInput.hasError()) {
    handleOscInput(oscBundleInput);
  }

  if (receivedFile) {
    file.close();
    
    OSCMessage message("/success/file");
    message.add(fileName);
    slipSerial.beginPacket();
    message.send(slipSerial);
    slipSerial.endPacket();
    message.empty();

    readSerialMode = ReadSerialModeOSC;
  }
}

void setup() {
  slipSerial.begin(9600);
  while (!Serial) {}

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Initialization failed!");
    return;
  }
}

void loop() {
  readSerial();
  readKnob();
}