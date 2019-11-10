#ifndef PROGRESS_VIEW_H__
#define PROGRESS_VIEW_H__

#include "View.h"
#include "../controller/ControllerInfo.h"
#include "../display/Display.h"

class ProgressView: public View {
public:
  ProgressView(Display &display): View(display) {
  }

  void setProgress(ControllerInfo const &ci) {
    info = ci;
  }

  void show() {
    String temp = String(info.temperature, 1) + " " + (char)176 + "C";

    display.clear();
    display.setFontSize(10);
    display.drawTextCenter(0, temp);

    float progress = floor(info.totalProgress);
    display.drawRect(0, 17, 128, 12, WHITE);
    display.fillRect(0, 17, progress / 100.0F * 128.0F, 12, WHITE);

    display.drawTextCenter(32, String(progress, 0) + " %");

    display.setFontSize(6);
    String stats = String(info.step.id + 1) + "/" + String(info.totalSteps);
    stats += ", " + String(info.step.temperature, 0) + (char)176 + "C, ";
    stats += String(info.step.time / 1000.0F / 60 / 60, 1) + "h, ";
    stats += (info.step.smokeOn ? 'T' : 'N');
    display.drawTextCenter(53, stats);

    display.display();
  }

  ViewEvent press() {
    return ViewEvent::Click;
  }

private:
  ControllerInfo info;
};

#endif // PROGRESS_VIEW_H__
