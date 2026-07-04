#pragma once
#include "consts.h"
#include <Arduino.h>

namespace Util {

int calcBattPercentage(float voltage);
void updateBattery();
Orientation calcOrientation(float ax, float ay, float az);
void deepSleep();
int getTimerByOrientation(Orientation ori);

class OrientationDebouncer {
private:
  unsigned long lastChangeTime = 0;
  unsigned long debounceDelay;
  Orientation debouncedState = Orientation::UNDEFINED;

public:
  OrientationDebouncer(unsigned long delayMs) {
    this->debounceDelay = delayMs;
  }

  bool update(Orientation rawState) {
    if (rawState == debouncedState) {
      lastChangeTime = millis();
    } else if ((millis() - lastChangeTime) >= debounceDelay) {
      debouncedState = rawState;
      return true;
    }
    return false;
  }

  Orientation getState() const {
    return debouncedState;
  }
};

}