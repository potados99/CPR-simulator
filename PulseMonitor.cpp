#include "PulseMonitor.h"

PulseMonitor::PulseMonitor() {
  memset(this->times, 0, sizeof(times));
  this->ready = false;
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
  // keep only when the sensor and display is not null, and enabled
  if (! this->pushSensor || ! this->BPMDisp || ! this->enable) { return; }

  // detect push and if then, add the time to buffer
  detectPush();

  bool initiated = this->times[0];
  // when the first push is detected, times[0] must be true
  if (initiated) {
    // when it was not ready before
    if (! this->ready) {
      // then it is the first time push
      this->startTime = millis();
      this->ready = true;
    }
  }
  else {
    // if not initiated yet
    this->ready = false;
  }

  if (this->ready) { showBPM(); }
}

void PulseMonitor::detectPush() {
  static bool beingPushed = false;
  static uint8_t tindex = 0;

  uint16_t pressure = this->currentPressure = this->pushSensor->read();
  bool justPushed = (pressure > this->threshold) && (! beingPushed); /* was not pushed and pushed just now */
  bool released = (pressure <= this->threshold / 10); /*  */

  // only when just pushed
  if (justPushed) {
    this->times[tindex] = millis();
    tindex = (tindex + 1) % TBUF_SIZE;
    beingPushed = true;
  }
  // after the push, completely released
  else if (released) {
    beingPushed = false; /* push is over */
  }
  else {
    // being released
  }
}

void PulseMonitor::showBPM() {
  static uint32_t lastBPMCalc = 0;
  uint32_t now = millis();

  // not yet to refresh
  if (now - lastBPMCalc < refreshCycle) { return; }

  uint16_t count = 0;
  for (uint8_t i = 0; i < TBUF_SIZE; ++ i) {
    if (times[i] && (now - times[i] < dataLife)) { ++ count; }
  }

  bool young = (millis() - this->startTime) < this->dataLife;
  uint16_t BPM = (uint16_t)((60000L / dataLife) * count);

  // when low bpm, display only if not young
  /*
   * low bpm and young -> not visible
   * low bpm and not young -> visible
   * not low bpm and young -> visible
   * not low bpm and not young -> visible
   *
   */
  if (BPM < 80) {
    pulseVisible = ! young;
  }
  else {
    pulseVisible = true;
  }

  if (this->pulseVisible) {
    this->BPMDisp->show(BPM, -1);
  }
  else {
    // show loading dots
    static uint8_t d = 3;

    this->pulseVisible = false;
    this->BPMDisp->singleDot(d);
    if (d == 0) { d = 3; }
    else { d --; }
  }

  lastBPMCalc = millis();
}
