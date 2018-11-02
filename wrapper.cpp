#include "Display.h"

Sensor mySensor(A7);
Display myDisp(3, 2);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }

  mySensor.setSensitivity(95);
  mySensor.setSmoothness(20);
  mySenosr.setScale(0.016L);
}

void loop() {
  uint16_t readVal = mySensor.read(F_SENSITIVITY | F_SMOOTHNESS | F_SCALE);
  myDisp.show((double)readVal, 0);

  Serial.println(readVal);
}
