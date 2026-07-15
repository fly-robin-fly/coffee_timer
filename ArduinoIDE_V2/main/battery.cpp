#include "battery.h"
#include <Arduino.h>
#include "consts.h"
#include "util.h"


float Battery::getVoltage() {
  // Uses factory eFuse calibration for precise readings
  uint32_t adc_mV = analogReadMilliVolts(BAT_ADC_PIN); 
  
  // Multiply by 3.0 to account for Waveshare's 200k/100k voltage divider
  return (adc_mV * 3.0f) / 1000.0f; 
}

void Battery::sleepIfEmpty() {
  float batVoltage = Battery::getVoltage();
  if(batVoltage <= BAT_EMPTY_VOLTAGE) Util::deepSleep();
}


unsigned long lastBatteryUpdate = 0;

void Battery::cycleBatteryUpdate() {
  if (millis() - lastBatteryUpdate >= 5000) {
    Util::updateBattery();
    lastBatteryUpdate = millis();
    sleepIfEmpty();
  }
}