#include "SwitchesInput.hpp"

#include "Pins.hpp"
#include "MIDIControl.hpp"

// Function declarations
void pollDigitalValues(const int pins[3], byte* values, int nrICs);

// Flag for current/previous switching
bool _current_prev_toggle = false;
#define CURRENT (_current_prev_toggle ? 0 : 1)
#define PREV (_current_prev_toggle ? 1 : 0)
#define TOGGLE_CURRENT_PREV _current_prev_toggle = !_current_prev_toggle;

// Input and previous bytes for polling of digital switch values
byte valuesSwitches[2][SWITCHES_BYTES];

void initSwitches() {
  pinMode(PINS_SWITCHES[DATA], INPUT);
  pinMode(PINS_SWITCHES[CLOCK], OUTPUT);
  pinMode(PINS_SWITCHES[LATCH], OUTPUT);

  memset(valuesSwitches, 0x0, sizeof(valuesSwitches));
}

void sendAllSwitches() {
  TOGGLE_CURRENT_PREV
  
  // Poll all digital switch values
  pollDigitalValues(PINS_SWITCHES, &valuesSwitches[CURRENT][0], SWITCHES_BYTES);

  // Send piston values
  int number = 0;
  for(int i = 0; i < SWITCHES_BYTES; i++) {
    byte currentByte = reverse(valuesSwitches[CURRENT][i]);
    byte prevByte = reverse(valuesSwitches[PREV][i]);
    // Iterate all bits in current byte
    for(int j = 0; j < 8; j++) {
      bool currentState = (bool)(currentByte & (1 << j));
      bool prevState = (bool)(prevByte & (1 << j));
      if(currentState != prevState)
        sendPiston((byte) number, currentState);
      number++;
    }
  }
}

inline byte shiftIn(int dataPin, int clockPin) {
  byte result = 0;
  for (int i = 7; i >= 0; i--) {
    digitalWrite(clockPin, LOW);
    delayMicroseconds(2);
    if(digitalRead(dataPin) == HIGH)
      result = result | (1 << i);
    digitalWrite(clockPin, HIGH);
  }
  return result;
}

void pollDigitalValues(const int pins[3], byte* values, int nrBytes) {
  // Pulse the latch pin
  digitalWrite(pins[LATCH], HIGH);
  delayMicroseconds(20);
  digitalWrite(pins[LATCH], LOW);

  // Collect data
  for(int i = 0; i < nrBytes; i++)
    *(values+i) = shiftIn(pins[DATA], pins[CLOCK]);
}
