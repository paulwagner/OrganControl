#include "PedalControl.hpp"

#include "MidiControl.hpp"

#define GET_NOTE(i,j) (36 + (8*j) + i)

static int PINS_PLUS[] = {22, 24, 26, 28, 30, 32, 34, 36};
static int PINS_PLUS_SIZE = 8;
static int PINS_MINUS[] = {38, 40, 42, 44};
static int PINS_MINUS_SIZE = 4;

static int NOTES_ON_TABLE[8][4];

void initPedal() {
    int i, j;
    for(i = 0; i < PINS_PLUS_SIZE; i++) {
      for(j = 0; j < PINS_MINUS_SIZE; j++) {
        NOTES_ON_TABLE[i][j] = LOW;
      }
    }
    for(i = 0; i < PINS_PLUS_SIZE; i++) {
      pinMode(PINS_PLUS[i], OUTPUT);
      digitalWrite(PINS_PLUS[i], LOW);
    }
    for(j = 0; j < PINS_MINUS_SIZE; j++) {
      pinMode(PINS_MINUS[j], INPUT);
    }
}

void sendPedalNotes() {
  // Loop over the plus poles
  for (int i = 0; i < PINS_PLUS_SIZE; i++) {
    // Set current plus pole to HIGH
    digitalWrite(PINS_PLUS[i], HIGH);
    // Now loop over the minus poles and do the measurements
    for(int j = 0; j < PINS_MINUS_SIZE; j++) {
      if(j == 3 && i >= 6) {
        continue; // 30 note keyboard: no fis' and g'
      }
      
      int m = digitalRead(PINS_MINUS[j]);
      if(m == HIGH && NOTES_ON_TABLE[i][j] == LOW) {
        NOTES_ON_TABLE[i][j] = HIGH;
        sendPedalNote(GET_NOTE(i,j), true);
      } else if(m == LOW && NOTES_ON_TABLE[i][j] == HIGH) {
        NOTES_ON_TABLE[i][j] = LOW;
        sendPedalNote(GET_NOTE(i,j), false);
      }
    }
    // Reset current plus pole to LOW
    digitalWrite(PINS_PLUS[i], LOW);
  }
}
