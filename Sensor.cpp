#include "Sensor.h"

Sensor::Sensor(int pin) {
  pinMode(this->pin = pin, INPUT);
}

void Sensor::setSensitivity(uint8_t sens) {
  this->sensitivity = sens;
}

void Sensor::setSmoothness(uint8_t smooth) {
  this->smoothness = smooth;
}

uint16_t Sensor::read(uint8_t options) {
  uint16_t value = analogRead();

  if (HAS(options, F_SENSITIVITY))    filter_sensitivity(value);
  if (HAS(options, F_SMOOTHNESS))     filter_smoothness(value);
  if (HAS(options, F_SCALE))          filter_scale(value);

  return value;
}


void Sensor::filter_sensitivity(uint16_t& val) {
  static uint16_t lastVal = 0;

  uint16_t threshold = (100 - this->sensitivity) * 5;

  if (abs(lastVal - val) >= threshold) {
    lastVal = val;
  }
  else {
    val = lastVal;
  }
}

void Sensor::filter_smoothness(uint16_t& val) {
  static const uint8_t  numReadings = 50;

  static uint16_t       readings[numReadings];      // the readings from the analog input
  static uint8_t        readIndex = 0;              // the index of the current reading
  static uint32_t       total = 0;                  // the running total
  static uint16_t       average = 0;                // the average

  total = total - readings[readIndex];
  readings[readIndex] = val;
  total = total + readings[readIndex];

  readIndex = (readIndex + 1) % numReadings;

  average = total / numReadings;
  val = average;
} 

void Sensor::filter_scale(uint16_t& val) {
  val = val * this->scale;
}
