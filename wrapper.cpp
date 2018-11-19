#include "Display.h"
#include "Sensor.h"
#include "PulseMonitor.h"

Display presDisp(4, 2); /* used to display current pressure */

Sensor _pushSensor(A7);
Display _BPMDisp(5, 3);
PulseMonitor monitor; /* used to calculate and display current BPM */

void setup() {
  _pushSensor.setSensitivity(90);
  _pushSensor.setSmoothness(20);
  _pushSensor.setScale(100);
  _pushSensor.setOptions(F_SENSITIVITY | F_SMOOTHNESS | F_SCALE);

  monitor.attachSensor(&_pushSensor);
  monitor.attachDisplay(&_BPMDisp);

  monitor.setThreshold(70);
  monitor.setRefreshCycle(1000);

  monitor.start();
}

void loop() {
  monitor.loop();
  presDisp.show(monitor.getPressure() / 10, -1); /* -1: no decimal point */
}
