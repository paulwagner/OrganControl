#include "MIDIControl.hpp"

// MIDI driver instance
#include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();
using namespace MIDI_NAMESPACE;

void sendNoteOn(byte note, byte channel) {
  MIDI.sendNoteOn(note, 127, channel);
#ifdef PRINT_MIDI
  Serial.print("SendNoteOn: ");
  Serial.print(note);
  Serial.print(", Channel: ");
  Serial.println(channel);
#endif
}
void sendNoteOff(byte note, byte channel) {
  MIDI.sendNoteOff(note, 127, channel);
#ifdef PRINT_MIDI
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
  MIDI.sendControlChange(controllerNumber, value, channel);
#ifdef PRINT_MIDI
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