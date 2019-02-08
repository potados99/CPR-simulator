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

  /**
   * 특정 자리의 수를 새로 고칩니다.
   * @param pointDigit    고칠 자리(우측부터).
   */
  void refresh(int pointDigit);

  /**
   * 특정 자리의 수를 제거합니다.
   * @param d             제거할 자리(우측부터).
   * @return              인자가 유효하지 않으면 false.
   */
  bool clearDigit(uint8_t d);

  /**
   * 특정 자리의 수를 씁니다.
   * @param d             수를 쓸 자리(우측부터).
   * @param value         값
   * @return              인자가 유효하지 않으면 false.
   */
  bool writeDigit(uint8_t d, uint8_t value);

  /**
   * 에러를 표시합니다.
   * @param errorCode     에러 코드.
   */
  void displayError(uint8_t errorCode);

  /**
   * 버퍼의 모든 바이트에 기록합니다.
   * @param val           기록할 값.
   */
  void setBuffer(uint8_t val);

  /**
   * 버퍼에 씁니다.
   * @param val           버퍼에 쓸 10진수 숫자.
   * @param startIndex    기록을 시작할 인덱스
   * @param length        쓸 숫자의 길이.
   * @return              인자가 유효하지 않으면 false.
   */
  bool writeToBuffer(uint16_t val, uint8_t startIndex, uint8_t length);

  /**
   * 10진수 숫자가 몇 자리인지 구합니다.
   * @param num           몇 자리인지 궁금한 숫자.
   * @return              그 숫자의 길이.
   */
  uint8_t _getDigitsOfDecimal(uint16_t num);

  /**
   * 숫자를 자리수에 맞게 자릅니다.
   * LSB부터 자릅니다.
   * @param origin        자를 숫자.
   * @param length        목표 길이.
   * @return              잘린 숫자. 목표 숫자가 원본의 자리수보다 크면 원본을 반환.
   */
  uint16_t _truncate(uint16_t origin, uint8_t length);

public:
  Display(int rx, int tx);

  /**
   * 디스플레이에 값을 10진수로 표시합니다.
   * parent mode일 때 사용합니다.
   * @param val         표시할 값. 0부터 255까지.
   */
  void show(uint8_t val);

  /**
   * 디스플레이에 값을 10진수 소수로 표시합니다.
   * child mode일 때 사용합니다.
   * @param val         표시할 값.
   * @param pointDigit  소수점의 위치(우측부터).
   */
  void show(double val, int pointDigit);  // child mode

  /**
   * 점을 하나 표시합니다.
   * @param digit       점을 표시할 자리(우측부터).
   */
  void singleDot(uint8_t digit);

  /**
   * 0을 하나 표시합니다.
   * @param digit       0을 표시할 자리(우측부터).
   */
  void singleZero(uint8_t digit);
};

#endif
