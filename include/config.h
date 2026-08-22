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

#endif // CONFIG_H
