#pragma once
#include "consts.h"

namespace Beeper {

void setup();
void beep(unsigned int frequency, unsigned int duration);
bool cycleBeeper();
void playWakeUp();
void playShutdown();
}