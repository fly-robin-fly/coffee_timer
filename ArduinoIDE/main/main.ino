#include "lis2dw12.h"
#include "display.h"

Orientation lastOrientation = Orientation::UNKNOWN;
long lastTick = 0;
int timer = 0;
int countFrom = 0;
int timers[4] = { 120, 180, 240, 360 };

void setup() {
  Serial.begin(115200);

  display_setup();
  lis2dw12_setup();
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(100));

  Orientation orientation = lis2dw12_getOrientation();
  display_setOrientation(orientation);
  if (orientation != lastOrientation) {
    lastOrientation = orientation;
    lastTick = millis();
    switch (orientation) {
      case Orientation::DEGREES_0:
        countFrom = timers[0];
        break;
      case Orientation::DEGREES_90:
        countFrom = timers[1];
        break;
      case Orientation::DEGREES_180:
        countFrom = timers[2];
        break;
      case Orientation::DEGREES_270:
        countFrom = timers[3];
        break;
    }
    timer = countFrom;
    display_updateTimer(timer, countFrom);
  }
  if (timer > 0 && millis() - lastTick >= 1000) {
    timer--;
    display_updateTimer(timer, countFrom);
    lastTick = millis();
  }
}