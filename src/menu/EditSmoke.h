#ifndef EDIT_SMOKE_H__
#define EDIT_SMOKE_H__

#include "View.h"
#include "display/Display.h"

class EditSmokeView: public View {
public:
  EditSmokeView(Display &display): View(display) {
    initEncoder = encoder = 0;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader("Dym");
    display.setFontSize(14);
    display.drawTextCenter(26, smokeFormat(getSmoke()));
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
    if ((encoder - initEncoder) > SMOKE_MAX) {
      initEncoder = encoder - SMOKE_MAX;
    }
  }

  void setSmoke(bool smoke) {
    initEncoder = smoke ? encoder - 1 : encoder;
  }

  bool getSmoke() {
    return (encoder - initEncoder) == 1 ? true : false;
  }

  ViewEvent press() {
    return ViewEvent::Click;
  }

private:
  static constexpr int SMOKE_MAX = 1;

  int encoder;
  int initEncoder;

  String smokeFormat(bool smoke) {
    return smoke ? "T" : "N";
  }
};

#endif // EDIT_SMOKE_H__
