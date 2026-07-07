#include <Arduino.h>
#include "beeper.h"
#include "consts.h"

unsigned long lastBeep = 0;
int arrayLength = sizeof(beepDelays) / sizeof(beepDelays[0]);
int beepIndex = 0;

void Beeper::beep(unsigned int frequency, unsigned int duration) {
  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.print(" Duration: ");
  Serial.println(duration);
  tone(BEEPER_PIN, frequency, duration);
  delay(duration);
  noTone(BEEPER_PIN);
  digitalWrite(BEEPER_PIN, HIGH);
}

void Beeper::setup() {
  gpio_hold_dis((gpio_num_t)BEEPER_PIN);
  pinMode(BEEPER_PIN, OUTPUT);
  digitalWrite(BEEPER_PIN, HIGH);
  beep(1000, 100);
  beep(2000, 50);
}

void Beeper::cycleBeeper() {
  if (lastBeep == 0) beep(beepFrequencies[0], beepDurations[0]);
  if (millis() - lastBeep >= beepDelays[beepIndex]) {
    beep(beepFrequencies[beepIndex], beepDurations[beepIndex]);
    lastBeep = millis();
    beepIndex++;
    if (beepIndex >= arrayLength) beepIndex = 0;
  }
}