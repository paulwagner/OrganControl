#include "MidiControl.hpp"

#include "LEDsOutput.hpp"

#ifndef TEST_MIDI
  // MIDI driver instance
  #include <MIDI.h>
  struct MySettings : public midi::DefaultSettings {
    static const long BaudRate = 115200; // Use hairless-midiserial
  };
  MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MySettings);
  
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
#else
  void initMidi(){
    Serial.begin(115200);
  }
  void checkMidiData(){}
#endif

void sendNoteOn(byte note, byte channel) {
#ifndef TEST_MIDI
  MIDI.sendNoteOn(note, 127, channel);
#else
  Serial.print("SendNoteOn: ");
  Serial.print(note);
  Serial.print(", Channel: ");
  Serial.println(channel);
#endif
}
void sendNoteOff(byte note, byte channel) {
#ifndef TEST_MIDI
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
#ifndef TEST_MIDI
 // Use foot controller CC==0x04
 const byte controllerNumber = 0x04;
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
