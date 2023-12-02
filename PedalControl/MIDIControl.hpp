#pragma once

#include "Commons.hpp"

void initMidi();

void sendPedalNote(byte note, bool on); // MIDI note on/off, channel 1
void sendPiston(byte number, bool on);  // MIDI note on/off, channel 2
                                            // Note 16-23: Footstack pistons
                                            // Note 24-55: Left stops
                                            // Note 56-87: Right stops
void sendSweller1Value(byte value);     // MIDI foot pedal, channel 1
void sendSweller2Value(byte value);     // MIDI foot pedal, channel 2
