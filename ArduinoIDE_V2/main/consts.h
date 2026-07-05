#pragma once

#define BAT_ADC_PIN 1
#define TFT_BL_PIN 2
#define IMU_INT_PIN GPIO_NUM_4
#define I2C_SDA_PIN 6
#define I2C_SCL_PIN 7
#define BEEPER_PIN 15

#define BAT_FULL_VOLTAGE 4.2
#define BAT_EMPTY_VOLTAGE 3.5

#define ORI_DEBOUNCE_DELAY 300

constexpr int timers[4] = { 120, 180, 240, 10 };
constexpr int beepDelays[4] = { 200, 200, 1000 };

enum class Orientation {
  SLEEP,
  DEG_0,
  DEG_90,
  DEG_180,
  DEG_270,
  UNDEFINED
};