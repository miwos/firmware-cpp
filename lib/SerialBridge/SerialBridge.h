#ifndef SerialBridge_h
#define SerialBridge_h

#include <SPI.h>

#define START_OF_HEADER 0x01
#define START_OF_TEXT 0x02
#define END_OF_TRANSMISSION 0x04

class SerialBridge {
  public:
  typedef void(*CommandHandler)(int command, const char* argument);
  typedef void(*ContentHandler)(uint8_t);
  typedef void(*ContentEndHandler)(void);

  void update() {
    static char buffer[1024];
    static int bufferPosition = -1;

    if (Serial.available()) {
      char data = Serial.read();

      if (data == START_OF_HEADER) {
        // Reset the buffer.
        bufferPosition = 0;
        buffer[0] = (char) 0;
      } else if (data == START_OF_TEXT) {
        // Parse and reset the header.
        buffer[bufferPosition++] = (char) 0;
        bufferPosition = -1;
        int command = atoi(strtok(buffer, " "));
        const char* argument = strtok(NULL, " ");
        if (commandHandler) commandHandler(command, argument);
      } else if (data == END_OF_TRANSMISSION) {
        if (contentEndHandler) contentEndHandler();
      } else if (bufferPosition >= 0) {
        // Fill the header.
        buffer[bufferPosition++] = data;
      } else {
        if (contentHandler) contentHandler(data);
      }
    }    
  }

  // ? Send resource?
  void sendCommand(int command, const char* argument) {
    Serial.write(START_OF_HEADER);
    Serial.printf("%i %s", command, argument);
    Serial.write(END_OF_TRANSMISSION);
  }

  void sendCommand(int command, const char* argument, File content) {
    sendCommand(command, argument);
    while (content.available()) {
      Serial.write(content.read());
    }
    content.close();
  }

  void startContent() {
    Serial.write(START_OF_TEXT);
  }

  void endContent() {
    Serial.write(END_OF_TRANSMISSION);
  }

  void onCommand(CommandHandler handler) {
    commandHandler = handler;
  }

  void onContent(ContentHandler handler) {
    contentHandler = handler;
  }

  void onContentEnd(ContentEndHandler handler) {
    contentEndHandler = handler;
  }
  
  private:
  CommandHandler commandHandler;
  ContentHandler contentHandler;
  ContentEndHandler contentEndHandler;
};

#endif