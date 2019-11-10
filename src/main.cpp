#include <Arduino.h>

#include <Wire.h>
#include <Rotary.h>

#include "Temperature.h"
#include "FsmActions.h"
#include "utils/Interval.h"
#include "board.h"

#define MEASUREMENT_INTERVAL 1000

static Interval measInterval;

static Rotary r(ENC1_PIN, ENC2_PIN);
static int encCounter = 0;

static void configreRelays(void);
static void configureSerials(void);
static void configureEncoder(void);
static void configureButton(void);

void setup() {
  configreRelays();
  configureSerials();
  configureEncoder();
  configureButton();

  fsmActionsInit();

  measInterval.setInterval(MEASUREMENT_INTERVAL);
  measInterval.start();
}

static void configreRelays() {
  pinMode(SMOKE_RELAY_PIN, OUTPUT);
  digitalWrite(SMOKE_RELAY_PIN, true);

  pinMode(HEATER_RELAY_PIN, OUTPUT);
  digitalWrite(HEATER_RELAY_PIN, true);
}

static void configureSerials(void) {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start!");
  Serial.flush();
}

static void handleInterrupt(void);

static void configureEncoder(void) {
  pinMode(ENC1_PIN, INPUT);
  pinMode(ENC2_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC1_PIN), handleInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2_PIN), handleInterrupt, CHANGE);
}

static void handleInterrupt(void) {
  uint8_t res = r.process();
  if (res == DIR_NONE) { return; }
  encCounter += res == DIR_CW ? +1 : -1;
}

static void configureButton(void) {
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  static int lastCounter = 0;
  static bool pressed = false;

  if (lastCounter != encCounter) {
    lastCounter = encCounter;
    Serial.println("Counter: " + String(encCounter));

    fsmActionsSetEncoder(encCounter);
  }

  if (pressed == false) {
    if (digitalRead(BUTTON_PIN) == 0) {
      pressed = true;
      Serial.println("Button pressed");

      fsmActionsButtonPressed();
    }
  } else {
    if (digitalRead(BUTTON_PIN) == 1) {
      pressed = false;
    }
  }

  if (measInterval.hasElapsed() == true) {
    float resistance;
    float temperature = temperatureMeasure(&resistance);
    Serial.println("T" + String(temperature, 2) + " " + String(resistance, 2));

    fsmActionsSetTemperature(temperature);
  }

  fsmActionsRun();
}
