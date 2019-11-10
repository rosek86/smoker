#ifndef CONTROLLER_INFO_H__
#define CONTROLLER_INFO_H__

#include <stdint.h>

struct ControllerInfo {
  struct {
    uint8_t  id;
    uint32_t time;
    float    temperature;
    bool     smokeOn;
    float    progress;
  } step;

  float temperature;
  float totalProgress;

  uint8_t totalSteps;
};

#endif // CONTROLLER_INFO_H__
