#pragma once

#define DATA  0
#define CLOCK 1
#define LATCH 2

/* Switches */

// Keystack:              2 bytes
#define SWITCHES_BYTES    2
extern const int PINS_SWITCHES[3];

/* LEDs */

// Keystack:          1 IC
#define LED_NR_ICS    1
extern const int PINS_LEDS[3];
