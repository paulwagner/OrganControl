#include "Commons.hpp"
#include "LEDsOutput.hpp"
#include "MIDIControl.hpp"
#include "SwitchesInput.hpp"

#ifdef TEST_LEDS
  int testDigit = 0;
  int testColumn = 0;
  const long testDelay = 300; // in ms
  long lastTest = 0;
#endif

void setup() {
  Serial.begin(115200);
  while(!Serial);

  initLEDs();
  initSwitches();
  initMidi();
}

void loop() {
  sendAllSwitches();
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
