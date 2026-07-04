#include <Arduino.h>
#include "consts.h"
#include "battery.h"
#include "display.h"
#include "util.h"
#include "display.h"
#include "qmi.h"
#include <lvgl.h>


int remSeconds = 0;  // remaining seconds
int selSeconds = 0;  // selected  timer seconds
long lastTick = 0;   // last count tick timestamp


Util::OrientationDebouncer oriDebouncer(300);


void setup() {
  Serial.begin(115200);
  QMI::setup();
  Display::setup();
  Util::updateBattery();
  Display::updateTimer(60, 60);

  while (true) {
    for (int i = 100; i >= 0; i--) {
      Display::updateBattery(i);
      lv_timer_handler();
      delay(100);
      lv_tick_inc(100);
    }
  }
}

void loop() {
  lv_timer_handler();
  delay(5);
  lv_tick_inc(5);

  float ax, ay, az;
  if (QMI::getAccelerometer(ax, ay, az)) {
    Orientation currentOri = Util::calcOrientation(ax, ay, az);
    if (oriDebouncer.update(currentOri)) {
      Orientation ori = oriDebouncer.getState();
      if (ori == Orientation::SLEEP) Util::deepSleep();
      remSeconds = Util::getTimerByOrientation(ori);
      selSeconds = remSeconds;
      Display::rotateScreen(ori);
      Display::updateTimer(remSeconds, selSeconds);
      lastTick = millis();
    }
  }
  if (remSeconds > 0 && millis() - lastTick >= 1000) {
    remSeconds--;
    Display::updateTimer(remSeconds, selSeconds);
    lastTick = millis();
  }
}