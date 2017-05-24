#include <MIDI.h>
#include <SoftwareSerial.h>
#define USBserial Serial
SoftwareSerial MIDIserial(0, 1); // RX, TX

static const unsigned MIDI_CH = 1; //configure MIDI channel here
static const unsigned CC_COUNT = 8; // configure how many knobs here
static const unsigned CC_NUM[] = {81,82,83,84,85,86,87,88}; // configure MIDI CC numbers here
static const unsigned PIN_LED_INT = 13;
static const unsigned PIN_POT[] = {A0,A1,A2,A3,A4,A5,A6,A7}; // Uno max 6 inputs, Nano max 8
uint8_t CC_Val[] = {0,0,0,0,0,0,0,0};
uint8_t CC_ValOld[] = {0,0,0,0,0,0,0,0};
uint16_t PotVal[] = {0,0,0,0,0,0,0,0};
uint16_t PotValOld[] = {0,0,0,0,0,0,0,0};
uint8_t i = 8;
// smoothing
int PotSmoothVal[] = {0,0,0,0,0,0,0,0};
int PotQuickSmoothVal[] = {0,0,0,0,0,0,0,0};
int PotValDiff[] = {0,0,0,0,0,0,0,0};
bool QuickMode[] = {false,false,false,false,false,false,false,false};
//int x[] = {0,0,0,0,0,0,0,0};
int x = 0;

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
  for (i=0; i<CC_COUNT; i++) {
    PotVal[i] = analogRead(PIN_POT[i]);
    PotSmoothVal[i] = 0.6 * PotSmoothVal[i] + 0.4 * PotVal[i];
    PotValDiff[i] = abs(PotVal[i] - PotValOld[i]);
    if (PotValDiff[i] <= 5 && QuickMode[i] == false) {
      PotQuickSmoothVal[i] = 0.6 * PotSmoothVal[i] + 0.4 * PotVal[i];
    }
    else if (PotValDiff[i] <= 5 && QuickMode[i] == true){
      PotQuickSmoothVal[i] = PotVal[i];
      x++;
    }
    else {
      PotQuickSmoothVal[i] = PotVal[i];
      QuickMode[i] = true;
    }
    if (x >= 10) {
      x = 0;
      QuickMode[i] = false;
    }
    CC_Val[i] = map(PotQuickSmoothVal[i],0,1023,0,127);
    //if (CC_Val[i] != CC_ValOld[i]) {
    if (PotVal[i] != PotValOld[i]) {
     /* USBserial.print("PotSmoothVal: ");USBserial.println(PotSmoothVal[i]);
      USBserial.print("PotQuickSmootVal: ");USBserial.println(PotQuickSmoothVal[i]);
      USBserial.print("PotValDiff: ");USBserial.println(PotValDiff[i]);
      USBserial.print("QuickMode: ");USBserial.println(QuickMode[i]);
      USBserial.print("x: ");USBserial.println(x[i]);
      USBserial.print("CC_Val: ");USBserial.println(CC_Val[i]);
    */
      USBserial.print("\t");
      USBserial.print(PotVal[i]);
      USBserial.print("\t");
      USBserial.print(PotSmoothVal[i]);
      USBserial.print("\t");
      USBserial.print(PotValDiff[i]);
      USBserial.print("\t");
      USBserial.print(PotQuickSmoothVal[i]);
      USBserial.print("\t");
      USBserial.print(QuickMode[i]);
      USBserial.print("\t");
      USBserial.print(x);
      USBserial.print("\t");
      USBserial.println(CC_Val[i]);
      MIDI.sendControlChange(CC_NUM[i], CC_Val[i], MIDI_CH);
      CC_ValOld[i] = CC_Val[i];
      PotValOld[i] = PotVal[i];
    }
  }

}
