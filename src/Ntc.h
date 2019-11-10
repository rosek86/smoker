#ifndef NTC_H__
#define NTC_H__

#include <math.h>

class Ntc {
public:
  static float convertDegC(float resistance) {
    float steinhart;
    steinhart  = resistance / thermistorNominal;      // (R/Ro)
    steinhart  = log(steinhart);                      // ln(R/Ro)
    steinhart /= bCoefficient;                        // 1/B * ln(R/Ro)
    steinhart += 1.0 / (temperatureNominal + 273.15); // + (1/To)
    steinhart  = 1.0 / steinhart;                     // Invert
    steinhart -= 273.15;
    return steinhart;
  }

private:
  static constexpr float thermistorNominal = 10000.0F;
  static constexpr float temperatureNominal = 25.0F;
  static constexpr float bCoefficient = 3395;
};

#endif // NTC_H__
