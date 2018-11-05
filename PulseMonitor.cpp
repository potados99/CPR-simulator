#include "PulseMonitor.h"

PulseMonitor::PulseMonitor() {
  memset(this->times, 0, sizeof(times));

}

void PulseMonitor::attachSensor(Sensor *sens) {
  this->pushSensor = sens;
}

void PulseMonitor::attachDisplay(Display *disp) {
  this->BPMDisp = disp;
}

uint16_t PulseMonitor::getPressure() {
  return this->currentPressure;
}

void PulseMonitor::loop() {
  if (! this->pushSensor || ! this->BPMDisp) return;

  detectPush();
  showBPM();
}

void PulseMonitor::detectPush() {
  static bool beingPushed = false;
  static uint8_t tindex = 0;

  uint16_t pressure = this->currentPressure = this->pushSensor->read();
  bool justPushed = (pressure > this->threshold) && (! beingPushed);
  bool released = (pressure == 0);

  if (justPushed) {
    this->times[tindex] = millis();
    tindex = (tindex + 1) % TBUF_SIZE;
    beingPushed = true;
  }
  else if (released) {
    beingPushed = false;
  }
  else {
    // releasing
  }
}

void PulseMonitor::showBPM() {
  static uint32_t lastBPMCalc = 0;
  uint32_t now = millis();

  if (now - lastBPMCalc < refreshCycle) return;

  uint16_t count = 0;
  for (uint8_t i = 0; i < TBUF_SIZE; ++ i) {
    if (now - times[i] < dataLife) ++ count;
  }

  uint16_t BPM = (uint16_t)((60000L / dataLife) * count);
  this->BPMDisp->show(BPM, -1);

  lastBPMCalc = millis();
}
