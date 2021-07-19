#include "SwitchesInput.hpp"

#include "Pins.hpp"
#include "MIDIControl.hpp"

// Function declarations
void pollDigitalValues(const int pins[3], byte* values, int nrICs);
byte shiftIn(int myDataPin, int myClockPin);

// Flag for current/previous switching
bool _current_prev_toggle = false;
#define CURRENT (_current_prev_toggle ? 0 : 1)
#define PREV (_current_prev_toggle ? 1 : 0)
#define TOGGLE_CURRENT_PREV _current_prev_toggle = !_current_prev_toggle;

// Input and previous bytes for polling of digital switch values
byte valuesSwitches[2][SWITCHES_BYTES];

// Input bytes for swellers
byte valuesSwellers[2][NO_SWELLERS];


void initSwitches() {
  pinMode(PINS_SWITCHES[DATA], INPUT);
  pinMode(PINS_SWITCHES[CLOCK], OUTPUT);
  pinMode(PINS_SWITCHES[LATCH], OUTPUT);

  pinMode(PIN_SWELLER1, INPUT);
  pinMode(PIN_SWELLER2, INPUT);
  analogReadResolution(7); // 0-127

  // Clear all stored values
  for(int i = 0; i < SWITCHES_BYTES; i++) {
    valuesSwitches[CURRENT][i] = 0x0;
    valuesSwitches[PREV][i] = 0x0;
  }
  for(int i = 0; i < NO_SWELLERS; i++) {
    valuesSwellers[CURRENT][i] = 0x0;
    valuesSwellers[PREV][i] = 0x0;
  }
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
  valuesSwellers[CURRENT][0] = (byte) analogRead(PIN_SWELLER1);
  valuesSwellers[CURRENT][1] = (byte) analogRead(PIN_SWELLER2);

  // Send sweller values
  if(valuesSwellers[CURRENT][0] != valuesSwellers[PREV][0])
    sendSweller1Value(valuesSwellers[CURRENT][0]);
  if(valuesSwellers[CURRENT][1] != valuesSwellers[PREV][1])
    sendSweller2Value(valuesSwellers[CURRENT][1]);

  // TODO: Also display sweller values on LCD display
}

void pollDigitalValues(const int pins[3], byte* values, int nrBytes) {
  //Pulse the latch pin:
  //set it to 1 to collect parallel data
  digitalWrite(pins[LATCH],1);
  //set it to 1 to collect parallel data, wait
  delayMicroseconds(20);
  //set it to 0 to transmit data serially  
  digitalWrite(pins[LATCH],0);

  //while the shift register is in serial mode
  //collect each shift register into a byte
  //the register attached to the chip comes in first
  for(int i = 0; i < nrBytes; i++)
    *(values+i) = shiftIn(pins[DATA], pins[CLOCK]);
}

byte shiftIn(int myDataPin, int myClockPin) {
  int i;
  int temp = 0;
  byte myDataIn = 0;

// TODO: Necessary?
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);

  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(2);
    temp = digitalRead(myDataPin);
    if (temp) {
      //set the bit to 0 no matter what
      myDataIn = myDataIn | (1 << i);
    }
    digitalWrite(myClockPin, 1);
  }
  return myDataIn;
}
