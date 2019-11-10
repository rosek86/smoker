#ifndef CONTROLLER_H__
#define CONTROLLER_H__

#include <PID_v1.h>
#include "ControllerInfo.h"

#include "../utils/Timeout.h"

#define SECS_TO_MSECS(s) ((s) * 1000)

#define EPSILON 0.0001
#define FLOAT_EQUAL(a, b) (fabs((a) - (b)) <= EPSILON)
#define FLOAT_NOT_EQUAL(a, b) (!FLOAT_EQUAL(a, b))

struct ControllerStep {
  unsigned long time_ms;
  float temperature;
  bool smoke;
};

typedef void (*ControllerRelay)(bool state);

class Controller {
public:
  Controller(ControllerRelay smokeRelay, ControllerRelay heaterRelay):
      smokeRelay(smokeRelay), heaterRelay(heaterRelay),
      pid(&temp, &dutyCycle, &tempSetpoint, Kp, Ki, Kd, DIRECT) {
    stepsCount = 0;
    started = false;

    windowSize = 10;

    pid.SetSampleTime(SECS_TO_MSECS(windowSize));
    pid.SetOutputLimits(0, windowSize);
    pid.SetMode(AUTOMATIC);
  }

  void start() {
    started = true;
    startTime = millis();
    totalTime = calculateTotalTime();

    setupStep(0);
  }

  void run() {
    if (started == false || step >= stepsCount) {
      return;
    }

    processRelay();

    if (stepTimeout.hasElapsed() == true) {
      setupStep(step + 1);
    }
  }

  void stop() {
    started = false;

    // Turn off relays
    smokeRelay(false);
    heaterRelay(false);
  }

  void setTemperature(float t) {
    temp = t;
  }

  bool addStep(ControllerStep step) {
    if (stepsCount >= MAX_STEPS) {
      return false;
    }

    steps[stepsCount++] = step;
    return true;
  }

  void clearSteps() {
    stepsCount = 0;
  }

  bool getProgressInfo(ControllerInfo &info) {
    if (step >= stepsCount || stepsCount == 0) { return false; }

    if (started == true) {
      fillProgressInfo(info);
    } else {
      fillCompletedProgressInfo(info);
    }
    return true;
  }

  bool isRunning() {
    return started;
  }

private:
  static constexpr int MAX_STEPS = 10;

  ControllerRelay smokeRelay;
  ControllerRelay heaterRelay;

  uint8_t stepsCount = 0;
  ControllerStep steps[MAX_STEPS];

  bool started;
  uint32_t startTime;
  uint32_t totalTime;

  uint8_t step;

  // params must be before PID
  double Kp = 2, Ki = 5, Kd = 1;
  PID pid;

  double temp, tempSetpoint;
  double dutyCycle;
  double windowSize;
  Timeout timeout;
  Timeout stepTimeout;

  void setupStep(uint8_t s) {
    if (s >= stepsCount) {
      // Completed
      stop();
      Serial.println("C");
      return;
    }

    step = s;

    // Setup smoke relay
    if (steps[step].smoke == true) {
      smokeRelay(true);
    } else {
      smokeRelay(false);
    }

    // Setup heater relay
    heaterRelay(false);
    tempSetpoint = steps[step].temperature;

    stepTimeout.setTimeout(steps[step].time_ms);
    stepTimeout.start();

    Serial.print("S");
    Serial.print(step);
    Serial.print(",");
    Serial.print(steps[step].smoke);
    Serial.print(",");
    Serial.print(steps[step].temperature);
    Serial.print(",");
    Serial.println(steps[step].time_ms);
  }

  void processRelay() {
    if (pid.Compute() == true) {
      driveHeaterRelay();
      Serial.print("ON");
      Serial.print(dutyCycle);
      Serial.print(" ");
      Serial.println(temp);
    }
  
    if (timeout.hasElapsed() == true) {
      heaterRelay(false);
      Serial.println("OFF");
    }  
  }

  void driveHeaterRelay() {
    if (shouldTurnRelayOn()) {
      heaterRelay(true);

      if (shouldEnableTimeout()) {
        timeout.setTimeout(round(SECS_TO_MSECS(dutyCycle)));
        timeout.start();
      }
    } else {
      heaterRelay(false);
    }
  }

  inline bool shouldTurnRelayOn() {
    return FLOAT_EQUAL(dutyCycle, 0) ? false : true;
  }

  inline bool shouldEnableTimeout() {
    return FLOAT_EQUAL(dutyCycle, windowSize) ? false : true;
  }

  void fillProgressInfo(ControllerInfo &info) {
    info.step = {
      .id               = step,
      .time             = steps[step].time_ms,
      .temperature      = steps[step].temperature,
      .smokeOn          = steps[step].smoke,
      .progress         = stepTimeout.progress()
    };
    info.temperature    = temp;
    info.totalProgress  = calculateTotalProgress();
    info.totalSteps     = stepsCount;
  }

  void fillCompletedProgressInfo(ControllerInfo &info) {
    uint8_t lastStep = stepsCount - 1;
    info.step = {
      .id               = lastStep,
      .time             = steps[lastStep].time_ms,
      .temperature      = steps[lastStep].temperature,
      .smokeOn          = steps[lastStep].smoke,
      .progress         = 100.0F
    };
    info.temperature    = temp;
    info.totalProgress  = 100.0F;
    info.totalSteps     = stepsCount;
  }

  uint32_t calculateTotalTime() {
    uint32_t t = 0;
    for (uint8_t s = 0; s < stepsCount; s++) {
      t += steps[s].time_ms;
    }
    return t;
  }

  float calculateTotalProgress() {
    return (float)(millis() - startTime) / totalTime * 100.0F;
  }
};

#endif // CONTROLLER_H__
