#include "FsmActions.h"

#include <stdint.h>
#include <stdlib.h>

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "board.h"

#include "menu/StartMenuView.h"
#include "menu/ProgressView.h"
#include "menu/MainMenuView.h"
#include "menu/ConfigView.h"
#include "menu/EditSteps.h"
#include "menu/EditStep.h"
#include "menu/EditTemp.h"
#include "menu/EditTime.h"
#include "menu/EditSmoke.h"

#include "fsm/fsm.h"

#include "controller/Controller.h"
#include "controller/ControllerInfo.h"

#include "Config.h"

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static Display newdisp(display);
static StartMenuView startMenuView(newdisp);
static ProgressView progressView(newdisp);
static MainMenuView mainMenuView(newdisp);
static ConfigView configView(newdisp);
static EditStepsView editStepsView(newdisp);
static EditStepView editStepView(newdisp);
static EditTimeView editTimeView(newdisp);
static EditTempView editTempView(newdisp);
static EditSmokeView editSmokeView(newdisp);

static View *currentView = &startMenuView;

static int lastEncoder = 0;
static int encCounter = 0;

static Config config;
static ConfigData configData;

static uint32_t editConfigStep = 0;
static ConfigData editConfigData;

static void configureDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(2);
}

static void smokeRelay(bool state) { digitalWrite(SMOKE_RELAY_PIN, !state); }
static void heaterRelay(bool state) { digitalWrite(HEATER_RELAY_PIN, !state); }

static Controller ctrl(smokeRelay, heaterRelay);

static void configureController(void) {
  config.load();
  config.print();

  config.getData(configData);

  ctrl.clearSteps();

  for (uint32_t i = 0; i < configData.entries; i++) {
    ctrl.addStep({
      configData.data[i].time_ms,
      configData.data[i].temperature,
      configData.data[i].smoke
    });
  }

  editConfigData = configData;
}

static void no_action(fsm_state_t state, fsm_event_t evt) {
  // Serial.println("FSM[NoAction]: " + String(state) + " " + String(evt));
}

static void update_start_menu(void) {
  uint32_t option = View::encoderToOption(
    encCounter - lastEncoder,
    startMenuView.numberOfOptions());

  startMenuView.selectOption(option);
  startMenuView.show();
}

static void update_progress(void) {
  ControllerInfo info = {0};
  ctrl.getProgressInfo(info);

  progressView.setProgress(info);
  progressView.show();
}

static void start(void) {
  Serial.println("FSM[Start]");
  currentView = &progressView;

  ctrl.start();
  lastEncoder = encCounter;

  update_progress();
}

static void stop(void) {
  Serial.println("FSM[Stop]");
  currentView = &startMenuView;

  ctrl.stop();
  lastEncoder = encCounter;

  update_start_menu();
}

static void update_edit_steps(void) {
  editStepsView.setEncoder(encCounter);
  editStepsView.show();
}

static void edit_steps(void) {
  Serial.println("FSM[Edit]");
  currentView = &editStepsView;

  editConfigStep = 0;
  editStepsView.setInitEncoder(encCounter);
  editStepsView.setSteps(editConfigData.entries);

  update_edit_steps();
}

static void update_edit_step(void) {
  editStepView.setEncoder(encCounter);
  editStepView.show();
}

static void edit_step(void) {
  Serial.println("FSM[Edit]");
  currentView = &editStepView;

  editConfigData.entries = editStepsView.getSteps();

  ConfigEntry *data = &editConfigData.data[editConfigStep];

  editStepView.setInitEncoder(encCounter);
  editStepView.setSteps(editConfigData.entries);
  editStepView.setStep(editConfigStep);
  editStepView.setTime(data->time_ms);
  editStepView.setTemperature(data->temperature);
  editStepView.setSmoke(data->smoke);

  update_edit_step();
}

static void start_menu(void) {
  Serial.println("FSM[OpenStartMenu]");
  currentView = &startMenuView;

  lastEncoder = encCounter;
  update_start_menu();
}

static void update_view(void) {
  uint32_t option = View::encoderToOption(
    encCounter - lastEncoder,
    configView.numberOfOptions());

  configView.selectOption(option);
  configView.show();
}

static void view(void) {
  Serial.println("FSM[OpenView]");
  currentView = &configView;

  lastEncoder = encCounter;
  configView.setConfiguration(configData);
  update_view();
}

static void update_main_menu(void) {
  uint32_t option = View::encoderToOption(
    encCounter - lastEncoder,
    mainMenuView.numberOfOptions());

  mainMenuView.selectOption(option);
  mainMenuView.show();
}

static void main_menu(void) {
  Serial.println("FSM[OpenMainMenu]");
  currentView = &mainMenuView;

  lastEncoder = encCounter;
  update_main_menu();
}

static void progress(void) {
  Serial.println("FSM[OpenProgress]");
  currentView = &progressView;

  lastEncoder = encCounter;
  update_progress();
}

static void update_edit_time(void) {
  editTimeView.setEncoder(encCounter);
  editTimeView.show();
}

static void edit_time(void) {
  Serial.println("FSM[EditTimeView]");
  currentView = &editTimeView;

  ConfigEntry *data = &editConfigData.data[editConfigStep];

  editTimeView.setInitEncoder(encCounter);
  editTimeView.setTime(data->time_ms);

  update_edit_time();
}

static void update_edit_temp(void) {
  editTempView.setEncoder(encCounter);
  editTempView.show();
}

static void edit_temp(void) {
  Serial.println("FSM[EditTempView]");
  currentView = &editTempView;

  ConfigEntry *data = &editConfigData.data[editConfigStep];

  editTempView.setInitEncoder(encCounter);
  editTempView.setTemperature(data->temperature);

  update_edit_temp();
}

static void update_edit_smoke(void) {
  editSmokeView.setEncoder(encCounter);
  editSmokeView.show();
}

static void edit_smoke(void) {
  Serial.println("FSM[EditSmokeView]");
  currentView = &editSmokeView;

  ConfigEntry *data = &editConfigData.data[editConfigStep];

  editSmokeView.setInitEncoder(encCounter);
  editSmokeView.setSmoke(data->smoke);

  update_edit_smoke();
}

static void edit_set_time(void) {
  ConfigEntry *data = &editConfigData.data[editConfigStep];
  data->time_ms = editTimeView.getTime();
  edit_step();
}

static void edit_set_temp(void) {
  ConfigEntry *data = &editConfigData.data[editConfigStep];
  data->temperature = editTempView.getTemperature();
  edit_step();
}

static void edit_set_smoke(void) {
  ConfigEntry *data = &editConfigData.data[editConfigStep];
  data->smoke = editSmokeView.getSmoke();
  edit_step();
}

static void edit_next_step(void) {
  if (++editConfigStep >= editConfigData.entries) {
    config.setData(editConfigData);
    config.store();

    configureController();

    fsm_event(FSM_EVT_CLICK_EXIT);
  } else {
    edit_step();
  }
}

void fsmActionsInit(void) {
  fsm_actions_t actions = {
    .start              = start,
    .stop               = stop,

    .edit_steps         = edit_steps,
    .update_edit_steps  = update_edit_steps,

    .start_menu         = start_menu,
    .update_start_menu  = update_start_menu,

    .view               = view,
    .update_view        = update_view,

    .main_menu          = main_menu,
    .update_main_menu   = update_main_menu,

    .progress           = progress,
    .update_progress    = update_progress,

    .edit_step          = edit_step,
    .update_edit_step   = update_edit_step,

    .edit_time          = edit_time,
    .update_edit_time   = update_edit_time,

    .edit_temp          = edit_temp,
    .update_edit_temp   = update_edit_temp,

    .edit_smoke         = edit_smoke,
    .update_edit_smoke  = update_edit_smoke,

    .edit_set_time      = edit_set_time,
    .edit_set_temp      = edit_set_temp,
    .edit_set_smoke     = edit_set_smoke,

    .edit_next_step     = edit_next_step,

    .no_action          = no_action
  };
  fsm_init(&actions);

  configureDisplay();
  configureController();

  startMenuView.selectOption(0);
  startMenuView.show();
}

void fsmActionsSetTemperature(float temperature) {
  ctrl.setTemperature(temperature);
  fsm_event(FSM_EVT_TIMER);
}

void fsmActionsSetEncoder(int encoder) {
  encCounter = encoder;
  fsm_event(FSM_EVT_ENCODER);
}

void fsmActionsButtonPressed(void) {
  ViewEvent event = currentView->press();

  switch (event) {
    case ViewEvent::Start:      fsm_event(FSM_EVT_CLICK_START); break;
    case ViewEvent::Stop:       fsm_event(FSM_EVT_CLICK_STOP);  break;
    case ViewEvent::Edit:       fsm_event(FSM_EVT_CLICK_EDIT);  break;
    case ViewEvent::View:       fsm_event(FSM_EVT_CLICK_VIEW);  break;
    case ViewEvent::Click:      fsm_event(FSM_EVT_CLICK);       break;
    case ViewEvent::Exit:       fsm_event(FSM_EVT_CLICK_EXIT);  break;
    case ViewEvent::EditTime:   fsm_event(FSM_EVT_CLICK_TIME);  break;
    case ViewEvent::EditTemp:   fsm_event(FSM_EVT_CLICK_TEMP);  break;
    case ViewEvent::EditSmoke:  fsm_event(FSM_EVT_CLICK_SMOKE); break;
    case ViewEvent::Next:       fsm_event(FSM_EVT_CLICK_NEXT);  break;
    case ViewEvent::Prev:       fsm_event(FSM_EVT_CLICK_PREV);  break;
  }

  Serial.println("State: " + String(fsm_get_state()));
}

void fsmActionsRun() {
  ctrl.run();
}
