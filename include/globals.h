#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Preferences.h>
#include <BLEConnection.h>
#include <USBMIDI.h>

// persistent objects
extern Preferences presets;
extern BLEConnection ble; // ESP32_Host_MIDI BLE transport
extern USBMIDI usbMIDI;   // native USB MIDI device

// pin assignments
extern uint8_t ledLeft_Red;
extern uint8_t ledLeft_Green;
extern uint8_t ledLeft_Blue;
extern uint8_t ledRight_Red;
extern uint8_t ledRight_Green;
extern uint8_t ledRight_Blue;
extern uint8_t buttonPins[18];
extern uint8_t chargePin; // charge indicator is shown on this pin, high for charged
extern uint8_t encoderPinA[2];
extern uint8_t encoderPinB[2];

// MIDI / scale state, shared between the input, notes, midi and presets modules
extern uint8_t midiChan;
extern uint8_t valScale;
extern uint8_t valRoot;
extern uint8_t velocityValue;
extern uint8_t currentOctave;
extern uint8_t chordMode;    // 0 = notes only, 1 = chords only (bottom row plays chords, top rows silent), 2 = chord + note (bottom row plays chords, top rows still play notes)
extern uint8_t chordVoicing; // 0 = triads (root+3rd+5th), 1 = seventh chords (root+3rd+5th+7th)

extern uint8_t buttonNotes[18]; // currently assigned button note
extern bool buttonIsOn[18];     // whether each button's base note is currently sounding

// LED blink state, shared between the status LEDs in loop() and the setup-menu
// prompts in presets.cpp, which all reuse the same blink timer
extern unsigned long ledTimer;
extern unsigned long ledCountTimer;
extern bool ledState;
extern bool ledCountDown; // true while the LEDs are showing a count display instead of status

extern unsigned long idleTime; // millis() timestamp of the last user interaction

#endif // GLOBALS_H
