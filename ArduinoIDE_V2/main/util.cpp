#include "util.h"
#include <Arduino.h>
#include "consts.h"
#include "battery.h"
#include "util.h"
#include "qmi.h"
#include "display.h"

int Util::calcBattPercentage(float voltage) {
  return (int)map(voltage, BAT_EMPTY_VOLTAGE, BAT_FULL_VOLTAGE, 1, 100);
}

void Util::updateBattery() {
  float voltage = Battery::getVoltage();
  int percentage = Util::calcBattPercentage(voltage);
  Display::setBattery(percentage);
}

Orientation Util::calcOrientation(float ax, float ay, float az) {
  //if (az < -0.8) return Orientation::SLEEP;
  if (ay > 0.8) return Orientation::DEG_270;
  if (ax > 0.8) return Orientation::DEG_180;
  if (ay < -0.8) return Orientation::DEG_90;
  if (ax < -0.8) return Orientation::DEG_0;
  return Orientation::DEG_0;
}

void Util::deepSleep() {
  Serial.println("Face down: Entering Deep Sleep.");
  Display::shutOffBacklight();
  QMI::setupWakeup();
  esp_sleep_enable_ext0_wakeup(IMU_INT_PIN, 1);
  esp_deep_sleep_start();
}

int Util::getTimerByOrientation(Orientation ori) {
  if (ori == Orientation::DEG_0) return timers[0];
  if (ori == Orientation::DEG_90) return timers[1];
  if (ori == Orientation::DEG_180) return timers[2];
  if (ori == Orientation::DEG_270) return timers[3];
  return timers[0];
}