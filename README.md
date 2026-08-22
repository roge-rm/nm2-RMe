# nm2-RMe
##### alternate firmware for the NM2 by this.is.NOISE

nm2-RMe is a scaled down alternative firmware for the NM2 MIDI controller that offers a different experience - still 18 buttons assigned to scales but everything is configured on device using key combinations and a pair of very expressive LEDs.

A few things that differ:
- there is no app to configure anything, everything is done on device
- you can save to and load from 12 presets to quickly configure the device
- the encoders control volume (left) and octave (right), making it easier to use for note entry into your favourite hardware or software DAW
- chords! there are four chord modes to choose from, described below
- the X/Y sensors currently are not assigned to anything (but may be in the future)

### How to use

#### On boot

Press button 1 to turn on the device.

On boot the device will blink a few times, waiting for you to choose from one of the 12 presets (buttons 1-12, starting from the top left) - or button 18 (bottom right) to set the device up. Selecting a preset will load the MIDI channel, scale, root note, chord mode, and chord voicing (the last two are optional) automatically so you can use it right away.

If you press button 18 you will be run through a quick setup:
1. First you are prompted (by a few blinks) to choose the MIDI channel. Use buttons 1-16 to do so.
2. Then you will be prompted to choose your desired scale. Buttons 1-16 will choose from the following scales: Major, Natural Minor, Melodic Minor, Hamonic Minor, Dorian, Phrygian, Lydian, Mixolydian, Locrian, Minor Pentatonic, Major Pentatonic, Major Blues, Minor Blues, Augmented, Diminished, or No Scale
3. Lastly you will be prompted to select your root note. Starting with C in the top left corner, press buttons 1-18 (each goes up one semitone) to select your root note.

The device will then launch and initiate looking for a BLE connection. USB MIDI will work immediately. 

You can now enter notes freely. The left encoder controls the volume (sent when you press a button) and the right encoder contols the octave. Turning either will show a 'display' using the two sets of three LEDs between the encoders.

Button 18 can be held for 5 seconds to turn off the device.

#### Alternative functions

Turning the left encoder until the volume is at 0 (the LEDs will show you) unlocks a few alternate button functions when held for some time. Hold one of the following buttons for 1 second to perform:

* Button 1: change MIDI channel - you can select the MIDI channel like in the setup step above
* Button 3: enable chord only mode - 7ths (see below)
* Button 4: enable chord + note mode - 7ths (see below)
* Button 5: enable chord only mode, triads (see below)
* Button 6: enable chord + note mode, triads (see below)
* Button 7: change scale (see scale setup step above)
* Button 8: change root note (see root note setup step above)

#### Chord Modes

There are two chord modes, twice, offered in either 7th or triad mode:
*Chords only - all rows play chords only, going up an octave each row
*Chords + notes - only the bottom row plays chords and you can still play singular notes on the top two rows

### Installation

Remove the four face screws from your NM2, remove the faceplate and then gently lift the main board out of the housing - be careful about the battery cable. On the back of the device you will see two switches labelled SW1 and SW2.

Ensure your device is on and connect a USB C cable to the USB C port from your computer. While holding SW2, briefly press SW1. The LEDs will turn off, indicating your device is now in flash mode. Flash the firmware. 

-- I'll put more flashing instructions here later --