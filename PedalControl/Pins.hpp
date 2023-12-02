#pragma once

#define DATA  0
#define CLOCK 1
#define LATCH 2

/* Switches and swellers */

// Footstack:             1 byte
#define SWITCHES_BYTES    1
extern const int PINS_SWITCHES[3];

#define NO_SWELLERS       2
#define PIN_SWELLER1      A0
#define PIN_SWELLER2      A1