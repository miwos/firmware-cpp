#ifndef Log_h
#define Log_h

#include <SlipSerial.h>
#include <OSCMessage.h>

class OSCLogger {
public:
  SLIPSerial* slipSerial;

  enum LogType {
    LogTypeInfo,
    LogTypeWarning,
    LogTypeError
  };

  OSCLogger(SLIPSerial* slipSerial) {
    this->slipSerial = slipSerial;
  }

  char* getLogAddress(LogType type) {
    switch (type) {
      case LogTypeInfo:
        return "/log/info";
      case LogTypeWarning:
        return "/log/warning";
      case LogTypeError:
        return "/log/erro";
    }
  }

  void log(LogType type, const char* text) {
    OSCMessage message(getLogAddress(type));
    message.add(text);
    slipSerial->beginPacket();
    message.send(*slipSerial);
    slipSerial->endPacket();
    message.empty();
  }

  void info(const char* text) {
    log(LogTypeInfo, text);
  }

  void warning(const char* text) {
    log(LogTypeWarning, text);
  }

  void error(const char* text) {
    log(LogTypeError, text);
  }
};

#endif