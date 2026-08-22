#include "globals.h"
#include "config.h"

Preferences presets;

BLEConnection ble;
USBMIDI usbMIDI(BLENAME);

// pin assignments
uint8_t ledLeft_Red = 6;
uint8_t ledLeft_Green = 5;
uint8_t ledLeft_Blue = 4;
uint8_t ledRight_Red = 16;
uint8_t ledRight_Green = 15;
uint8_t ledRight_Blue = 7;

uint8_t buttonPins[18] = {1, 2, 42, 12, 45, 38, 41, 40, 39, 37, 36, 35, 21, 14, 13, 47, 48, 46};
uint8_t chargePin = 8;

uint8_t encoderPinA[2] = {18, 17};
uint8_t encoderPinB[2] = {44, 43};

// state variables
uint8_t midiChan = DEFAULTCHAN;
uint8_t valScale = DEFAULTSCALE;
uint8_t valRoot = DEFAULTROOT;
uint8_t velocityValue = DEFAULTVELOCITY; // MIDI velocity value
uint8_t currentOctave;
uint8_t chordMode = 0; // chords, baby!
uint8_t chordVoicing = 0;

uint8_t buttonNotes[18];
bool buttonIsOn[18];

// timer variables
unsigned long ledTimer = 0;
unsigned long ledCountTimer = 0;
bool ledState = false;
bool ledCountDown = false;

unsigned long idleTime = 0;
