#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h> // https://github.com/thomasfredericks/Bounce2

#include "config.h"
#include "globals.h"
#include "functions.h"

// bounce objects for each of the 18 buttons
static Bounce2::Button buttons[18];

void initButtons()
{
  for (int i = 0; i < 18; i++)
  {
    buttons[i].attach(buttonPins[i], INPUT);
    buttons[i].interval(5); // button debounce interval in ms
  }
}

void initEncoders()
{
  pinMode(encoderPinA[0], INPUT_PULLUP);
  pinMode(encoderPinB[0], INPUT_PULLUP);
  pinMode(encoderPinA[1], INPUT_PULLUP);
  pinMode(encoderPinB[1], INPUT_PULLUP);
}

void updateButtons()
{ // poll each button for state updates
  for (int i = 0; i < 18; i++)
    buttons[i].update();
}

void updateEncoders()
{
  static int MSB[2] = {0};
  static int LSB[2] = {0};
  static int encoded[2] = {0};
  static int sum[2] = {0};
  static int lastReportedPos[2] = {-1};
  static volatile int encoderPosCount[2] = {DEFAULTVELOCITY, 30};
  static int lastEncoded[2] = {0};

  for (int b = 0; b < 2; b++)
  { // read values from rotary encoders

    MSB[b] = digitalRead(encoderPinA[b]);
    LSB[b] = digitalRead(encoderPinB[b]);

    encoded[b] = (MSB[b] << 1) | LSB[b];         // Converting the 2 pin value to single number
    sum[b] = (lastEncoded[b] << 2) | encoded[b]; // Adding it to the previous encoded value

    if (sum[b] == 0b1101 || sum[b] == 0b0100 || sum[b] == 0b0010 || sum[b] == 0b1011)
      encoderPosCount[b]++;
    if (sum[b] == 0b1110 || sum[b] == 0b0111 || sum[b] == 0b0001 || sum[b] == 0b1000)
      encoderPosCount[b]--;

    lastEncoded[b] = encoded[b]; // Store this value for next time

    if (encoderPosCount[b] > 127)
    {
      encoderPosCount[b] = 127;
    }

    if (encoderPosCount[b] < 0)
    {
      encoderPosCount[b] = 0;
    }
  }

  if (encoderPosCount[0] != lastReportedPos[0])
  {                                      // if left encoder has changed
    velocityValue = encoderPosCount[0]; // update velocity value accordingly
    int velocityDisplay;
    switch (velocityValue)
    {
    case 0:
      velocityDisplay = 0;
      break;
    case 1 ... 11:
      velocityDisplay = 1;
      break;
    case 12 ... 25:
      velocityDisplay = 2;
      break;
    case 26 ... 38:
      velocityDisplay = 3;
      break;
    case 39 ... 51:
      velocityDisplay = 4;
      break;
    case 52 ... 64:
      velocityDisplay = 5;
      break;
    case 65 ... 77:
      velocityDisplay = 6;
      break;
    case 78 ... 90:
      velocityDisplay = 7;
      break;
    case 91 ... 103:
      velocityDisplay = 8;
      break;
    case 104 ... 116:
      velocityDisplay = 9;
      break;
    case 117 ... 127:
      velocityDisplay = 10;
      break;
    }
    countLEDs(velocityDisplay);
    lastReportedPos[0] = encoderPosCount[0];
    idleTime = millis(); // reset idle time every time the knob is touched
  }

  if (encoderPosCount[1] != lastReportedPos[1])
  { // if right encoder is changed
    uint8_t newOctave;
    switch (encoderPosCount[1])
    {
    case 0 ... 16:
      newOctave = 0;
      break;
    case 17 ... 32:
      newOctave = 1;
      break;
    case 33 ... 48:
      newOctave = 2;
      break;
    case 49 ... 64:
      newOctave = 3;
      break;
    case 65 ... 80:
      newOctave = 4;
      break;
    case 81 ... 96:
      newOctave = 5;
      break;
    case 97 ... 112:
      newOctave = 6;
      break;
    case 113 ... 127:
      newOctave = 7;
      break;
    }
    if (currentOctave != newOctave)
    { // change the octave and call for a note update
      currentOctave = newOctave;
      setNotes();
      countLEDs(newOctave);
    }
    lastReportedPos[1] = encoderPosCount[1];
    idleTime = millis();
  }
}

void actionButtons()
{ // perform actions based on buttons pressed/released or held
  for (int i = 0; i < 18; i++)
  {
    // always release a sounding note on button release, even if velocity was zeroed after the note started,
    // otherwise the note hangs (it would only be caught below if velocityValue is still > 0 at release time)
    if (buttons[i].fell() && buttonIsOn[i])
      midiOFF(i);
  }

  if (velocityValue > 0)
  { // read buttons and send corresponding MIDI messages
    for (int i = 0; i < 18; i++)
    {
      if (buttons[i].rose())
      {
        if (i < 12)
        {
          if (chordMode != 1) // chords-only mode silences the top two rows' single notes
            midiON(i);
        }
        else if (chordMode == 0)
          midiON(i); // no chord mode active: bottom row plays plain notes like the rest
        else
          midiONChord(i, i - 12); // button 13 -> scale degree 1, button 14 -> degree 2, ... button 18 -> degree 6
      }
    }
  }
  else
  { // some buttons have second functions when the velocity is turned to 0 and they are held for BUTTONHOLDTIME ms
    if (buttons[12].released() && (buttons[12].previousDuration() > BUTTONHOLDTIME))
    {              // button 13 saves preset
      midiOFF(12); // turn off any notes sent when button was held
      savePreset();
    }
    if (buttons[0].released() && (buttons[0].previousDuration() > BUTTONHOLDTIME))
    { // button 1 changes MIDI channel
      flashLEDs(1);
      setupMIDIChan();
      setNotes();
    }
    if (buttons[2].released() && (buttons[2].previousDuration() > BUTTONHOLDTIME))
    { // button 3 enables chord only mode, seventh chords (bottom row plays diatonic 7th chords, top rows silent)
      flashLEDs(5);
      bool active = (chordMode == 1 && chordVoicing == 1);
      chordMode = active ? 0 : 1;
      chordVoicing = 1;
    }
    if (buttons[3].released() && (buttons[3].previousDuration() > BUTTONHOLDTIME))
    { // button 4 enables chord + note mode, seventh chords (bottom row plays diatonic 7th chords, top rows still play notes)
      flashLEDs(6);
      bool active = (chordMode == 2 && chordVoicing == 1);
      chordMode = active ? 0 : 2;
      chordVoicing = 1;
    }
    if (buttons[4].released() && (buttons[4].previousDuration() > BUTTONHOLDTIME))
    { // button 5 enables chord only mode, triads (bottom row plays diatonic triads, top rows silent)
      flashLEDs(4);
      bool active = (chordMode == 1 && chordVoicing == 0);
      chordMode = active ? 0 : 1;
      chordVoicing = 0;
    }
    if (buttons[5].released() && (buttons[5].previousDuration() > BUTTONHOLDTIME))
    { // button 6 enables chord + note mode, triads (bottom row plays diatonic triads, top rows still play notes)
      flashLEDs(3);
      bool active = (chordMode == 2 && chordVoicing == 0);
      chordMode = active ? 0 : 2;
      chordVoicing = 0;
    }
    if (buttons[6].released() && (buttons[6].previousDuration() > BUTTONHOLDTIME))
    { // button 7 changes scale
      flashLEDs(1);
      setupScale(false);
      setNotes();
    }
    if (buttons[7].released() && (buttons[7].previousDuration() > BUTTONHOLDTIME))
    { // button 8 changes root note
      flashLEDs(1);
      setupRoot();
      setNotes();
    }
  }

  if (buttons[17].isPressed() && (buttons[17].currentDuration() > SLEEPBUTTONHOLDTIME))
  {              // sleep function only requires button 18 is held for SLEEPBUTTONTIME ms
    midiOFF(17); // turn off any notes sent when button was held
    enterSleep();
  }

  for (int i = 0; i < 18; i++)
  {
    if (buttons[i].changed())
    {
      idleTime = millis();
      break;
    }
  }
}

int buttonChoice()
{
  updateButtons();

  for (int i = 0; i < 18; i++)
    if (buttons[i].released())
      return i;

  return -1;
}
