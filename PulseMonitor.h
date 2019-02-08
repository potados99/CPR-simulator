#ifndef _PULSEMONITOR_H
#define _PULSEMONITOR_H

#include "Arduino.h"
#include "Sensor.h"
#include "Display.h"

#define TBUF_SIZE 64

/**
 * 센서로 읽은 값을 처리해 디스플레이로 보여주는 심장 압박 모니터 클래스입니다.
 * Sensor 클래스와 Display 클래스를 사용합니다.
 */
class PulseMonitor {
private:
  Sensor    *pushSensor = nullptr;
  Display   *BPMDisp = nullptr;

  uint16_t  threshold = 30;
  uint32_t  refreshCycle = 1000;
  uint32_t  dataLife = 10000; // 10 sec

  uint16_t  currentPressure = 0;
  uint32_t  times[TBUF_SIZE];

  uint32_t  startTime = 0;
  bool      enable = false;
  bool      ready = false;
  bool      pulseVisible = false;

  /**
   * 현재의 센서값과 맥락을 이용해 사용자가 압박을 가했는지 판단합니다.
   * 압박이라 판단되면 그 시각을 저장합니다.
   */
  void      detectPush();

  /* 압박 빈도를 표시합니다.*/
  void      showBPM();

public:
  PulseMonitor();

  /**
   * 센서를 추가합니다.
   * @param sens    추가할 Sensor 객체.
   */
  void      attachSensor(Sensor *sens);

  /**
   * 디스플레이를 추가합니다.
   * @param disp    추가할 Display 객체.
   */
  void      attachDisplay(Display *disp);

  /**
   * 1회 압박으로 간주할 최소 압력을 설정합니다.
   * @param threshold   최소 압력.
   */
  void      setThreshold(uint16_t threshold);

  /**
   * 압박 빈도를 표시하는 디스플레이를 새로고침할 주기를 설정합니다.
   * @param cycle       주기. 밀리초 단위.
   */
  void      setRefreshCycle(uint32_t cycle);

  /**
   * 현재 센서에 가해지는 압력을 구합니다.
   * @return            센서값.
   */
  uint16_t  getPressure();

  /* 모니터를 시작합니다. */
  void      start();

  /* 호출될 때 마다 작업을 수행합니다. loop()에서 반드시 호출해주어야 합니다. */
  void      loop();
};

#endif
