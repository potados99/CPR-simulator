#include "Display.h"
#include "Sensor.h"

#define TBUF_SIZE 16

Sensor mySensor(A7);
Display PressureDisplay(4, 2);
Display BPMDisplay(5, 3);

uint32_t times[TBUF_SIZE];
uint16_t BPM;

static const uint32_t refreshCycle = 1000;
static const uint32_t dataLife = 10000;

void countPush(uint16_t readVal);
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
  uint16_t readVal = mySensor.read(F_SENSITIVITY | F_SMOOTHNESS | F_SCALE);
  int8_t decpnt = -1;

  PressureDisplay.show(readVal, decpnt);

  countPush(readVal);
  calculateBPM();
}

void countPush(uint16_t readVal) {
  static bool pushing = false;
  static uint8_t tindex = 0;

  static const uint16_t threshold = 100;

  bool justPushed = (readVal > threshold) && (! pushing);
  bool released = (readVal == 0);

  if (justPushed) {
      times[tindex] = millis();
      tindex = (tindex + 1) % TBUF_SIZE;
      pushing = true;
  }
  else if (released) {
    pushing = false;
  }
  else {
    // releasing
  }
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
