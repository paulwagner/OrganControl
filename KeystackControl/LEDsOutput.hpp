#pragma once

#include "Commons.hpp"

void initLEDs();
void clearLEDs();
void setLEDOutput(int number, bool on);     // Numbers equivalent to MIDI notes for switches
                                            // Number 0-15: Keystack pistons
void set7SegsOutput(int digit, byte character);

void testLEDs(int testDigit, int testColumn);
