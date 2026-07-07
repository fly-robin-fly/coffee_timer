#pragma once
#include "consts.h"

namespace Beeper {

void setup();
void beep(unsigned int frequency, unsigned int duration);
void cycleBeeper();
}