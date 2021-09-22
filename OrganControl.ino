#include "Commons.hpp"
#include "MIDIControl.hpp"
#include "PedalControl.hpp"
#include "SwitchesInput.hpp"
#include "LEDsOutput.hpp"

#ifdef TEST_LEDS
  int testRow = 0;
  const long testDelay = 500; // in ms
  long lastTimestamp = 0;
#endif

void setup() {
  Serial.begin(19200);

  initLEDs();
  initSwitches();
  initPedal();
  initMidi();
}

void loop() {
  sendAllSwitches();
  sendPedalNotes();
  checkMidiData();
  
#ifdef TEST_LEDS
  if((millis() - lastTimestamp) > testDelay) {
    testRow = (testRow + 1) % 5; // Only 5 digits
    testLEDRow(testRow, 0xFF);
    lastTimestamp = millis();
  }
#endif
}
