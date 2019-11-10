#ifndef CONFIG_VIEW_H__
#define CONFIG_VIEW_H__

#include "View.h"
#include "display/Display.h"
#include "../Config.h"

class ConfigView: public View {
public:
  ConfigView(Display &display): View(display) {
    option = 0;
  }

  void show() {
    display.clear();
    display.setFontSize(7);
    display.drawHeader("Konfiguracja (" + String(data.entries) + ")");

    uint32_t i, o = option < 3 ? 0 : option - 2;
    for (i = 0; i < 3 && o < data.entries; i++, o++) {
      display.drawButton(i, buildStats(o), option == o);
    }

    if (i != 3) {
      display.drawButton(i, "Powrot", option == data.entries);
    }

    display.display();
  }

  uint8_t numberOfOptions() {
    return data.entries + 1;
  }

  void selectOption(uint32_t o) {
    if (o != option) {
      option = o;
      show();
    }
  }

  ViewEvent press() {
    if (option == data.entries) {
      return ViewEvent::Exit;
    }
    return ViewEvent::Click;
  }

  void setConfiguration(ConfigData const &d) {
    data = d;
  }

private:
  uint32_t option;
  ConfigData data;

  String buildStats(uint32_t option) {
    uint32_t o = option;
    String stats = String(o + 1) + ": ";
    stats       += String(data.data[o].temperature, 0) + (char)176 + "C, ";
    stats       += String(millisToHours(data.data[o].time_ms), 1) + "h, ";
    stats       += (data.data[o].smoke ? 'T' : 'N');
    return stats;
  }

  float millisToHours(uint32_t ms) {
    return ms / 1000.0F / 60 / 60;
  }
};

#endif // CONFIG_VIEW_H__
