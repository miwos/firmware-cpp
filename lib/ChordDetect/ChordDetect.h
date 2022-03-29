#ifndef ChordDetect_h
#define ChordDetect_h

#include <Arduino.h>

/**
 * based on
 * https://github.com/SDooman/polyphonic_tuner/blob/master/auduino/NoteProcess.ino
 */

class ChordDetect {
private:
  static const byte maxTypes = 40;

  // Bit masks represent a collection of notes, where the LSB represents a C and
  // the MSB represents a B.
  // 0  0  0  0  0  0  0  0  0  0  0  0
  // B  A# A  G# G  F# F  E  D# D  C# C

  const uint16_t maj = 0b000010010001;
  const uint16_t min = 0b000010001001;
  const uint16_t dim = 0b000001001001;
  const uint16_t aug = 0b000100010001;
  const uint16_t min7 = 0b010010001001;
  const uint16_t halfDim7 = 0b010001001001;
  const uint16_t dim7 = 0b001001001001;
  const uint16_t minMaj7 = 0b100010001001;
  const uint16_t majMin7 = 0b010010010001;
  const uint16_t maj7 = 0b100010010001;

  const uint16_t addB9 = 0b000000000010;
  const uint16_t add9 = 0b000000000100;
  const uint16_t add4 = 0b000000100000;

  const uint16_t types[maxTypes] = {
      // basic
      maj,
      min,
      dim,
      aug,
      min7,
      halfDim7,
      dim7,
      minMaj7,
      majMin7,
      maj7,

      // add b9
      (uint16_t)(maj | addB9),
      (uint16_t)(min | addB9),
      (uint16_t)(dim | addB9),
      (uint16_t)(aug | addB9),
      (uint16_t)(min7 | addB9),
      (uint16_t)(halfDim7 | addB9),
      (uint16_t)(dim7 | addB9),
      (uint16_t)(minMaj7 | addB9),
      (uint16_t)(majMin7 | addB9),
      (uint16_t)(maj7 | addB9),

      // add 9
      (uint16_t)(maj | add9),
      (uint16_t)(min | add9),
      (uint16_t)(dim | add9),
      (uint16_t)(aug | add9),
      (uint16_t)(min7 | add9),
      (uint16_t)(halfDim7 | add9),
      (uint16_t)(dim7 | add9),
      (uint16_t)(minMaj7 | add9),
      (uint16_t)(majMin7 | add9),
      (uint16_t)(maj7 | add9),

      // add 4
      (uint16_t)(maj | add4),
      (uint16_t)(min | add4),
      (uint16_t)(dim | add4),
      (uint16_t)(aug | add4),
      (uint16_t)(min7 | add4),
      0, // empty
      0, // empty
      (uint16_t)(minMaj7 | add4),
      (uint16_t)(majMin7 | add4),
      (uint16_t)(maj7 | add4),
  };

  bool checkFlag(byte number, byte flag) { return (number & flag) == flag; }

public:
  bool analyze(
      uint16_t chord, byte &resultRoot, byte &resultQuality, byte &resultAdd) {
    for (byte root = 0; root < 12; root++) {
      for (byte type = 0; type < maxTypes; type++) {
        if (chord == types[type]) {
          resultRoot = root;
          resultQuality = type % 10;
          if (type >= 10) {
            resultAdd = 8;
          } else if (type >= 20) {
            resultAdd = 9;
          } else if (type >= 30) {
            resultAdd = 4;
          } else {
            resultAdd = 0;
          }
          return true;
        }
      }
      // If the first bit is one, we rotate it, otherwise we just shift.
      chord = 1 & chord ? (chord >> 1) | 2048 : (chord >> 1);
    }
    return false;
  }
};

#endif