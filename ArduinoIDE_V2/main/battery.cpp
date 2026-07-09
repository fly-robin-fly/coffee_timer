#include "battery.h"
#include <Arduino.h>
#include "consts.h"
#include "util.h"

float Battery::getVoltage() {
  analogReadResolution(12);
  int adValue = analogRead(BAT_ADC_PIN);
  float batVoltage = (3.3 / 4096.0) * 3.0 * adValue;
  return batVoltage;
}


unsigned long lastBatteryUpdate = 0;

void Battery::cycleBatteryUpdate() {
  if (millis() - lastBatteryUpdate >= 5000) {
    Util::updateBattery();
    lastBatteryUpdate = millis();
  }
}