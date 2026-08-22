#include <Arduino.h>
#include <ESP32_Host_MIDI.h> // midiHandler
#include <USB.h>             // USB.begin()

#include "config.h"
#include "globals.h"
#include "functions.h"

void setup()
{
  initPower(); // release RTC IO from deep sleep, configure charge pin

  presets.begin("nm2", false); // initiate nm2 namespace to store/retrieve

  ble.begin(BLENAME);
  midiHandler.addTransport(&ble);
  midiHandler.begin();

  usbMIDI.begin();
  USB.begin();

  initLEDs();
  initButtons();

  // setup ESP sleep mode
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

  if (cause == ESP_SLEEP_WAKEUP_EXT0)
  {
    digitalWrite(ledLeft_Red, HIGH);
    digitalWrite(ledLeft_Green, HIGH);
    digitalWrite(ledLeft_Blue, HIGH);
    digitalWrite(ledRight_Red, HIGH);
    digitalWrite(ledRight_Green, HIGH);
    digitalWrite(ledRight_Blue, HIGH);
    chargeStatus();
    enterSleep();
  }

  if (cause == ESP_SLEEP_WAKEUP_GPIO)
  {
    ESP.restart();
  }

  delay(100);
  flashLEDs(1);

  initEncoders();

  updateButtons();
  updateEncoders();

  currentOctave = 2;
  velocityValue = DEFAULTVELOCITY;

  selectPreset(); // choose one of the available presets or initiate setup mode
  setNotes();     // set initial note values

  ledTimer = millis();
  ledCountTimer = millis();
  flashLEDs(1);
}

void loop()
{
  static bool deviceConnected = false; // track whether the BLE central is connected
  const int ledTime = 1500;            // LED cycle time in ms
  const int ledCountTime = 1500;       // LED counting display hold time

  midiHandler.task(); // service BLE transport

  if (!deviceConnected && blinkTick(ledTime))
    writeLED(ledRight_Blue, ledState); // flash blue LED if BLE is not connected

  if ((ledCountDown) && ((millis() - ledCountTimer) > ledCountTime))
  { // if LED count display has been up for long enough go back to normal status display mode
    flashLEDs(0);
    ledCountTimer = millis();
    ledCountDown = false;
  }
  else if (!ledCountDown)
  { // update status LEDs if LEDs aren't being used as a count display
    if (deviceConnected)
      writeLED(ledRight_Blue, true); // show BLE connection
    chargeStatus();                  // show charge status
  }
  updateEncoders(); // check encoders for updates
  updateButtons();  // check for button presses
  actionButtons();  // process button presses and queue MIDI messages

  chargeStatus();
  deviceConnected = ble.isConnected(); // check for BLE connection

  if ((millis() - idleTime) > (SLEEPTIME * 60000))
    enterSleep(); // sleep if device idle for longer than SLEEPTIME minutes
}
