#ifndef _Sensor_h
#define _Sensor_h

#include "Arduino.h"

#define HAS(X, Y) ((X) & (Y))

typedef enum _FILTER_OPTIONS {
  F_SENSITIVITY     = 0x01,
  F_SMOOTHNESS      = 0x02,
  F_SCALE           = 0x04
} FILTER_OPTIONS;

/**
 * 압력센서를 다루는 클래스입니다.
 * 센서의 민감도, 부드러움, 스케일을 조절할 수 있습니다.
 */
class Sensor {
private:
  uint8_t   pin;
  uint8_t   sensitivity = 50; // 1 to 100
  uint8_t   smoothness = 50; // 1 to 100
  uint8_t   scale = 2; //1 to 100, percent
  uint8_t   options = 0;

  void      filter_sensitivity(uint16_t& val);
  void      filter_smoothness(uint16_t& val);
  void      filter_scale(uint16_t& val);

public:
  Sensor(int pin);

  /**
   * 센서의 민감도를 조절합니다.
   * 민감도가 높으면 더 작은 변화도 감지할 수 있습니다.
   * @param sens      민감도. 0부터 100까지.
   */
  void      setSensitivity(uint8_t sens);

  /**
   * 센서의 부드러움을 조절합니다.
   * 값들의 평균을 이용해 데이터의 변화를 완만하게 만들어 줍니다,
   * 부드러울수록 센서값의 변화가 선형적입니다.
   * @param smooth    부드러움. 0부터 100까지.
   */
  void      setSmoothness(uint8_t smooth);

  /**
   * 센서값의 규모를 조절합니다.
   * @param scale     규모. 0부터 65535까지. 오버플로우 주의.
   */
  void      setScale(uint8_t scale);

  /**
   * 원시 센서값에 적용할 필터를 선택합니다.
   * @param options   옵션. 8비트로 최대 8개의 옵션 적용 가능.
   */
  void      setOptions(uint8_t options);

  /**
   * 옵션이 모두 적용된 센서값을 읽어 반환합니다.
   * @return          처리된 센서값.
   */
  uint16_t  read();
};

#endif
