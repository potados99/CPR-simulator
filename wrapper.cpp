#include "Display.h"
#include "Sensor.h"
#include "PulseMonitor.h"

Display presDisp(4, 2); /* 현재 압력을 표시하기 위해 사용. */

Sensor _pushSensor(A7); /* monitor에 연결할 센서. */
Display _BPMDisp(5, 3); /* monitor에 연결할 디스플레이. */
PulseMonitor monitor; /* 현재 압박 빈도를 계산하고 표시하기 위해 사용. */

void setup() {
  // 센서를 설정합니다.
  _pushSensor.setSensitivity(90);
  _pushSensor.setSmoothness(20);
  _pushSensor.setScale(100);
  _pushSensor.setOptions(F_SENSITIVITY | F_SMOOTHNESS | F_SCALE);

  // monitor에 센서와 디스플레이를 연결합니다.
  monitor.attachSensor(&_pushSensor);
  monitor.attachDisplay(&_BPMDisp);

  // monitor를 설정합니다.
  monitor.setThreshold(70);
  monitor.setRefreshCycle(1000);

  // monitor를 시작합니다.
  monitor.start();
}

void loop() {
  monitor.loop();

  // 현재 압력을 표시합니다.
  presDisp.show(monitor.getPressure() / 10, -1); /* -1: 소수점 없음. */
}
