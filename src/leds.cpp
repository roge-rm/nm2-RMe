#include <Arduino.h>

#include "globals.h"
#include "functions.h"

void initLEDs()
{
  pinMode(ledLeft_Red, OUTPUT);
  pinMode(ledLeft_Green, OUTPUT);
  pinMode(ledLeft_Blue, OUTPUT);
  pinMode(ledRight_Red, OUTPUT);
  pinMode(ledRight_Green, OUTPUT);
  pinMode(ledRight_Blue, OUTPUT);
}

void writeLED(uint8_t pin, bool on)
{ // the LEDs are wired active-low: HIGH is off, LOW is on
  digitalWrite(pin, on ? LOW : HIGH);
}

bool blinkTick(unsigned long intervalMs)
{ // toggles the shared ledState/ledTimer blink clock once per intervalMs; returns true on the tick it flips.
  // Used by every "blink an LED while waiting for a button press" loop, since they never run concurrently.
  if (millis() <= (ledTimer + intervalMs))
    return false;
  ledState = !ledState;
  ledTimer = millis();
  return true;
}

void flashLEDs(int flashes)
{ // flash the left/right bank of LEDs alternately as a way to communicate confirmations and prompts
  for (int i = 0; i < flashes; i++)
  {
    writeLED(ledLeft_Green, true);
    writeLED(ledLeft_Red, true);
    writeLED(ledLeft_Blue, true);
    writeLED(ledRight_Green, false);
    writeLED(ledRight_Red, false);
    writeLED(ledRight_Blue, false);
    delay(65);
    writeLED(ledLeft_Green, false);
    writeLED(ledLeft_Red, false);
    writeLED(ledLeft_Blue, false);
    writeLED(ledRight_Green, true);
    writeLED(ledRight_Red, true);
    writeLED(ledRight_Blue, true);
    delay(65);
  }
  writeLED(ledLeft_Green, false);
  writeLED(ledLeft_Red, false);
  writeLED(ledLeft_Blue, false);
  writeLED(ledRight_Green, false);
  writeLED(ledRight_Red, false);
  writeLED(ledRight_Blue, false);
}

void countLEDs(int count)
{ // use the left/right bank of three LEDs each as a 6-LED bargraph: 0-6 fills left-to-right,
  // 6-10 then empties the same way, so e.g. count=8 leaves the last 4 LEDs (of 6) lit
  uint8_t leds[6] = {ledLeft_Blue, ledLeft_Red, ledLeft_Green, ledRight_Blue, ledRight_Red, ledRight_Green};

  for (int i = 0; i < 6; i++)
  {
    bool on = (count <= 6) ? (i < count) : (i >= (count - 6));
    writeLED(leds[i], on);
  }

  ledCountDown = true;
  ledCountTimer = millis();
}
