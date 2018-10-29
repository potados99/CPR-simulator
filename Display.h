#ifndef _Display_h
#define _Display_h

#include "Arduino.h"
#include <SoftwareSerial.h>

#define DIGITS 4
#define CLEARABLE_DIGIT 3

class Display {
private:
  SoftwareSerial serial;
  uint8_t digit[DIGITS];
  void refresh(int pointDigit);
  void clearDigit(uint8_t d);
  void writeDigit(uint8_t d, uint8_t value);
  uint8_t truncate(uint8_t origin, uint8_t length);

public:
  Display(int rx, int tx);
  void show(double val, int pointDigit);
};

#endif
