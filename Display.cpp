#include "Display.h"

Display::Display(int rx, int tx): serial(rx, tx) {
  setBuffer(0);

  this->serial.begin(9600);
  refresh(-1);
}

void Display::show(uint8_t val) {
  this->serial.write(val);
}

void Display::show(double val, int pointDigit) {
  if ((val > pow(10, 4 - pointDigit) - 1) || (val < 0L)) { // over representation limit.
    displayError(0x0F);
    return;
  }

  int16_t integerPart = (int)val;
  uint16_t decimalPart = (int)((val - integerPart) * pow(10, pointDigit));

  this->validInts = _getDigitsOfDecimal(integerPart);

  // OK Clear 2018.11.2
  uint8_t decLength = (pointDigit < 0) ? 0 : pointDigit;
  writeToBuffer(decimalPart, 0, decLength);
  writeToBuffer(integerPart, decLength, 4 - decLength);

  refresh(pointDigit);
}

void Display::singleDot(uint8_t digit) {
  for (uint8_t i = 0; i < DIGITS; ++ i) {
    clearDigit(i);
  }
  writeDigit(digit + 4, 4);  // point
}

void Display::singleZero(uint8_t digit) {
  for (uint8_t i = 0; i < DIGITS; ++ i) {
    clearDigit(i);
  }
  writeDigit(digit, 0);  // point
}

void Display::refresh(int pointDigit) {
  for (uint8_t i = 0; i < DIGITS; ++ i) {
    if (i > 0 && i - (pointDigit < 0 ? 0 : pointDigit) + 1 > validInts) clearDigit(i);
    else writeDigit(i, digit[i]);
  }

  if (pointDigit < 0 || pointDigit > 3) return;
  writeDigit(pointDigit + 4, 4);  // point
}

bool Display::clearDigit(uint8_t d) {
  if (d > 3) {
    displayError(0x0C);
    return false;
  }

  return writeDigit(d + 4, 0);
}

bool Display::writeDigit(uint8_t d, uint8_t val) {
  if (d > 7) {
    displayError(0x0D);
    return false;
  }
  this->serial.write((d << 4) + val);
  return true;
}

void Display::displayError(uint8_t errorCode) {
  for (uint8_t i = 0; i < DIGITS; ++ i) digit[i] = errorCode;
  refresh(-1); // no decimal places.
}

void Display::setBuffer(uint8_t val) {
  memset(digit, val, DIGITS);
}

bool Display::writeToBuffer(uint16_t val, uint8_t startIndex, uint8_t length) {
  if (length > DIGITS)                    return false;
  if (startIndex + length > DIGITS)       return false;

  // cut(divide) number to fit into length
  uint16_t truncated = _truncate(val, length);

  // in range zero to length, add each digit to buffer, from left to right.
  // if number is small(digits are less than length), 0 is padded left.
  for (uint8_t i = 0; i < length; ++ i) {
    digit[startIndex + i] = truncated ? truncated % 10 : 0;
    truncated /= 10;
  }

  return true;
}

uint8_t Display::_getDigitsOfDecimal(uint16_t num) {
  uint8_t digits = 0;
  while (num > pow(10, ++ digits));

  return digits;
}

uint16_t Display::_truncate(uint16_t origin, uint8_t length) {
	if (origin < pow(10, length)) return origin; // length over
	while ((origin /= 10) >= pow(10, length));
	return origin;
}
