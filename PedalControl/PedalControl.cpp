#include "PedalControl.hpp"

#include "MidiControl.hpp"

#define GET_NOTE(m,p) (36 + (8*m) + p)

#define PINS_MINUS_SIZE 4
static int pinsMinus[] = {47, 49, 51, 53};

#define PINS_PLUS_SIZE 8
static int pinsPlus[] = {31, 33, 35, 37, 39, 41, 43, 45};

static bool noteIsDown[4][8];
static bool lastKeyState[4][8];
unsigned long lastDebounceTimes[4][8];
const unsigned long debounceDelay = 1; // in ms

void initPedal() {
  memset(noteIsDown, false, sizeof(noteIsDown));
  memset(lastKeyState, false, sizeof(lastKeyState));
  memset(lastDebounceTimes, 0, sizeof(lastDebounceTimes));
  
  for(int p = 0; p < PINS_PLUS_SIZE; p++) {
    pinMode(pinsPlus[p], OUTPUT);
    digitalWrite(pinsPlus[p], LOW);
  }
  for(int m = 0; m < PINS_MINUS_SIZE; m++) {
    pinMode(pinsMinus[m], INPUT);
  }
}

inline void readNote(int m, int p) {
  digitalWrite(pinsPlus[p], HIGH);
  delayMicroseconds(2); // Wait for stable signal

  bool down = (digitalRead(pinsMinus[m]) == HIGH);
  if(down != lastKeyState[m][p]) {
    lastDebounceTimes[m][p] = millis();
  }
  lastKeyState[m][p] = down;

  if((millis() - lastDebounceTimes[m][p]) > debounceDelay) {
    if(down && !noteIsDown[m][p]) {
      sendPedalNote(GET_NOTE(m,p), true);
      noteIsDown[m][p] = true;
    } else if(!down && noteIsDown[m][p]) {
      sendPedalNote(GET_NOTE(m,p), false);
      noteIsDown[m][p] = false;
    }
  }

  digitalWrite(pinsPlus[p], LOW);
}

void sendPedalNotes() {
  for (int p = 0; p < PINS_PLUS_SIZE; p++) {
    for (int m = 0; m < PINS_MINUS_SIZE; m++) {
      if(m == 3 && p >= 6)
        continue; // Last quadrant only has 6 notes
      readNote(m,p);
      delayMicroseconds(10); // Prevent cross talk
    }
  }
}
