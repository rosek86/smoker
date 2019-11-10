#ifndef EDIT_STEP_H__
#define EDIT_STEP_H__

#include "View.h"
#include "display/Display.h"

class EditStepView: public View {
public:
  EditStepView(Display &display): View(display) {
    option = 0;
    step = steps = 0;
    initEncoder = encoder = 0;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader("Krok " + String(step+1) + "/" + String(steps));

    uint32_t i = 0, o = option < 3 ? 0 : option - 2;

    if (o == 0) {
      display.drawButton(i, timeFormat(time_ms), option == o);
      o++, i++;
    }
    if (o == 1) {
      display.drawButton(i, temperatureFormat(temperature), option == o);
      o++, i++;
    }
    if (o == 2) {
      display.drawButton(i, smokeFormat(smoke), option == o);
      o++, i++;
    }
    if (i != 3) {
      display.drawButton(i, "Dalej", option == 3);
    }

    display.display();
  }

  void setInitEncoder(int e) {
    initEncoder = encoder = e;
  }

  void setEncoder(int e) {
    encoder = e;
    option = View::encoderToOption(e - initEncoder, OPTIONS_NO);
  }

  void setStep(uint32_t s) {
    step = s;
  }

  void setSteps(uint32_t s) {
    steps = s;
  }

  void setTime(uint32_t ms) {
    time_ms = ms;
  }

  void setTemperature(float t) {
    temperature = t;
  }

  void setSmoke(bool s) {
    smoke = s;
  }

  ViewEvent press() {
    switch(option) {
      case 0: return ViewEvent::EditTime;
      case 1: return ViewEvent::EditTemp;
      case 2: return ViewEvent::EditSmoke;
    }
    return ViewEvent::Next;
  }

private:
  static constexpr int OPTIONS_NO = 4;

  int initEncoder;
  int encoder;

  uint32_t option;
  uint32_t step, steps;
  uint32_t time_ms;
  float temperature;
  bool smoke;

  String timeFormat(uint32_t ms) {
    int totalMinutes = round((float)ms / 1000.0F / 60.0F);
    int hours   = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return String(hours) + " h " + String(minutes) + " m";
  }

  String temperatureFormat(float temp) {
    return String(temperature) + " " + String((char)176) + "C";
  }

  String smokeFormat(bool smoke) {
    return smoke ? "T" : "N";
  }
};

#endif // EDIT_STEP_H__
