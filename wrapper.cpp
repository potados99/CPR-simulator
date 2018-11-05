#include "Display.h"
#include "Sensor.h"
#include "PulseMonitor.h"

Display presDisp(4, 2);

Sensor _pushSensor(A7);
Display _BPMDisp(5, 3);
PulseMonitor monitor;

void setup() {
  _pushSensor.setSensitivity(90);
  _pushSensor.setSmoothness(20);
  _pushSensor.setScale(100);
  _pushSensor.setOptions(F_SENSITIVITY | F_SMOOTHNESS | F_SCALE);

  monitor.attachSensor(&_pushSensor);
  monitor.attachDisplay(&_BPMDisp);

  monitor.setThreshold(30);
  monitor.setRefreshCycle(500);

  monitor.start();
}

void loop() {
  monitor.loop();
  presDisp.show(monitor.getPressure(), -1);
}
