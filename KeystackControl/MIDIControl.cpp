#include "MIDIControl.hpp"

#include "LEDsOutput.hpp"

// MIDI driver instance
#include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();
using namespace MIDI_NAMESPACE;

// MIDI callback definitions
void clbkHandleNoteOn(byte channel, byte note, byte velocity);
void clbkHandleNoteOff(byte channel, byte note, byte velocity);
void clbkHandleSystemExclusive(byte* data, unsigned int dataSize);

void initMidi() {
  MIDI.begin(3);
  MIDI.turnThruOff();

#ifndef TEST_LEDS
  MIDI.setHandleNoteOn(clbkHandleNoteOn);
  MIDI.setHandleNoteOff(clbkHandleNoteOff);
  MIDI.setHandleSystemExclusive(clbkHandleSystemExclusive);
#endif
}

void checkMidiData() {
  MIDI.read();
}

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

void sendPiston(byte number, bool on) {
  if(on)
    sendNoteOn(number, 2);
  else
    sendNoteOff(number, 2);
}

void clbkHandleNoteOn(byte channel, byte note, byte velocity) {
  setLEDOutput(note, true);
}

void clbkHandleNoteOff(byte channel, byte note, byte velocity) {
  setLEDOutput(note, false);
}

void clbkHandleSystemExclusive(byte* data, unsigned int dataSize) {
  if(dataSize < 8 || data[3] != 1)
    return; // Not the right sysex id

  set7SegsOutput(2, data[4]);
  set7SegsOutput(1, data[5]);
  set7SegsOutput(0, data[6]);
}
