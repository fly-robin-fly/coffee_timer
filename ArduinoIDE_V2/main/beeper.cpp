#include <Arduino.h>
#include "beeper.h"
#include "consts.h"

unsigned long lastBeep = 0;
int seqLength = 3;
int beepIndex = 0;

void Beeper::beep(unsigned int frequency, unsigned int duration) {
  tone(BEEPER_PIN, frequency);
  delay(duration);
  noTone(BEEPER_PIN);
}

void Beeper::playWakeUp() {
  beep(1500, 150);
  beep(2000, 80);
}

void Beeper::playShutdown() {
  beep(1500, 100);
  beep(1000, 150);
}

void Beeper::setup() {
  gpio_hold_dis((gpio_num_t)BEEPER_PIN);
  pinMode(BEEPER_PIN, OUTPUT);
  digitalWrite(BEEPER_PIN, HIGH);
  playWakeUp();
}

bool Beeper::cycleBeeper() {
  if (millis() - lastBeep >= beepDelays[beepIndex]) {
    beep(beepFrequencies[beepIndex], beepDurations[beepIndex]);
    lastBeep = millis();
    beepIndex++;
    if (beepIndex >= seqLength) {
      beepIndex = 0;
      return true;
    }
  }
  return false;
}
