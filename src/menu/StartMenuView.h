#ifndef START_MENU_VIEW_H__
#define START_MENU_VIEW_H__

#include "View.h"
#include "../display/Display.h"

class StartMenuView: public View {
public:
  StartMenuView(Display &display): View(display) {
    option = 0;
    options[0] = ViewEvent::Start;
    options[1] = ViewEvent::View;
    options[2] = ViewEvent::Edit;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader("Menu");
    display.drawButton(0, "Start",    option == 0);
    display.drawButton(1, "Podglad",  option == 1);
    display.drawButton(2, "Edycja",   option == 2);
    display.display();
  }

  uint8_t numberOfOptions() {
    return MAX_OPTIONS;
  }

  void selectOption(uint32_t o) {
    if (o < MAX_OPTIONS && o != option) {
      option = o;
      show();
    }
  }

  ViewEvent press() {
    return options[option];
  }

private:
  const static uint8_t MAX_OPTIONS = 3;

  uint32_t option;
  ViewEvent options[MAX_OPTIONS];
};

#endif // START_MENU_VIEW_H__
