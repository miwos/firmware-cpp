#ifndef MiwosBridge_h
#define MiwosBridge_h

#include <SPI.h>
#include <SdFat.h>
#include <SLIPSerial.h>
#include <OSCMessage.h>
#include <OSCBundle.h>

class MiwosBridge {
public:
  static const uint16_t fileNameLength = 256;
  
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

  SdFat sd;
  File fileRead;
  File fileWrite;
  char fileWriteName[fileNameLength];

  ReadSerialMode readSerialMode = ReadSerialModeOSC;

  typedef void (*OscInputHandler)(OSCBundle &oscInput);
  OscInputHandler oscInputHandler;

  void handleOscInput(OSCBundle &oscInput);
  void startWriteFile();
  void endWriteFile();
  void readFile(char* fileName);

  const char* getLogAddress(LogType type);
  void log(LogType type, const char* text);

public:
  SLIPSerial* slipSerial;

  MiwosBridge(SLIPSerial *slipSerial);
  void begin();

  void sendMessage(OSCMessage &message);
  void sendRaw();

  void update();

  void info(const char* text);
  void warning(const char* text);
  void error(const char* text);
  void errorBegin();
  void errorEnd();

  void onOscInput(OscInputHandler handler) {
    oscInputHandler = handler;
  }
};

#endif