#include "lis2dw12.h"
#include "display.h"
#include "esp_sleep.h"

#define LIS_INT1_PIN GPIO_NUM_1

Orientation lastOrientation = Orientation::UNKNOWN;
long lastTick = 0;
int timer = 0;
int countFrom = 0;
int timers[4] = { 120, 180, 240, 360 };

#define LIS_INT1_PIN GPIO_NUM_9

void enterSleepMode() {
  Serial.println("Going to sleep...");

  display_enterSleepMode();

  vTaskDelay(pdMS_TO_TICKS(200));

  gpio_set_direction(LIS_INT1_PIN, GPIO_MODE_INPUT);
    
    // 2. Enable internal pull-up (assuming you are triggering wake by pulling LOW)
    gpio_set_pull_mode(LIS_INT1_PIN, GPIO_PULLUP_ONLY);

    // 3. Configure the wake level (wake on LOW level)
    gpio_wakeup_enable(LIS_INT1_PIN, GPIO_INTR_LOW_LEVEL);

    // 4. Enable the GPIO wakeup source globally
    esp_sleep_enable_gpio_wakeup();

    // 5. Enter light sleep
    esp_light_sleep_start();

  // Code resumes here after the interrupt
  Serial.println("Woke up from Light Sleep!");
  display_setup();
}


void setup() {
  Serial.begin(9600);

  display_setup();
  lis2dw12_setup();
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(200));

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
  float pitch = lis2dw12_getPitch();
  if (pitch > 70 && pitch < 110) {
    enterSleepMode();
  }
  Serial.println(pitch);
}