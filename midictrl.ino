#include <advancedSerial.h>
#define Srl Serial
#define p Serial.print
#define pln Serial.println

static const unsigned MIDI_CMD = 176; // 176 = CC on channel 1
static const unsigned CC_COUNT = 8; // configure how many knobs here
static const unsigned CC_NUM[] = {81,82,83,84,85,86,87,88}; // configure MIDI CC numbers here
static const unsigned PIN_LED_INT = 13;
static const unsigned PIN_POT[] = {A0,A1,A2,A3,A4,A5,A6,A7}; // Uno max 6 inputs, Nano max 8
//static const unsigned PIN_SW_BANK = A8; // FIXME not enough inputs available, handle Pots with 4051 IC
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

void setup() {
  pinMode(PIN_LED_INT, OUTPUT);
  //pinMode(PIN_SW_BANK, INPUT_PULLUP);
  while(!Srl); // wait until Serial is accessible
  Srl.begin(9600); // common serial rate -> hairless midi bridge
  //Srl.begin(31250); // MIDI serial rate -> cheap USB MIDI cable hack
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
    if (CC_Val[i] != CC_ValOld[i]) {
    //if (PotVal[i] != PotValOld[i]) { // DEBUG START
        p("\t");
        p(PotVal[i]);
        p("\t");
        p(PotSmoothVal[i]);
        p("\t");
        p(PotValDiff[i]);
        p("\t");
        p(PotQuickSmoothVal[i]);
        p("\t");
        p(QuickMode[i]);
        p("\t");
        p(x);
        p("\t");
        pln(CC_Val[i]); // DEBUG END

      //Srl.write(MIDI_CMD);
      //Srl.write(CC_NUM[i]);
      //Srl.write(CC_Val[i]);
      CC_ValOld[i] = CC_Val[i];
      PotValOld[i] = PotVal[i];
    }
  }

}
