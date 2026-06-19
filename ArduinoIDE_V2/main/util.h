#pragma once
#include "consts.h"

int util_calcBattPercentage(float voltage);
void util_updateBattery();
Orientation util_calcOrientation(float ax, float ay, float az);
void util_deepSleep();
int util_getTimerByOrientation(Orientation ori);