#ifndef RangeEncoder_h
#define RangeEncoder_h

#include <Encoder.h>

class RangeEncoder {
private:
  Encoder *encoder;
  int32_t min = 0;
  int32_t max = 127;
  int32_t value = 0;

public:
  RangeEncoder(uint8_t pin1, uint8_t pin2, int32_t min, int32_t max) {
    encoder = new Encoder(pin1, pin2);
    this->min = min;
    this->max = max;
  }

  int32_t read() {
    int32_t newValue = encoder->read();

    if (newValue < min) {
      newValue = 0;
      encoder->write(min - 1);
    } else if (newValue > max) {
      newValue = max;
      encoder->write(max + 1);
    }

    value = newValue;
    return value;
  }

  int32_t read(bool &changed) {
    int32_t oldValue = value;
    int32_t value = read();
    changed = value != oldValue;
    return value;
  }

  void write(int32_t value) {
    encoder->write(value);
  }

  ~RangeEncoder() {
    delete encoder;
  }
};

#endif