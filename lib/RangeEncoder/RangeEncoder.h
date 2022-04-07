#ifndef RangeEncoder_h
#define RangeEncoder_h

#include <Encoder.h>

class RangeEncoder {
private:
  Encoder *encoder;
  int32_t min = 0;
  int32_t max = 127;
  int32_t value = 0;

  int32_t constrainValue(int32_t v) {
    if (v < min) {
      v = 0;
      encoder->write(min - 1);
    } else if (v > max) {
      v = max;
      encoder->write(max + 1);
    }
    return v;
  }

public:
  RangeEncoder(uint8_t pin1, uint8_t pin2, int32_t min, int32_t max) {
    encoder = new Encoder(pin1, pin2);
    this->min = min;
    this->max = max;
  }

  void setRange(int32_t min, int32_t max) {
    this->min = min;
    this->max = max;
    value = constrainValue(value);
  }

  int32_t read() {
    value = constrainValue(encoder->read());
    return value;
  }

  int32_t read(bool &changed) {
    int32_t oldValue = value;
    int32_t value = read();
    changed = value != oldValue;
    return value;
  }

  void write(int32_t value) { encoder->write(value); }

  ~RangeEncoder() { delete encoder; }
};

#endif