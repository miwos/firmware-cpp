#include <NativeEthernet.h>
#include <ArduinoWebsockets.h>
#include <TeensyID.h>
#include <OSCMessage.h>

using namespace websockets;
WebsocketsServer server;

byte mac[6];
const uint16_t port = 80;

void test(OSCMessage &msg) {
  Serial.println("Received OSC message");

  OSCData oscData = msg.getOSCData(1);
  String str(oscData.data.s);
  
  Serial.println(str);
}

void setup() {
  teensyMAC(mac);

  Serial.begin(9600);
  while (!Serial) {}

  Ethernet.begin(mac);
  server.listen(port);
  Serial.print("Server available at ws://");
  Serial.print(Ethernet.localIP());
  if (port != 80) Serial.printf(":%d", port);
  Serial.println();
}

class PrintToString : public Print {
public:
  char data[100];
  int pos = 0;

  size_t write(uint8_t c) {
    data[pos++] = c;
    return 1;
  }
  
  size_t write(const uint8_t* buffer, size_t size) {
    size_t n = 0;

    while (size--) {
      write(*buffer++);
    }

    return n;
  }
};

PrintToString printToString;

void loop() {
  WebsocketsClient client = server.accept();
  
  if (client.available()) {
    Serial.println("Client connected");

    // Read message from client and log it.
    WebsocketsMessage websocketsMsg = client.readBlocking();

    OSCMessage oscMsg;
    oscMsg.fill(
      reinterpret_cast<uint8_t*>(const_cast<char*>(websocketsMsg.c_str())),
      websocketsMsg.length()
    );
    oscMsg.dispatch("/plugins/update-code", test);

    OSCMessage oscMsgSend("/plugins/update-code/echo");
    oscMsgSend.add("test").add("hallo!!!");
    oscMsgSend.send(printToString);

    client.send((const char *) printToString.data);

    // Close the connection.
    client.close();
    Serial.println("Client closed");
  }
}