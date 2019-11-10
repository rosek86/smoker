#ifndef TIMEOUT_H__
#define TIMEOUT_H__

#include <Arduino.h>

#include <stdint.h>
#include <stdbool.h>

class Timeout {
public:
  Timeout() { started = false; }

  void setTimeout(uint32_t ms) {
    timeout = ms;
  }

  void start() {
    startTime = millis();
    started = true;
  }

  bool hasElapsed() {
    if (started == false) { return false; }

    if ((millis() - startTime) >= timeout) {
      started = false;
      return true;
    }

    return false;
  }

  float progress() {
    if (started == false) { return 0.0F; }

    float p = (float)(millis() - startTime) / timeout * 100.0F;

    if (p > 100.0F) { return 100.0F; }
    if (p <   0.0F) { return   0.0F; }
    return p;
  }

private:
  bool started;

  uint32_t timeout;
  uint32_t startTime;
};

#endif // TIMEOUT_H__
