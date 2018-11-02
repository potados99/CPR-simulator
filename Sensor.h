#ifndef _Sensor_h
#define _Sensor_h

#include "Arduino.h"

#define HAS(X, Y) ((X) & (Y))

typedef enum FILTER_OPTIONS {
  F_SENSITIVITY     = 0x01,
  F_SMOOTHNESS      = 0x02,
  F_SCALE           = 0x04
};

class Sensor {
private:
  uint8_t   pin;
  uint8_t   sensitivity = 50; // 1 to 100
  uint8_t   smoothness = 50; // 1 to 100
  double    scale = 0.016L;

  void      filter_sensitivity(uint16_t& val);
  void      filter_smoothness(uint16_t& val);
  void      filter_scale(uint16_t& val);

public:
  Sensor(int pin);

  void      setSensitivity(uint8_t sens);
  void      setSmoothness(uint8_t smooth);
  double    setScale(double scale);

  uint16_t  read(uint8_t options = 0);
};

#endif
