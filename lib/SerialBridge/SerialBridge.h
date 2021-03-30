#ifndef SerialBridge_h
#define SerialBridge_h

#include <SPI.h>
#include <SD.h>

#define START_OF_HEADER 0x01
#define START_OF_TEXT 0x02
#define END_OF_TRANSMISSION 0x04
#define ENQUIRY 0x05

class SerialBridgeMessage {
  private:
  static const int bufferSize = 1024;
  char buffer[bufferSize];
  int bufferPosition = 0;

  int type;
  const char* segmentSeparator = " ";
  bool isParsing = false;

  void parse() {
    if (!isParsing) {
      type = atoi(strtok(buffer, segmentSeparator));
      isParsing = true;
    }
  }

  public:
  void fill(char data) {
    if (bufferPosition < bufferSize - 1) {
      buffer[bufferPosition++] = data;
    }
  }

  int getType() {
    parse();
    return type;
  }

  int getInt() {
    parse();
    return atoi(strtok(NULL, " "));
  }

  double getFloat() {
    parse();
    return atof(strtok(NULL, " "));
  }

  char* getStr() {
    parse();
    return strtok(NULL, " ");    
  }
};

// class SerialBridge {
//   private:
//   // The buffer is used to store the command header (command name and
//   // argument).Additional content is not stored in the buffer but written
//   // directly to a file.
//   char buffer[1024];
//   int bufferPosition = -1;

//   void sendHeader(int type, const char* payload, bool hasContent = false) {
//     Serial.write(START_OF_HEADER);
//     Serial.printf("%i %s", type, payload);
//     if (!hasContent) Serial.write(END_OF_TRANSMISSION);
//   }

//   void sendResponseHeader(
//     int type,
//     int id,
//     const char* payload,
//     bool hasContent = false
//   ) {
//     Serial.write(START_OF_HEADER);
//     Serial.printf("%i %s %s", type, id, payload);
//     if (!hasContent) Serial.write(END_OF_TRANSMISSION);
//   }

//   void sendResponseHeader(
//     int type,
//     int id,
//     bool hasContent = false
//   ) {
//     Serial.write(START_OF_HEADER);
//     Serial.printf("%i %s", type, id);
//     if (!hasContent) Serial.write(END_OF_TRANSMISSION);
//   }   

//   void handleMessage() {

//   }

//   public:
//   enum MessageTypes {
//     MessageSuccess = (int) 'S',
//     MessageError = (int) 'E'
//   };

//   void update() {
//     if (Serial.available()) {
//       char data = Serial.read();

//       if (data == START_OF_HEADER) {
//         // A new message begins.
//         bufferPosition = 0;
//         buffer[0] = (char) 0;
//       } else if (data == START_OF_TEXT || data == END_OF_TRANSMISSION) {
//         // The message header ends (either with a `START_OF_TEXT`, if additional
//         // file content follows, or an `END_OF_TRANSMISSION`), so we can parse
//         // the header.
//         buffer[bufferPosition++] = (char) 0;



//         int command = atoi(strtok(buffer, " "));
//         int id = atoi(strtok(NULL, " "));
//         const char* argument = strtok(NULL, " ");
//         handleCommand(command, id, argument);
//       } else if (isReceivingFile) {
//         // Write content to file.
//         if (fileReceive) fileReceive.write(data);
//       } else if (bufferPosition < 1024 - 1) {
//         // Fill the command buffer.
//         buffer[bufferPosition++] = data;
//       }
//     }
//   }

//   void action(int type, const char* payload) {

//   }

//   void event(int type, const char* payload) {

//   }

//   void response(int type, int id, const char* payload) {
//     sendResponseHeader(type, id, payload, false);
//   }

//   void response(int type, int id) {
//     sendResponseHeader(type, id, false);
//   }  

//   void success(int id, const char* payload) {
//     response(MessageSuccess, id, payload);
//   }

//   void success(int id) {
//     response(MessageSuccess, id);
//   }

//   void error(int id, const char* payload) {
//     response(MessageError, id, payload);
//   }

//   void error(int id) {
//     response(MessageError, id);
//   }  
// };


// class SerialBridgeOLD {
//   private:
//   // The buffer is used to store the command header (command name and
//   // argument).Additional content is not stored in the buffer but written
//   // directly to a file.
//   char buffer[1024];
//   int bufferPosition = -1;
//   bool isReceivingFile = false;

//   File fileSend;
//   File fileReceive;

//   enum MessageTypes {
//     MessageSuccess = (int) 'S',
//     MessageError = (int) 'E',
//     MessageReadFile = (int) 'R',
//     MessageWriteFile = (int) 'W'
//   };

//   typedef void(*CommandHandler)(int command, int id, const char* argument);
//   CommandHandler commandHandler;

//   public:
//   /**
//    * Read and parse the serial for commands.
//    */
//   void update() {
//     if (Serial.available()) {
//       char data = Serial.read();

//       if (data == START_OF_HEADER) {
//         // A new message begins.
//         bufferPosition = 0;
//         buffer[0] = (char) 0;
//       } else if (data == END_OF_TRANSMISSION && isReceivingFile) {
//         // When we are receiving a file, an `END_OF_TRANSMISSION` signals that
//         // the content is finished.
//         if (fileReceive) fileReceive.close();
//         isReceivingFile = false;
//       } else if (data == START_OF_TEXT || data == END_OF_TRANSMISSION) {
//         // The message header ends (either with a `START_OF_TEXT`, if additional
//         // file content follows, or an `END_OF_TRANSMISSION`), so we can parse
//         // the header.
//         buffer[bufferPosition++] = (char) 0;
//         isReceivingFile = (data == START_OF_TEXT);
//         int command = atoi(strtok(buffer, " "));
//         int id = atoi(strtok(NULL, " "));
//         const char* argument = strtok(NULL, " ");
//         handleCommand(command, id, argument);
//       } else if (isReceivingFile) {
//         // Write content to file.
//         if (fileReceive) fileReceive.write(data);
//       } else if (bufferPosition < 1024 - 1) {
//         // Fill the command buffer.
//         buffer[bufferPosition++] = data;
//       }
//     }
//   }

//   /**
//    * Handle the command. `CommandWriteFile = nd `CommandReadFile` are reserved
//    * for handling files, all other commands are passed to the command handler.
//    */
//   void handleCommand(int command, int id, const char* argument) {
//     if (command == CommandReadFile) {
//       sendHeader(command, id);
//       sendFile(argument);
//     } else if (command == CommandWriteFile) {
//       receiveFile(argument);
//     } else if (commandHandler) {
//       commandHandler(command, id, argument);
//     }
//   }

//   /**
//    * Send a command header to the serial.
//    */
//   void sendHeader(int command, const char* argument) {
//     Serial.write(START_OF_HEADER);
//     Serial.printf("%i %s", command, argument);
//   }
  
//   void sendHeader(int command, int argument) {
//     Serial.write(START_OF_HEADER);
//     Serial.printf("%i %i", command, argument);
//   }

//   /**
//    * Send a command to the serial.
//    */
//   void sendCommand(int command, const char* argument) {
//     sendHeader(command, argument);
//     Serial.write(END_OF_TRANSMISSION);
//   }

//   /**
//    * Send a file to the serial.
//    */
//   void sendFile(const char* fileName) {
//     Serial.write(START_OF_TEXT);

//     fileSend = SD.open(fileName);
//     while (fileSend.available()) {
//       Serial.write(fileSend.read());
//     }
//     fileSend.close();

//     Serial.write(END_OF_TRANSMISSION);
//   }

//   /**
//    * Begin receiving a file through the serial. The file's content is then 
//    * written asynchronously in the `update()` method.
//    */
//   void receiveFile(const char* fileName) {
//     if (SD.exists(fileName)) SD.remove(fileName);
//     fileReceive = SD.open(fileName, FILE_WRITE);
//   }

//   /**
//    * Register a command handler.
//    */
//   void onCommand(CommandHandler handler) {
//     commandHandler = handler;
//   }
// };

#endif