#include "MIDIUSB.h"
#include "PitchToNote.h"
#define NUM_INPUTS 12
#define NUM_BUTTONS 3

int threshold = 0;

// pentatonic
//const byte notePitches[NUM_INPUTS] = {pitchC2, pitchD2, pitchE2, pitchG2, pitchA2, pitchC3,
//                                pitchD3, pitchE3, pitchG3, pitchA3, pitchC4, pitchD4};

// Major chords
const byte notePitches[NUM_INPUTS] = {
  pitchC1, pitchE1, pitchG1,
  pitchC3, pitchE3, pitchG3,
pitchC5, pitchE5, pitchG5,
pitchC7, pitchE7, pitchG7};

const int sensors[NUM_INPUTS] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11};
byte velocities[NUM_INPUTS];
uint16_t pressedSensors = 0x00;
uint16_t previousSensors = 0x00;

// Button settings
const byte buttonNotePitches[NUM_BUTTONS] = {pitchC2, pitchC1, pitchC3};
const int buttons[NUM_BUTTONS] = {0, 2, 3};
uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;


void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < NUM_INPUTS; i++)
  {
    pinMode(sensors[i], INPUT);
    int initialReading = analogRead(sensors[i]);
    if (initialReading + 32 > threshold)
    {
      threshold = initialReading + 32;
    }
  }
  Serial.println("Starting. Light threshold set at " + threshold);
}

void loop() {
  readSensors();
  readButtons();
  playNotes();
  playButtonNotes();
}

void readSensors()
{
  for (int i = 0; i < NUM_INPUTS; i++)
  {
    int reading = analogRead(sensors[i]);
    delay(50);
    if (reading > threshold)
    {
      velocities[i] = constrain(map(reading, threshold, 1023, 0, 127), 0, 127);
      bitWrite(pressedSensors, i, 1);
    }
    else
    {
      velocities[i] = 0;
      bitWrite(pressedSensors, i, 0);
    }
  }
  Serial.println("Note velocities: " + String(velocities[0]) + ", " + String(velocities[1]) + ", " + 
  String(velocities[2]) + ", " + String(velocities[3]) + ", " + String(velocities[4]) + ", " + 
  String(velocities[5]) + ", " + String(velocities[6]) + ", " + String(velocities[7]) + ", " + 
  String(velocities[8]) + ", " + String(velocities[9]) + ", " + String(velocities[10]) + ", " + 
  String(velocities[11]) + ", ");
}

void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == HIGH)
    {
      bitWrite(pressedButtons, i, 1);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void playNotes()
{
  for (int i = 0; i < NUM_INPUTS; i++)
  {
    if (bitRead(pressedSensors, i) != bitRead(previousSensors, i))
    {
      if (bitRead(pressedSensors, i))
      {
        bitWrite(previousSensors, i , 1);
        noteOn(0, notePitches[i], velocities[i]);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousSensors, i , 0);
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

void playButtonNotes()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i, 1);
        noteOn(1, buttonNotePitches[i], 100);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousButtons, i , 0);
        noteOff(0, buttonNotePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
