#pragma once

namespace QMI {

void setup();
void setupWakeup();
bool getAccelerometer(float &ax, float &ay, float &az);

}