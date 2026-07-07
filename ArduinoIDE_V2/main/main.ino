#include <Arduino.h>
#include "consts.h"
#include "battery.h"
#include "display.h"
#include "util.h"
#include "display.h"
#include "qmi.h"
#include <lvgl.h>
#include "beeper.h"


int remSeconds = 0;          // remaining seconds
int selSeconds = 0;          // selected  timer seconds
unsigned long lastTick = 0;  // last count tick timestamp
unsigned long startedBeeping = 0;


void setup() {
  setCpuFrequencyMhz(80);  // reducing CPU clock to 80MHz

  Serial.begin(115200);
  QMI::setup();
  Display::setup();
  Beeper::setup();
  Util::updateBattery();
}

void loop() {
  lv_timer_handler();
  delay(20);
  lv_tick_inc(20);

  float ax, ay, az;
  if (QMI::getAccelerometer(ax, ay, az)) {
    Orientation currentOri = Util::calcOrientation(ax, ay, az);
    if (Util::updateOriDebounce(currentOri)) {
      Orientation ori = Util::getDebouncedOriState();
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
    Util::updateBattery();
    if (remSeconds == 0) startedBeeping = millis();
  }

  if (remSeconds == 0) {
    Beeper::cycleBeeper();
    Display::cycleTimerFinish();
  }
  if (remSeconds == 0 && millis() - startedBeeping >= 1000 * 30) Util::deepSleep();
}