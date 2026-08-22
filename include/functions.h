#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// power.cpp
void initPower();
void enterSleep();
void chargeStatus();

// leds.cpp
void initLEDs();
void writeLED(uint8_t pin, bool on);
bool blinkTick(unsigned long intervalMs);
void flashLEDs(int flashes);
void countLEDs(int count);

// input.cpp
void initButtons();
void initEncoders();
void updateButtons();
void updateEncoders();
void actionButtons();
int buttonChoice();

// notes.cpp
void setNotes();
void setupScale(bool skipsetup);

// midi.cpp
void midiON(int bNum);
void midiONChord(int bNum, int degree);
void midiOFF(int bNum);

// presets.cpp
void selectPreset();
void setupMode();
void setupMIDIChan();
void setupRoot();
void savePreset();
void storePrefs(int presetNum);
void recallPrefs(int presetNum);

#endif // FUNCTIONS_H
