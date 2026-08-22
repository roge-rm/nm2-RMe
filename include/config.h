#ifndef CONFIG_H
#define CONFIG_H

#define FIRMWARE_VERSION 20260821

// set defaults here
#define DEFAULTROOT 0            // default root note
#define DEFAULTSCALE 0           // default scale (same as below)
#define DEFAULTCHAN 1            // default MIDI channel
#define DEFAULTVELOCITY 100      // default velocity
#define BLENAME "nm2-RMe"        // name for BLE device
#define SLEEPBUTTONHOLDTIME 5000 // number of ms to hold button for sleep
#define BUTTONHOLDTIME 1000      // number of ms to hold button for second function
#define SLEEPTIME 10             // number of minutes to allow idle before going to sleep

#define BUTTON_PIN_BITMASK 0x2

// physical buttons are numbered 1-18 (as printed on the device and used throughout comments),
// but the buttons[]/buttonPins[] arrays are 0-indexed - use these constants instead of a raw
// index whenever a specific button is referenced by number, to avoid off-by-one mix-ups
enum
{
  BTN1 = 0,
  BTN2,
  BTN3,
  BTN4,
  BTN5,
  BTN6,
  BTN7,
  BTN8,
  BTN9,
  BTN10,
  BTN11,
  BTN12,
  BTN13,
  BTN14,
  BTN15,
  BTN16,
  BTN17,
  BTN18
};

#endif // CONFIG_H
