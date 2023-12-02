#include "LEDsOutput.hpp"

#include "Pins.hpp"
#include <LedControl.h>

LedControl leds = LedControl(PINS_LEDS[DATA], PINS_LEDS[CLOCK], PINS_LEDS[LATCH], LED_NR_ICS);

void initLEDs() {
  // Keystack IC uses 5 digits with high intensity
  leds.setScanLimit(0,5);
  leds.setIntensity(0,15);
  clearLEDs();
}

void clearLEDs() {
  for(int i = 0; i < LED_NR_ICS; i++) {
    leds.clearDisplay(i);
    leds.shutdown(i, false);
  }
}

void setLEDOutput(int number, bool on) {
  if(number < 0 || number > 15)
    return; // Invalid LED number
  
  int row = number / 8;
  int col = number % 8;
  leds.setLed(0, row, col, on);
}

void set7SegsOutput(int digit, byte character) {
  leds.setChar(0, digit + 2, character, false);
}

void testLEDs(int testDigit, int testColumn) {
  clearLEDs();
  // Key stack
  for(int row = 0; row < 2; row++)
    leds.setLed(0, row, testColumn, true);
  // 7Segs
  for(int row = 2; row < 5; row++)
    leds.setDigit(0, row, testDigit, false);
}
