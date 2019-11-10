#ifndef DISPLAY_H__
#define DISPLAY_H__

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../fonts/roboto_regular_6.h"
#include "../fonts/roboto_regular_7.h"
#include "../fonts/roboto_regular_8.h"
#include "../fonts/roboto_regular_10.h"
#include "../fonts/roboto_regular_12.h"
#include "../fonts/roboto_regular_14.h"
#include "../fonts/roboto_regular_16.h"
#include "../fonts/roboto_regular_20.h"

struct FontEntry {
  int size;
  GFXfont const *font;
};

class Display {
public:
  Display(Adafruit_SSD1306 &display): internalDispaly(display) {
    font = &Roboto_Regular12pt8b;
  }

  void clear() {
    internalDispaly.clearDisplay();

    internalDispaly.setTextSize(1);
    internalDispaly.setTextColor(WHITE);
    internalDispaly.cp437(true);
    internalDispaly.setFont(font);
  }

  void setFontSize(uint8_t size) {
    static FontEntry regular[] = {
      { 20, &Roboto_Regular20pt8b },
      { 16, &Roboto_Regular16pt8b },
      { 14, &Roboto_Regular14pt8b },
      { 12, &Roboto_Regular12pt8b },
      { 10, &Roboto_Regular10pt8b },
      { 8,  &Roboto_Regular8pt8b  },
      { 7,  &Roboto_Regular7pt8b  },
      { 6,  &Roboto_Regular6pt8b  },
    };

    for (size_t i = 0; i < sizeof(regular) / sizeof(FontEntry); i++) {
      if (size == regular[i].size) {
        internalDispaly.setFont(regular[i].font);
        return;
      }
    }
  }

  void display() {
    internalDispaly.display();
  }

  void drawHeader(String title) {
    internalDispaly.setCursor(0, 10);
    internalDispaly.println(title);
    internalDispaly.drawFastHLine(0, 13, 128, WHITE);
  }

  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    internalDispaly.drawRect(x, y, w, h, color);
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    internalDispaly.fillRect(x, y, w, h, color);
  }

  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    internalDispaly.drawRoundRect(x, y, w, h, r, color);
  }

  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    internalDispaly.fillRoundRect(x, y, w, h, r, color);
  }

  void drawTextCenter(int16_t y, String const &text) {
    int16_t x1, y1;
    uint16_t w, h;

    internalDispaly.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int16_t x = 128 / 2 - w / 2;

    x = x - x1;
    y = y - y1;

    internalDispaly.setCursor(x, y);
    internalDispaly.print(text);
  }

  void drawButton(int index, String const &label, bool active = false) {
    int yOffset = 14 + (index % 3) * 17;
    int xOffset = 0;

    if (active) {
      fillRoundRect(
        xOffset + btnMargin,
        yOffset + btnMargin,
        btnWidth, btnHeight, 3, WHITE
      );
    } else {
      drawRoundRect(
        xOffset + btnMargin,
        yOffset + btnMargin,
        btnWidth, btnHeight, 3, WHITE
      );
    }
    internalDispaly.setCursor(
      xOffset + btnMargin + btnMarginText,
      yOffset + btnMargin + btnMarginText + btnTextHeight
    );
    internalDispaly.setTextColor(active ? BLACK : WHITE);
    internalDispaly.print(label);
  }
protected:
  Adafruit_SSD1306 &internalDispaly;

  GFXfont const *font = NULL;

  const int btnMargin     = 2;
  const int btnMarginText = 3;
  const int btnTextHeight = 8;
  const int btnWidth      = 120;
  const int btnHeight     = 14;
};

#endif // DISPLAY_H__
