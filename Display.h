#ifndef _Display_h
#define _Display_h

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <stdlib.h>

#define DIGITS 4
#define CLEARABLE_DIGIT 3

class Display {
private:
  SoftwareSerial serial;
  uint8_t digit[DIGITS];
  uint8_t validInts;

  void refresh(int pointDigit);
  bool clearDigit(uint8_t d);
  bool writeDigit(uint8_t d, uint8_t value);

  void displayError(uint8_t errorCode);

  void setBuffer(uint8_t val);
  bool writeToBuffer(uint16_t val, uint8_t startIndex, uint8_t length);

  uint8_t _getDigitsOfDecimal(uint16_t num);
  uint16_t _truncate(uint16_t origin, uint8_t length);

public:
  Display(int rx, int tx);

  void show(uint8_t val);                 // parent mode
  void show(double val, int pointDigit);  // child mode

  void singleDot(uint8_t digit);
  void singleZero(uint8_t digit);
};

#endif
