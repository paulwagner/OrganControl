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

// Sent bytes for swellers
byte valuesSwellersSent[NO_SWELLERS];
// Sweller threshold
const int thresholdSweller = 4;

void initSwitches() {
  pinMode(PINS_SWITCHES[DATA], INPUT);
  pinMode(PINS_SWITCHES[CLOCK], OUTPUT);
  pinMode(PINS_SWITCHES[LATCH], OUTPUT);

  pinMode(PIN_SWELLER1, INPUT);
  pinMode(PIN_SWELLER2, INPUT);

  memset(valuesSwitches, 0x0, sizeof(valuesSwitches));
  memset(valuesSwellersSent, 0x0, sizeof(valuesSwellersSent));
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

  // Poll all analog values
  int sweller1_i = analogRead(PIN_SWELLER1);
  byte sweller1 = (byte) max(0, min(127, map(sweller1_i, 280, 1005, 0, 127)));
  int sweller2_i = analogRead(PIN_SWELLER2);
  byte sweller2 = (byte) max(0, min(127, map(sweller2_i, 300, 965, 0, 127)));

  // Send sweller values
  if(abs((int)sweller1 - (int)valuesSwellersSent[0]) > thresholdSweller) {
    //Serial.println(sweller1_i);
    sendSweller1Value(sweller1);
    valuesSwellersSent[0] = sweller1;
  }
  if(abs((int)sweller2 - (int)valuesSwellersSent[1]) > thresholdSweller) {
    //Serial.println(sweller2_i);
    sendSweller2Value(sweller2);
    valuesSwellersSent[1] = sweller2;
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
