#include <Arduino.h>
#include "driver/rtc_io.h"
#include "soc/rtc_cntl_reg.h"

#include "config.h"
#include "globals.h"
#include "functions.h"

static bool chargeState; // last-read charge/USB-power state, shown on the left red LED

void initPower()
{
  rtc_gpio_deinit(GPIO_NUM_8); // release chargePin from RTC control in case we just woke from deep sleep
  pinMode(chargePin, INPUT_PULLUP);
}

void chargeStatus()
{ // read charge status from charging board and display with left red LED
  chargeState = digitalRead(chargePin);
  writeLED(ledLeft_Red, !chargeState);
}

void enterSleep()
{
  flashLEDs(2);
  chargeStatus(); // refresh charge state in case the caller's reading is stale

  if (!chargeState)
  {
    rtc_gpio_pullup_en(GPIO_NUM_8);
    rtc_gpio_pulldown_dis(GPIO_NUM_8);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_8, HIGH);
  }
  else
  {
    rtc_gpio_pullup_en(GPIO_NUM_8);
    rtc_gpio_pulldown_dis(GPIO_NUM_8);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_8, LOW);
  }

  gpio_wakeup_enable(GPIO_NUM_1, GPIO_INTR_HIGH_LEVEL); // set button 1 (pin 1) to wake device
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  esp_sleep_enable_gpio_wakeup();

  esp_deep_sleep_start(); // go to deep sleep - never returns; waking is a full chip reset back through setup()
}

void rebootToBootloader()
{ // forces the ROM bootloader into USB/UART download mode on the next boot (bypassing GPIO0
  // strapping), so the device can be reflashed without opening the case
  flashLEDs(2);
  REG_WRITE(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT);
  esp_restart(); // never returns
}
