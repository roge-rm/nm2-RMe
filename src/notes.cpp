#include <Arduino.h>

#include "globals.h"
#include "functions.h"

// scale interval definitions (semitones between steps)
static uint8_t noteInterval[17];
static const uint8_t scaleMajor[17] = {2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1};
static const uint8_t scaleNaturalMinor[17] = {2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2};
static const uint8_t scaleMelodicMinor[17] = {2, 1, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2, 1, 2};
static const uint8_t scaleHarmonicMinor[17] = {2, 1, 2, 2, 1, 3, 1, 2, 1, 2, 2, 1, 3, 1, 2, 1, 2};
static const uint8_t scaleDorian[17] = {2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2};
static const uint8_t scalePhrygian[17] = {1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2};
static const uint8_t scaleLydian[17] = {2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2};
static const uint8_t scaleMixolydian[17] = {2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1};
static const uint8_t scaleLocrian[17] = {1, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 2};
static const uint8_t scaleMinorPentatonic[17] = {3, 2, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2};
static const uint8_t scaleMajorPentatonic[17] = {2, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 2};
static const uint8_t scaleMajorBlues[17] = {2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2};
static const uint8_t scaleMinorBlues[17] = {3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3};
static const uint8_t scaleAugmented[17] = {3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3};
static const uint8_t scaleDiminished[17] = {2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2};
static const uint8_t scaleNone[17] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// indexed by valScale (0-15), matches notes.txt / the button-select order in setupScale()
static const uint8_t *const scales[16] = {
    scaleMajor, scaleNaturalMinor, scaleMelodicMinor, scaleHarmonicMinor,
    scaleDorian, scalePhrygian, scaleLydian, scaleMixolydian,
    scaleLocrian, scaleMinorPentatonic, scaleMajorPentatonic, scaleMajorBlues,
    scaleMinorBlues, scaleAugmented, scaleDiminished, scaleNone};

void setNotes()
{
  buttonNotes[0] = valRoot + ((currentOctave) * 12); // update this to reflect octave - use right knob and right LEDs to show octave selected
  for (int i = 1; i < 18; i++)
  {
    buttonNotes[i] = buttonNotes[i - 1] + noteInterval[i - 1];
  }
}

void setupScale(bool skipsetup)
{ // bool input to skip selection and assign scale
  ledTimer = millis();
  updateButtons();
  int buttonNum;
  bool select = false;
  if (skipsetup == false)
    flashLEDs(2); // only flash when not loading a preset
  if (skipsetup == true)
    select = true;
  while (select == false)
  { // select scale
    chargeStatus();
    if (blinkTick(1000))
      writeLED(ledRight_Green, ledState);
    buttonNum = buttonChoice();
    if ((buttonNum > -1) && (buttonNum < 16))
    {
      valScale = buttonNum;
      select = true;
    }
  }

  memcpy(noteInterval, scales[valScale], sizeof noteInterval);
  flashLEDs(1);
}
