#ifndef MAIN_MENU_VIEW_H__
#define MAIN_MENU_VIEW_H__

#include "View.h"
#include "../display/Display.h"

class MainMenuView: public View {
public:
  MainMenuView(Display &display): View(display) {
    option = 0;
    options[0] = ViewEvent::View;
    options[1] = ViewEvent::Stop;
    options[2] = ViewEvent::Exit;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader("Menu");
    display.drawButton(0, "Podglad",  option == 0);
    display.drawButton(1, "Stop",     option == 1);
    display.drawButton(2, "Powrot",   option == 2);
    display.display();
  }

  uint8_t numberOfOptions() {
    return 3;
  }

  void selectOption(uint32_t o) {
    if (o != option) {
      option = o;
      show();
    }
  }

  ViewEvent press() {
    return options[option];
  }

private:
  uint32_t option;
  ViewEvent options[3];
};

#endif // MAIN_MENU_VIEW_H__
