#include "Commons.hpp"
#include "MIDIControl.hpp"
#include "PedalControl.hpp"
#include "SwitchesInput.hpp"

void setup() {
  Serial.begin(115200);
#ifdef PRINT_MIDI
  while(!Serial);
#endif

  initSwitches();
  initPedal();
}

void loop() {
  sendAllSwitches();
  sendPedalNotes();
}
