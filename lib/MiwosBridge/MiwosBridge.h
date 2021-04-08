#ifndef MiwosBridge_h
#define MiwosBridge_h

#include <SPI.h>
#include <SD.h>
#include <SLIPSerial.h>
#include <OSCMessage.h>
#include <OSCBundle.h>

class MiwosBridge {
private:
  enum ReadSerialMode {
    ReadSerialModeOSC,
    ReadSerialModeFile
  };

  enum LogType {
    LogTypeInfo,
    LogTypeWarning,
    LogTypeError
  };

  File fileRead;
  File fileWrite;

  static const byte fileNameLength = 50;
  char fileNameRead[fileNameLength];
  char fileNameWrite[fileNameLength];

  ReadSerialMode readSerialMode = ReadSerialModeOSC;

  void handleOscInput(OSCBundle &oscInput);

  void startWriteFile(OSCMessage &fileMessage);
  void endWriteFile();

  const char* getLogAddress(LogType type);
  void log(LogType type, const char* text);

public:
  SLIPSerial* slipSerial;

  MiwosBridge(SLIPSerial *slipSerial);
  void sendMessage(OSCMessage &message);
  void sendRaw();

  void update();

  void info(const char* text);
  void warning(const char* text);
  void error(const char* text);
};

#endif