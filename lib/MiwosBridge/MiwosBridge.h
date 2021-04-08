#ifndef MiwosBridge_h
#define MiwosBridge_h

#include <SLIPSerial.h>
#include <OSCMessage.h>

class MiwosBridge {
public:
  SLIPSerial* slipSerial;

  MiwosBridge(SLIPSerial *slipSerial);
  void sendMessage(OSCMessage &message);
  void sendRaw();
};

#endif