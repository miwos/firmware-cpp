#ifndef SLIPSerial_h
#define SLIPSerial_h

#include <Arduino.h>
#include <Stream.h>
#include <usb_serial.h>

class SLIPSerial: public Stream {
private:
	enum erstate { CHAR, FIRSTEOT, SECONDEOT, SLIPESC } rstate;
  usb_serial_class *serial;
	
public:
	SLIPSerial(usb_serial_class&);
	operator bool() const { return *serial; }
	int available();
	int read();
  int readBytes( uint8_t *buffer, size_t size);
	int peek();
	void flush();
	void begin(unsigned long);
	void beginPacket();
	void endPacket();
	bool endOfPacket();
	// Overrides the Stream's write function to encode SLIP.
	size_t write(uint8_t b);
  size_t write(const uint8_t *buffer, size_t size);
};

#endif