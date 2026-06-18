#include <Arduino.h>
#include "types.h"
#include "battery.h"
#include "display.h"
#include "util.h"
#include "display.h"
#include "qmi.h"


int countFrom = 0;
int timers[4] = { 120, 180, 240, 360 };
int selTimer = 0;
long lastTick = 0;


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
      if (Orientation::SLEEP) util_deepSleep();
      disp_rotateScreen(ori);
      lastOrientation = ori;
    }
  }
}