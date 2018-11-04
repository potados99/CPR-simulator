#include "Display.h"
#include "Sensor.h"

#define TBUF_SIZE 16

Sensor mySensor(A7);
Display PressureDisplay(4, 2);
Display BPMDisplay(5, 3);

uint32_t times[TBUF_SIZE];
uint16_t BPM;

static const uint32_t refreshCycle = 1000;
static const uint32_t dataLife = 5000;


void pushed();
void calculateBPM();

void setup() {
  Serial.begin(9600);
  memset(times, 0, sizeof(times));

  BPM = 0;

  mySensor.setSensitivity(90);
  mySensor.setSmoothness(20);
  mySensor.setScale(100);
}

void loop() {
  static bool pushing = false;

  uint16_t readVal = mySensor.read(F_SENSITIVITY | F_SMOOTHNESS | F_SCALE);
  uint16_t threshold = 100;
  int8_t decpnt = -1;


  if (readVal > threshold) {
    if (! pushing) {
      pushed();
      pushing = true;
    }
  }
  else if (readVal == 0) {
    pushing = false;
  }


  PressureDisplay.show(readVal, decpnt);

  calculateBPM();

}

void pushed() {
  static uint8_t tindex = 0;

  times[tindex] = millis();
  tindex = (tindex + 1) % TBUF_SIZE;
}

void calculateBPM() {
  static uint32_t lastBPMCalc = 0;

  uint32_t now = millis();

  if (now - lastBPMCalc > refreshCycle) {
    uint16_t count = 0;


    for (uint8_t i = 0; i < TBUF_SIZE; ++ i) {
      if (now - times[i] < dataLife) {
        ++ count;
      }
    }

    BPM = (uint16_t)((60000L / dataLife) * count);

    BPMDisplay.show(BPM, -1);

    lastBPMCalc = millis();
  }
}
