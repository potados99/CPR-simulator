#ifndef _Sensor_h
#define _Sensor_h

#include "Arduino.h"

#define HAS(X, Y) ((X) & (Y))

typedef enum _FILTER_OPTIONS {
  F_SENSITIVITY     = 0x01,
  F_SMOOTHNESS      = 0x02,
  F_SCALE           = 0x04
} FILTER_OPTIONS;

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

  void      setSensitivity(uint8_t sens);
  void      setSmoothness(uint8_t smooth);
  void      setScale(uint8_t scale);
  void      setOptions(uint8_t options);

  uint16_t  read();
};

#endif
