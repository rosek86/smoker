#ifndef INTERVAL_H__
#define INTERVAL_H__

#include <Arduino.h>

#include <stdint.h>
#include <stdbool.h>

class Interval {
public:
  Interval() { started = false; }

  void setInterval(uint32_t ms) {
    interval = ms;
  }

  void start() {
    started = true;
    cycleStartTime = millis();
  }

  bool hasElapsed() {
    if (started == false) { return false; }

    uint32_t timestamp = millis();

    if ((timestamp - cycleStartTime) >= interval) {
      cycleStartTime = timestamp;
      return true;
    }

    return false;
  }

private:
  bool started;

  uint32_t interval;
  uint32_t cycleStartTime;
};

#endif // INTERVAL_H__
