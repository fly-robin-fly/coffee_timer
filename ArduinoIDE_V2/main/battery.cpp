#include "battery.h"
#include <Arduino.h>
#include "consts.h"

float Battery::getVoltage() {
  analogReadResolution(12);
  int adValue = analogRead(BAT_ADC_PIN);
  float bat_voltage = (3.3 / 4096.0) * 3.0 * adValue;
  return bat_voltage;
}