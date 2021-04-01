#include <Arduino.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Encoder.h>
#include <RangeEncoder.h>
#include <OSCBoards.h>
#include <SlipSerial.h>

RangeEncoder knob(26, 27, 0, 127);
SLIPSerial slipSerial(Serial);

void handleWriteFile(OSCMessage &message) {
  char buffer[50];
  message.getString(0, buffer, 50);
  Serial.println(buffer);
}

void handleOscInput(OSCBundle &oscBundleInput) {
  oscBundleInput.dispatch("/write-file", handleWriteFile);
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
  if (slipSerial.available()) {
    while (!slipSerial.endOfPacket()) {
      while (slipSerial.available()) {
        receivedOsc = true;
        oscBundleInput.fill(slipSerial.read());
      }
    }
  }

  if (receivedOsc && !oscBundleInput.hasError()) {
    handleOscInput(oscBundleInput);
  }  
}

void setup() {
  slipSerial.begin(9600);
}

void loop() {
  readSerial();
  readKnob();
}