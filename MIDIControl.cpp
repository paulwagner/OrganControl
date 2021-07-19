#include "MidiControl.hpp"

#include "LEDsOutput.hpp"

#ifndef PRINT_OUTPUT
#include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();
using namespace MIDI_NAMESPACE;
#endif

// MIDI callback definitions
void clbkHandleNoteOn(byte channel, byte note, byte velocity);
void clbkHandleNoteOff(byte channel, byte note, byte velocity);
void clbkHandleSystemExclusive(byte* data, unsigned int arraySize);

void initMidi() {
#ifndef PRINT_OUTPUT
  MIDI.setHandleNoteOn(clbkHandleNoteOn);
  MIDI.setHandleNoteOff(clbkHandleNoteOff);
  MIDI.setHandleSystemExclusive(clbkHandleSystemExclusive);
  MIDI.begin(MIDI_CHANNEL_OMNI);
#endif
}

void checkMidiData() {
#ifndef PRINT_OUTPUT
  MIDI.read();
#endif  
}

void sendNoteOn(byte note, byte channel) {
#ifndef PRINT_OUTPUT
  MIDI.sendNoteOn(note, 127, channel);
#else
  Serial.print("SendNoteOn: ");
  Serial.print(note);
  Serial.print(", Channel: ");
  Serial.println(channel);
#endif
}
void sendNoteOff(byte note, byte channel) {
#ifndef PRINT_OUTPUT
  MIDI.sendNoteOff(note, 127, channel);
#else
  Serial.print("SendNoteOff: ");
  Serial.print(note);
  Serial.print(", Channel: ");
  Serial.println(channel);
#endif 
}

void sendPedalNote(byte note, bool on) {
  if(on)
    sendNoteOn(note, 1);
  else
    sendNoteOff(note, 1);
}

void sendPiston(byte number, bool on) {
  if(on)
    sendNoteOn(number, 2);
  else
    sendNoteOff(number, 2);
}

void sendSwellerValue(byte value, byte channel) {
 // Use foot controller CC==0x04
 const byte controllerNumber = 0x04;
#ifndef PRINT_OUTPUT
  MIDI.sendControlChange(controllerNumber, value, channel);
#else
  Serial.print("SendSweller");
  Serial.print(channel);
  Serial.print(": ");
  Serial.println(value);
#endif
}

void sendSweller1Value(byte value) {
  sendSwellerValue(value, 1);
}

void sendSweller2Value(byte value) {
  sendSwellerValue(value, 2);
}


void clbkHandleNoteOn(byte channel, byte note, byte velocity) {
  // TODO: It probably won't work doing this in the callback directly. Use a synchronized buffer instead?
  setLEDOutput(note, true);
}

void clbkHandleNoteOff(byte channel, byte note, byte velocity) {
  // TODO: It probably won't work doing this in the callback directly. Use a synchronized buffer instead?
  setLEDOutput(note, false);
}

void clbkHandleSystemExclusive(byte* data, unsigned int arraySize) {
  // TODO: It probably won't work doing this in the callback directly. Use a synchronized buffer instead?
  
  if (data[3] != 1)
    return; // Not the right display id

  // Find start of data field
  int i = 0;
  while(data[i] != 125 && i < arraySize)
    i++;

  // Get sequencer number
  int digit = 2;
  while(digit >= 0 && i < arraySize) {
    set7SegsOutput(digit, data[i]);
    digit--;
    i++;
  }
}