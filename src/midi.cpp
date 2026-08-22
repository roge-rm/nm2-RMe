#include <Arduino.h>
#include <ESP32_Host_MIDI.h>

#include "globals.h"

static uint8_t buttonPlayed[4][18]; // what note was played by each button last (in case the page of notes is changed while a note is being played; to prevent hung notes)

void midiON(int bNum)
{
  buttonIsOn[bNum] = true;
  midiHandler.sendNoteOn(midiChan, buttonNotes[bNum], velocityValue);
  usbMIDI.noteOn(buttonNotes[bNum], velocityValue, midiChan);
  buttonPlayed[0][bNum] = buttonNotes[bNum];
  buttonPlayed[1][bNum] = 0;
  buttonPlayed[2][bNum] = 0;
  buttonPlayed[3][bNum] = 0;
}

// plays the diatonic chord built on the given scale degree (0-5, i.e. buttons map to degrees
// 1-6, skipping the scale's top degree), stacked in thirds directly off the scale ladder in
// buttonNotes[] so it automatically follows whatever scale/root/octave is active. octaveShift
// (in semitones, e.g. +/-12 per octave) transposes the whole chord for the row-spread in
// chords-only mode, without needing a second scale ladder.
void midiONChord(int bNum, int degree, int octaveShift)
{
  buttonIsOn[bNum] = true;
  uint8_t root = buttonNotes[degree] + octaveShift;
  uint8_t third = buttonNotes[degree + 2] + octaveShift;
  uint8_t fifth = buttonNotes[degree + 4] + octaveShift;

  midiHandler.sendNoteOn(midiChan, root, velocityValue);
  usbMIDI.noteOn(root, velocityValue, midiChan);
  buttonPlayed[0][bNum] = root;

  midiHandler.sendNoteOn(midiChan, third, velocityValue);
  usbMIDI.noteOn(third, velocityValue, midiChan);
  buttonPlayed[1][bNum] = third;

  midiHandler.sendNoteOn(midiChan, fifth, velocityValue);
  usbMIDI.noteOn(fifth, velocityValue, midiChan);
  buttonPlayed[2][bNum] = fifth;

  if (chordVoicing == 1)
  {
    uint8_t seventh = buttonNotes[degree + 6] + octaveShift;
    midiHandler.sendNoteOn(midiChan, seventh, velocityValue);
    usbMIDI.noteOn(seventh, velocityValue, midiChan);
    buttonPlayed[3][bNum] = seventh;
  }
  else
    buttonPlayed[3][bNum] = 0;
}

void midiOFF(int bNum)
{
  buttonIsOn[bNum] = false;
  midiHandler.sendNoteOff(midiChan, buttonPlayed[0][bNum], 0); // base note is always played in midiON, so always turn it off
  usbMIDI.noteOff(buttonPlayed[0][bNum], 0, midiChan);
  if ((buttonPlayed[1][bNum]) > 0)
  {
    midiHandler.sendNoteOff(midiChan, buttonPlayed[1][bNum], 0);
    usbMIDI.noteOff(buttonPlayed[1][bNum], 0, midiChan);
  }
  if ((buttonPlayed[2][bNum]) > 0)
  {
    midiHandler.sendNoteOff(midiChan, buttonPlayed[2][bNum], 0);
    usbMIDI.noteOff(buttonPlayed[2][bNum], 0, midiChan);
  }
  if ((buttonPlayed[3][bNum]) > 0)
  {
    midiHandler.sendNoteOff(midiChan, buttonPlayed[3][bNum], 0);
    usbMIDI.noteOff(buttonPlayed[3][bNum], 0, midiChan);
  }
}
