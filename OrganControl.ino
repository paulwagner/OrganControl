#include "Commons.hpp"
#include "MIDIControl.hpp"
#include "PedalControl.hpp"
#include "SwitchesInput.hpp"
#include "LEDsOutput.hpp"

// For debugging
int testRow = 0;

void setup() {
  initLEDs();
  
  initSwitches();
  initPedal();
  initMidi();

#ifdef PRINT_OUTPUT
  Serial.begin(19200);
#endif
}

void loop() {
  sendAllSwitches();
  sendPedalNotes();
  checkMidiData();
  
#ifndef PRINT_OUTPUT
  // Delay a little to keep the load down.
  delay(1);
#else
  testLEDRow(testRow, 0xFF);
  testRow = (testRow + 1) % 5; // Only 5 digits
  delay(500);
#endif
}
