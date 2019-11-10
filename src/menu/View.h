#ifndef VIEW_H__
#define VIEW_H__

#include "../display/Display.h"

enum class ViewEvent {
  Start, Stop, Edit, View, Click, Exit,
  EditTime, EditTemp, EditSmoke, Next, Prev
};

class View {
public:
  View(Display &display): display(display) {
  }

  virtual void show() = 0;
  virtual ViewEvent press() = 0;

  static uint32_t encoderToOption(int encoder, uint32_t options) {
    if (encoder >= 0) {
      return encoder % options;
    } else {
      return ((options - 1) - (abs(encoder) % options) + 1) % options;
    }
  }

protected:
  Display &display;
};

#endif // VIEW_H__
