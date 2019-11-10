#ifndef EDIT_TEMP_H__
#define EDIT_TEMP_H__

#include "View.h"
#include "display/Display.h"

class EditTempView: public View {
public:
  EditTempView(Display &display): View(display) {
    initEncoder = 0;
    encoder = 0;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader(String("Temperatura (") + String((char)176) + "C)");
    display.setFontSize(16);
    display.drawTextCenter(26, String(getTemperature()));
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
    if ((encoder - initEncoder) > RANGE_TEMP) {
      initEncoder = encoder - RANGE_TEMP;
    }
  }

  void setTemperature(float temperature) {
    initEncoder = encoder - temperature + MIN_TEMP;
  }

  float getTemperature() {
    return (encoder - initEncoder) + MIN_TEMP;
  }

  ViewEvent press() {
    return ViewEvent::Click;
  }

private:
  static constexpr int MIN_TEMP = 20;
  static constexpr int MAX_TEMP = 100;
  static constexpr int RANGE_TEMP = MAX_TEMP - MIN_TEMP;

  int encoder;
  int initEncoder;
};

#endif // EDIT_TEMP_H__
