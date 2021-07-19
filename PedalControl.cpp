#include "PedalControl.hpp"

#include "MidiControl.hpp"

#define GET_NOTE(i,j) (36 + (8*j) + i)

static int PINS_MINUS[] = {28, 26, 24, 22};
static int PINS_MINUS_SIZE = 4;
static int PINS_PLUS[] = {46, 44, 42, 40, 38, 36, 34, 32};
static int PINS_PLUS_SIZE = 8;

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
        continue; // Unused combinations
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
