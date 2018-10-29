#include "Display.h"

Display::Display(int rx, int tx): serial(rx, tx) {
  for (uint8_t i = 0; i < DIGITS; ++ i) {
    digit[i] = 0;
  }

  serial.begin(19200);
  refresh(2);
}

void Display::show(double val, int pointDigit) {
  if (val > pow(10, 4 - pointDigit) - 1) { // over representation limit.
    for (uint8_t i = 0; i < DIGITS; ++ i) { 
      digit[i] = 0x0F;
    }
    refresh(-1);
    return;
  }
  
  if (val < 0L) { 
    for (uint8_t i = 0; i < DIGITS; ++ i) { 
      digit[i] = 0x0E;
    }
    refresh(-1);
    return;
  }
  
  int integerPart = (int)val;
  int decimalPart = (int)((val - integerPart) * pow(10, pointDigit));

  int dec1st = decimalPart / 10; 
  int dec10th = decimalPart % 10;

  digit[0] = dec10th;
  digit[1] = dec1st;

  int int1st;
  int int10th;
  
  if (integerPart >= 10) {
    int1st = integerPart % 10;
    int10th = integerPart / 10;
  }
  else {
    int1st = integerPart;
    int10th = 0;
  }

  digit[2] = int1st;
  digit[3] = int10th;  

  refresh(pointDigit);
}

void Display::refresh(int pointDigit) {
  for (uint8_t i = 0; i < DIGITS; ++ i) {
    if (i > 0 && i > pointDigit && digit[i] == 0) clearDigit(i);
    else writeDigit(i, digit[i]);
  }

  if (pointDigit < 0 && pointDigit > 3) return;
  serial.write(((pointDigit + 4) << 4) + 4); // point
}

void Display::clearDigit(uint8_t d) {
  if (d > 3) return;

  serial.write((d + 4) << 4);
}

void Display::writeDigit(uint8_t d, uint8_t val) {
  serial.write((d << 4) + val);
}

uint8_t Display::truncate(uint8_t origin, uint8_t length)
{
	if (origin < pow(10, length + 1)) return origin; // length over
	while ((origin /= 10) >= pow(10, length));
	return origin;
}
