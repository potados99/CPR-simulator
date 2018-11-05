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

void PulseMonitor::setThreshold(uint16_t threshold) {
  this->threshold = threshold;
}

void PulseMonitor::setRefreshCycle(uint32_t cycle) {
  this->refreshCycle = cycle;
}

uint16_t PulseMonitor::getPressure() {
  return this->enable ? this->currentPressure : 0;
}

void PulseMonitor::start() {
  this->enable = true;
}

void PulseMonitor::loop() {
  if (! this->pushSensor || ! this->BPMDisp || ! this->enable) return;

  detectPush();

  bool firstPushed =  this->times[0];
  if (firstPushed) {
    if (! ready) this->startTime = millis();
    this->ready = true;
  }
  else {
    this->ready = false;
  }

  if (this->ready) showBPM();
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
    if (times[i] && (now - times[i] < dataLife)) ++ count;
  }

  bool young = (millis() - this->startTime) < this->dataLife;
  uint16_t BPM = (uint16_t)((60000L / dataLife) * count);

  if (BPM < 60) {
    pulseVisible = young ? false : true;
  }
  else pulseVisible = true;

  if (this->pulseVisible) this->BPMDisp->show(BPM, -1);
  else {
    static uint8_t d = 3;

    this->pulseVisible = false;
    this->BPMDisp->singleDot(d);
    if (d == 0) d = 3;
    else d --;
  }

  lastBPMCalc = millis();
}
