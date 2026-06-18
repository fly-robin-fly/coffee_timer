#include <Arduino.h>
#include "types.h"
#include "battery.h"
#include "util.h"
#include "qmi.h"
#include "display.h"

#define IMU_INT_PIN GPIO_NUM_4

#define BAT_FULL_VOLTAGE 4.2
#define BAT_EMPTY_VOLTAGE 3.5

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
