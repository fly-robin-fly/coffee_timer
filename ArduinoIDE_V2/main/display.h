#pragma once
#include "consts.h"

namespace Display {

void setup();
void setBattery(int percentage);
void shutOffBacklight();
void rotateScreen(Orientation ori);
void updateTimer(int seconds, int selSeconds);
}