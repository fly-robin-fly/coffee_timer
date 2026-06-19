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


Orientation lastOrientation = Orientation::SLEEP;


void setup() {
  Serial.begin(115200);

  qmi_setup();
  disp_setup();
  util_updateBattery();
}

void loop() {
  lv_timer_handler();  // Process LVGL tasks
  delay(5);

  float ax, ay, az;
  if (qmi_getAccelerometer(ax, ay, az)) {

    Orientation ori = util_calcOrientation(ax, ay, az);
    if (ori != lastOrientation) {
      lastOrientation = ori;
      if (ori == Orientation::SLEEP) util_deepSleep();
      remSeconds = util_getTimerByOrientation(ori);
      selSeconds = remSeconds;
      disp_rotateScreen(ori);
      disp_updateTimer(remSeconds, selSeconds);
      lastTick = millis();
    }
  }
  if (remSeconds > 0 && millis() - lastTick >= 1000) {
    remSeconds--;
    disp_updateTimer(remSeconds, selSeconds);
    lastTick = millis();
  }
}