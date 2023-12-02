#pragma once

#include "Commons.hpp"

void initMidi();
void checkMidiData();

void sendPiston(byte number, bool on);  // MIDI note on/off, channel 2
                                            // Note 0-15: Keystack pistons

