#include "Commons.hpp"
#include "MIDIControl.hpp"
#include "PedalControl.hpp"
#include "SwitchesInput.hpp"
#include "LEDsOutput.hpp"

#include <DueTimer.h>

// For LED testing
int testRow = 0;
void isr_test_led() {
  testRow = (testRow + 1) % 5; // Only 5 digits
}

void setup() {
  Serial.begin(19200);

  initLEDs();
  initSwitches();
  initPedal();
  initMidi();

  Timer6.attachInterrupt(isr_test_led).setFrequency(2).start();
}

void loop() {
  sendAllSwitches();
  sendPedalNotes();
  checkMidiData();

  // Delay a little to keep the load down.
  delay(2);
  
#ifdef TEST_LEDS
  testLEDRow(testRow, 0xFF);
#endif
}
