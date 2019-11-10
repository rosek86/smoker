#include "Temperature.h"
#include "Ntc.h"

#include <Arduino.h>

static float measureVoltageA0(void);
static float getResistance(float vo);

float temperatureMeasure(float *resistance) {
  float vo = measureVoltageA0();
  float res = getResistance(vo);

  if (resistance != NULL) {
    *resistance = res;
  }

  return Ntc::convertDegC(res);
}

static float measureVoltageA0(void) {
  const float adcref = 3.1F;
  const float adcres = 1023.0F;
  return adcref * analogRead(A0) / adcres;
}

static float getResistance(float vo) {
  const float supref = 3.3F;
  const float resref = 10000.0F;
  return resref * vo / (supref - vo);
}
