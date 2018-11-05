#ifndef _PULSEMONITOR_H
#define _PULSEMONITOR_H

#include "Arduino.h"
#include "Sensor.h"
#include "Display.h"

#define TBUF_SIZE 64

class PulseMonitor {
private:
  Sensor    *pushSensor = nullptr;
  Display   *BPMDisp = nullptr;

  uint16_t  threshold = 100;
  uint32_t  refreshCycle = 1000;
  uint32_t  dataLife = 10000;

  uint16_t  currentPressure;
  uint32_t  times[TBUF_SIZE];

  void      detectPush();
  void      showBPM();

public:
  PulseMonitor();

  void      attachSensor(Sensor *sens);
  void      attachDisplay(Display *disp);
  uint16_t  getPressure();

  void      loop();
};

#endif
