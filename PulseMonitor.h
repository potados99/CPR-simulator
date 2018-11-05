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

  uint16_t  threshold = 30;
  uint32_t  refreshCycle = 1000;
  uint32_t  dataLife = 10000; // 10 sec

  uint16_t  currentPressure = 0;
  uint32_t  times[TBUF_SIZE];

  uint32_t  startTime = 0;
  bool      enable = false;
  bool      ready = false;
  bool      pulseVisible = false;

  void      detectPush();
  void      showBPM();

public:
  PulseMonitor();

  void      attachSensor(Sensor *sens);
  void      attachDisplay(Display *disp);

  void      setThreshold(uint16_t threshold);
  void      setRefreshCycle(uint32_t cycle);

  uint16_t  getPressure();

  void      start();

  void      loop();
};

#endif
