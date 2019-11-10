#ifndef EDIT_TIME_H__
#define EDIT_TIME_H__

#include "View.h"
#include "display/Display.h"

class EditTimeView: public View {
public:
  EditTimeView(Display &display): View(display) {
    initEncoder = 0;
    encoder = 0;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader("Czas");
    display.setFontSize(14);
    display.drawTextCenter(26, timeFormat(getTime()));
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
    if ((encoder - initEncoder) > ENC_MAX) {
      initEncoder = encoder - ENC_MAX;
    }
  }

  void setTime(uint32_t ms) {
    initEncoder = encoder - round((float)ms / TIME_STEP);
  }

  uint32_t getTime() {
    return (uint32_t)(encoder - initEncoder) * TIME_STEP;
  }

  ViewEvent press() {
    return ViewEvent::Click;
  }

private:
  static constexpr uint32_t TIME_STEP = 10UL * 60 * 1000;
  static constexpr uint32_t TIME_MAX  = 12UL * 60 * 60 * 1000;
  static constexpr int ENC_MAX        = TIME_MAX / TIME_STEP;

  int encoder;
  int initEncoder;

  String timeFormat(uint32_t ms) {
    int totalMinutes = round((float)ms / 1000.0F / 60.0F);
    int hours   = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    return String(hours) + " h " + String(minutes) + " m";
  }
};

#endif // EDIT_TIME_H__
