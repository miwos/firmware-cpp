#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
// #include "SerialBridge.h"

class SerialBridgeMessage {
  public:
  static const int bufferSize = 1024;
  char buffer[bufferSize];
  int bufferPosition = 0;


   void fill(char data) {
     buffer[bufferPosition++] = data;
   }

   int getType() {
      return atoi(strtok(buffer, " "));
   }

   int getStr() {
      return atoi(strtok(NULL, " "));
   }

};

void setup () {
   SerialBridgeMessage test;

  Serial.begin(9600);
  while (!Serial) {}   

   const char* message = "das 1";
   for (unsigned int i = 0; i < sizeof(message); i++) {
     test.fill(message[i]);
   }

   Serial.println(test.getType());
   Serial.println(test.getStr());
}

void loop() {}

// void setup() {
//   Serial.begin(9600);
//   while (!Serial) {}

//   // if (!SD.begin(BUILTIN_SDCARD)) {
//   //   Serial.println("Failed to initialize SD card!");
//   // }

//   const char* data = "99 23 yes";
//   SerialBridgeMessage message;

//   for (unsigned int i = 0; i < sizeof(data) - 1; i++) {
//     message.fill(data[i]);
//   }

//   message.parse();

//   int type = message.getType();
//   Serial.printf("Type is %i\n", type);

//   // int number = message.getInt(0);
//   // Serial.printf("Number is %i\n", number);

//   // char* string = message.getStr();
//   // Serial.printf("Number is %s\n", string);
// }

// void loop() {
//   // bridge.update();
// }