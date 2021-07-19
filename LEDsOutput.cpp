#include "LEDsOutput.hpp"

#include "Pins.hpp"
#include <LedControl.h>

LedControl leds = LedControl(PINS_LEDS[DATA], PINS_LEDS[CLOCK], PINS_LEDS[LATCH], LED_NR_ICS);

void initLEDs() {
  // Keystack IC uses 5 digits with medium intensity
  leds.setScanLimit(0,5);
  leds.setIntensity(0,10);
  
  // Stop ICs use 4 digits with high intensity
  for(int i = 1; i < LED_NR_ICS; i++) {
    //leds.setScanLimit(i,4);
    //leds.setIntensity(i,15);
  }

  clearLEDs();
}

void clearLEDs() {
  for(int i = 0; i < LED_NR_ICS; i++) {
    leds.clearDisplay(i);
    leds.shutdown(i, false);
  }
}

void setLEDOutput(int number, bool on) {
  int ic = 0; // Keystack
  if(number >= 16 && number <= 23) {
    // Footer pistons -> no output
    return;
  } else if(number >= 24 && number <= 55) {
    ic = 1; // Left stops
    number -= 24;
  } else if(number >= 56 && number <= 87) {
    ic = 2; // Right stops
    number -= 56;
  }
  int row = number / 8;
  int col = number % 8;
  leds.setLed(ic, row, col, on);
}

void set7SegsOutput(int digit, byte character) {
  leds.setChar(0, digit + 2, character, false);
}

void testLEDRow(int row, byte b) {
  for(int i = 0; i < LED_NR_ICS; i++)
    leds.setRow(i, row, b);
}
