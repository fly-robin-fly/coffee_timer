#include "util.h"
#include <Arduino.h>
#include "consts.h"
#include "battery.h"
#include "util.h"
#include "qmi.h"
#include "display.h"
#include <driver/rtc_io.h>

int Util::calcBattPercentage(float voltage) {
  // Constrain bounds
  if (voltage <= BAT_EMPTY_VOLTAGE) return 1;
  if (voltage >= BAT_FULL_VOLTAGE) return 100;

  //  Linear interpolation for floats
  float percentage = ((voltage - BAT_EMPTY_VOLTAGE) / (BAT_FULL_VOLTAGE - BAT_EMPTY_VOLTAGE)) * 99.0f + 1.0f;
  
  return (int)percentage;
}

void Util::updateBattery() {
  float voltage = Battery::getVoltage();
  int percentage = Util::calcBattPercentage(voltage);
  Display::updateBattery(percentage);
}

Orientation Util::calcOrientation(float ax, float ay, float az) {
  if (az < -0.8 || az > 0.8) return Orientation::SLEEP;
  if (ay > 0.8) return Orientation::DEG_270;
  if (ax > 0.8) return Orientation::DEG_180;
  if (ay < -0.8) return Orientation::DEG_90;
  if (ax < -0.8) return Orientation::DEG_0;
  return Orientation::DEG_0;
}


void Util::deepSleep() {
  Serial.println("Face up or down: Entering Deep Sleep.");
  Display::deepSleep();

  delay(1000);

  QMI::setupWakeup();

  // Float I2C lines to prevent parasitic draw
  pinMode(I2C_SDA_PIN, INPUT);
  pinMode(I2C_SCL_PIN, INPUT);

  rtc_gpio_pullup_dis(IMU_INT_PIN);
  rtc_gpio_pulldown_en(IMU_INT_PIN);

  esp_sleep_enable_ext0_wakeup(IMU_INT_PIN, 1);  // 1 = Wakeup at HIGH

  // --- Beeper Hold Logic ---
  // Ensure the pin is explicitly HIGH (OFF) before sleeping
  digitalWrite(BEEPER_PIN, HIGH);
  
  // Lock the pin state in the RTC domain
  gpio_hold_en((gpio_num_t)BEEPER_PIN);
  gpio_deep_sleep_hold_en();
  // -------------------------

  esp_deep_sleep_start();
}

int Util::getTimerByOrientation(Orientation ori) {
  if (ori == Orientation::DEG_0) return timers[3];
  if (ori == Orientation::DEG_90) return timers[0];
  if (ori == Orientation::DEG_180) return timers[1];
  if (ori == Orientation::DEG_270) return timers[2];
  return timers[0];
}

unsigned long lastOriChangeTime = 0;
Orientation debouncedState = Orientation::UNDEFINED;

bool Util::updateOriDebounce(Orientation rawState) {
  if (rawState == debouncedState) {
    lastOriChangeTime = millis();
  } else if ((millis() - lastOriChangeTime) >= ORI_DEBOUNCE_DELAY) {
    debouncedState = rawState;
    return true;
  }
  return false;
}

Orientation Util::getDebouncedOriState() {
  return debouncedState;
}
