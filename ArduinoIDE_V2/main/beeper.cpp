#include <Arduino.h>
#include "beeper.h"
#include "consts.h"

unsigned long lastBeep = 0;
int arrayLength = sizeof(beepDelays) / sizeof(beepDelays[0]);
int beepIndex = 0;

void Beeper::beep(unsigned int duration) {
  digitalWrite(BEEPER_PIN, HIGH);
  delay(duration);
  digitalWrite(BEEPER_PIN, LOW);
}

void Beeper::setup() {
  pinMode(BEEPER_PIN, OUTPUT);
  digitalWrite(BEEPER_PIN, LOW);
}

void Beeper::cycleBeeper() {
  if (lastBeep == 0) beep(100);
  if (millis() - lastBeep >= beepDelays[beepIndex]) {
    beep(100);
    lastBeep = millis();
    beepIndex++;
    if (beepIndex >= arrayLength - 1) beepIndex = 0;
  }
}