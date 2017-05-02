#include <MIDI.h>
#include <SoftwareSerial.h>
#define USBserial Serial
SoftwareSerial MIDIserial(0, 1); // RX, TX

static const unsigned MIDI_CH = 1; //configure MIDI channel here
static const unsigned CC_COUNT = 6; // configure how many knobs here
static const unsigned CC_NUM[] = {81,82,83,84,85,86}; // configure MIDI CC numbers here
static const unsigned PIN_LED_INT = 13;
static const unsigned PIN_POT[] = {A0,A1,A2,A3,A4,A5}; // Uno max 6 inputs, Nano max 8
uint8_t CC_Val[] = {0,0,0,0,0,0};
uint8_t CC_ValOld[] = {0,0,0,0,0,0};
uint8_t PotVal[] = {0,0,0,0,0,0};

//MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_INSTANCE(SoftwareSerial, MIDIserial, MIDI);

void setup() {
  pinMode(PIN_LED_INT, OUTPUT);
  MIDI.begin(MIDI_CH); // Launch MIDI and set to channel 1
  while(!USBserial); // wait until USBserial is accessible 
  USBserial.begin(115200); // DEBUG here
  USBserial.println("Arduino midictrl ready");
}

void loop() {

}
