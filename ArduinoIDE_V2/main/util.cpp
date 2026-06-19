#include <Arduino.h>
#include "consts.h"
#include "battery.h"
#include "util.h"
#include "qmi.h"
#include "display.h"

int util_calcBattPercentage(float voltage) {
  return (int)map(voltage, BAT_EMPTY_VOLTAGE, BAT_FULL_VOLTAGE, 1, 100);
}

void util_updateBattery() {
  float voltage = bat_getVoltage();
  int percentage = util_calcBattPercentage(voltage);
  disp_setBattery(percentage);
}

Orientation util_calcOrientation(float ax, float ay, float az) {
  if (az < -0.8) return Orientation::SLEEP;
  if (ay > 0.8) return Orientation::DEG_0;
  if (ax > 0.8) return Orientation::DEG_90;
  if (ay < -0.8) return Orientation::DEG_180;
  if (ax < -0.8) return Orientation::DEG_270;
}

void util_deepSleep() {
  Serial.println("Face down: Entering Deep Sleep.");
  disp_shutOffBacklight();
  qmi_setupWakeup();
  esp_sleep_enable_ext0_wakeup(IMU_INT_PIN, 1);
  esp_deep_sleep_start();
}

int util_getTimerByOrientation(Orientation ori) {
  if (ori == Orientation::DEG_0) return timers[0];
  if (ori == Orientation::DEG_90) return timers[1];
  if (ori == Orientation::DEG_180) return timers[2];
  if (ori == Orientation::DEG_270) return timers[3];
  return timers[0];
}