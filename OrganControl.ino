#include "Commons.hpp"
#include "MIDIControl.hpp"
#include "PedalControl.hpp"
#include "SwitchesInput.hpp"
#include "LEDsOutput.hpp"

#ifdef TEST_LEDS
  int testDigit = 0;
  int testColumn = 0;
  const long testDelay = 300; // in ms
  long lastTest = 0;
#endif

void setup() {
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
  if((millis() - lastTest) > testDelay) {
    testDigit = (testDigit + 1) % 10;
    testColumn = (testColumn + 1) % 8;
    testLEDs(testDigit, testColumn);
    lastTest = millis();
  }
#endif
}
