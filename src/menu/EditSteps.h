#ifndef EDIT_STEPS_H__
#define EDIT_STEPS_H__

#include "View.h"
#include "display/Display.h"

class EditStepsView: public View {
public:
  EditStepsView(Display &display): View(display) {
    encoder = 0;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader("Ilosc krokow");
    display.setFontSize(16);
    display.drawTextCenter(26, String(getSteps()));
    display.display();
  }

  void setInitEncoder(int e) {
    initEncoder = encoder = e;
  }

  void setEncoder(int e) {
    encoder = e;
    if ((encoder - initEncoder) < 0) {
      initEncoder = encoder;
    }
    if ((encoder - initEncoder) > (MAX_STEPS - 1)) {
      initEncoder = encoder - (MAX_STEPS - 1);
    }
  }

  void setSteps(uint32_t steps) {
    initEncoder = encoder - steps + 1;
  }

  uint32_t getSteps() {
    return (encoder - initEncoder) + 1;
  }

  ViewEvent press() {
    return ViewEvent::Click;
  }

private:
  static constexpr int MAX_STEPS = 10;

  int encoder;
  int initEncoder;

  String encoderToTime() {
    int totalMinutes = (encoder - initEncoder) * 5;
    int hours   = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return String(hours) + " h " + String(minutes) + " m";
  }
};

#endif // EDIT_STEPS_H__
