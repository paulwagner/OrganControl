#pragma once

#define DATA  0
#define CLOCK 1
#define LATCH 2

/* Switches and swellers */

// Keystack:              2 bytes
// Footstack:             1 byte
// Switches L:            4 byte
// Switches R:            4 byte
#define SWITCHES_BYTES    3

#define NO_SWELLERS       2
#define PIN_SWELLER1      A0
#define PIN_SWELLER2      A1

extern const int PINS_SWITCHES[3];


/* LEDs */

// Keystack:          1 IC
// Left stops         1 IC
// Right stops:       1 IC
#define LED_NR_ICS    1

extern const int PINS_LEDS[3];
