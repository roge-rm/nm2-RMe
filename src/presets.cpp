#include <Arduino.h>

#include "config.h"
#include "globals.h"
#include "functions.h"

void selectPreset()
{
  ledTimer = millis();
  int buttonNum = -1;
  bool select = false;
  delay(50);
  chargeStatus();
  while (select == false)
  {
    if (blinkTick(1500))
      writeLED(ledRight_Green, ledState);
    buttonNum = buttonChoice();
    switch (buttonNum)
    {
    case 0 ... 11: // select between presets 0 through 11
      recallPrefs(buttonNum);
      flashLEDs(1);
      select = true;
      break;
    case 17: // initiate full setup
      flashLEDs(1);
      setupMode(); // run selection for channel, root note, scale
      select = true;
      break;
    }
  }
  flashLEDs(1);
}

void setupMode()
{
  updateButtons();
  chargeStatus();
  setupMIDIChan();   // choose MIDI channel
  setupScale(false); // choose scale
  setupRoot();       // choose root note
  delay(50);
  flashLEDs(1);
}

void setupMIDIChan()
{
  ledTimer = millis();
  updateButtons();
  int buttonNum;
  bool select = false;
  while (select == false)
  { // select MIDI channel
    chargeStatus();
    if (blinkTick(1200))
      writeLED(ledRight_Green, ledState);
    buttonNum = buttonChoice();
    if ((buttonNum > -1) && (buttonNum < 16)) // only buttons 1-16 map to a valid MIDI channel
    {
      midiChan = buttonNum + 1; // add one to MIDI value as channel appears to need to be sent as 1-16 instead of 0-15
      select = true;
    }
  }
  flashLEDs(1);
}

void setupRoot()
{
  ledTimer = millis();
  updateButtons();
  flashLEDs(1);
  int buttonNum;
  bool select = false;
  while (select == false)
  { // select root note (blinks out of phase with the other setup prompts, as it always has)
    chargeStatus();
    if (blinkTick(800))
      writeLED(ledRight_Green, !ledState);
    buttonNum = buttonChoice();
    if ((buttonNum > -1) && (buttonNum < 18))
    {
      valRoot = buttonNum;
      select = true;
      delay(50);
      updateButtons();
      buttonNum = -1;
    }
  }
  flashLEDs(1);
}

void savePreset()
{ // save preset to one of 12 preset slots
  updateButtons();
  ledTimer = millis();
  int buttonNum = -1;
  delay(150);
  bool select = false;
  flashLEDs(3);

  while (select == false)
  {
    if (blinkTick(750))
    { // left/right LEDs alternate in opposite phase
      writeLED(ledLeft_Green, ledState);
      writeLED(ledRight_Green, !ledState);
    }
    buttonNum = buttonChoice();
    switch (buttonNum)
    {
    case 0 ... 11: // select between presets 0 through 11
      storePrefs(buttonNum);
      select = true;
      break;
    case 17: // press button 18 row to exit and not save
      select = true;
      break;
    }
  }

  delay(250);
  flashLEDs(3);
  updateButtons();
}

void storePrefs(int presetNum)
{                                            // write current settings to NVS
  String key = String((presetNum * 10) + 1); // convert numerical index to string as NVS keys cannot be integers
  presets.putUInt(key.c_str(), midiChan);

  key = String((presetNum * 10) + 2);
  presets.putUInt(key.c_str(), valScale);

  key = String((presetNum * 10) + 3);
  presets.putUInt(key.c_str(), valRoot);

  key = String((presetNum * 10) + 4);
  presets.putUInt(key.c_str(), chordMode);

  key = String((presetNum * 10) + 5);
  presets.putUInt(key.c_str(), chordVoicing);
}

void recallPrefs(int presetNum)
{ // recall settings from NVS
  String key = String((presetNum * 10) + 1);
  midiChan = presets.getUInt(key.c_str(), DEFAULTCHAN);
  if (!((midiChan > 0) && (midiChan < 13)))
    midiChan = DEFAULTCHAN; // validate channel is between 1 and 12

  key = String((presetNum * 10) + 2);
  valScale = presets.getUInt(key.c_str(), DEFAULTSCALE);
  if (!((valScale > -1) && (valScale < 16)))
    valScale = DEFAULTSCALE; // validate scale is between 0 and 15

  key = String((presetNum * 10) + 3);
  valRoot = presets.getUInt(key.c_str(), DEFAULTROOT);
  if (!((valRoot > -1) && (valRoot < 18)))
    valRoot = DEFAULTROOT; // validate root is between 0 and 17

  key = String((presetNum * 10) + 4);
  chordMode = presets.getUInt(key.c_str(), 0);
  if (chordMode > 2)
    chordMode = 0; // validate chord mode is 0-2

  key = String((presetNum * 10) + 5);
  chordVoicing = presets.getUInt(key.c_str(), 0);
  if (chordVoicing > 1)
    chordVoicing = 0; // validate chord voicing is 0-1

  setupScale(true); // set note values based on scale
}
