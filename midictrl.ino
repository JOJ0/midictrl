#include <advancedSerial.h>
#define Srl Serial
#define p Serial.print
#define pln Serial.println
//#define debug // comment out for debugging

const int MIDI_CMD = 176; // 176 = CC on channel 1
const int CC_COUNT = 8; // configure how many knobs here
const int CC_NUM[] = {81,82,83,84,85,86,87,88}; // configure MIDI CC numbers here
const int PIN_LED_INT = 13;
const int PIN_POT[] = {A0,A1,A2,A3,A4,A5,A6,A7}; // Uno max 6 inputs, Nano max 8
int CC_Val[] = {0,0,0,0,0,0,0,0};
int CC_ValOld[] = {0,0,0,0,0,0,0,0};
uint16_t PotVal[] = {0,0,0,0,0,0,0,0};
uint16_t PotValOld[] = {0,0,0,0,0,0,0,0};
// smoothing refactor
const int numReadings = 10;
int readings[CC_COUNT][numReadings];         // the readings from the analog input
int readIndex[CC_COUNT] = {0,0,0,0,0,0,0,0}; // the index of the current reading
int total[CC_COUNT] = {0,0,0,0,0,0,0,0};     // the running total
int average[CC_COUNT] = {0,0,0,0,0,0,0,0};   // the average


void setup() {
  pinMode(PIN_LED_INT, OUTPUT);
  //pinMode(PIN_SW_BANK, INPUT_PULLUP);
  while(!Srl); // wait until Serial is accessible
  #ifdef debug
      Srl.begin(9600); // common serial rate -> hairless midi bridge or debugging
  #else
      Srl.begin(31250); // MIDI serial rate -> cheap USB MIDI cable hack
  #endif
  // initialize all the pots readings to 0:
  for (int potNo = 0; potNo < CC_COUNT; potNo++) {
      //p("outer loop potNo: "); pln(potNo);
      for (int thisReading = 0; thisReading < numReadings; thisReading++) {
          readings[potNo][thisReading] = 0;
          //p("inner loop: "); pln(readings[potNo][thisReading]);
      }
  }
}

void loop() {
  for (int i=0; i < CC_COUNT; i++) {
    #ifdef debug
    PotVal[i] = analogRead(PIN_POT[i]);
    #endif

    total[i] = total[i] - readings[i][readIndex[i]];
    readings[i][readIndex[i]] = analogRead(PIN_POT[i]);
    total[i] = total[i] + readings[i][readIndex[i]];
    readIndex[i]++;
    if (readIndex[i] >= numReadings) {
        readIndex[i] = 0;
    }
    average[i] = total[i] / numReadings;

    CC_Val[i] = map(average[i],0,1023,0,127);
    #ifdef debug
    if (PotVal[i] != PotValOld[i]) {
    #else
    if (CC_Val[i] != CC_ValOld[i]) {
    #endif

        #ifdef debug
        p("\tPotVal,PotAverage,CC_num,CC_val: ");
        p(PotVal[i]);
        p("\t");
        p(average[i]);
        p("\t");
        p(CC_NUM[i]);
        p("\t");
        pln(CC_Val[i]);
        #else
        Srl.write(MIDI_CMD);
        Srl.write(CC_NUM[i]);
        Srl.write(CC_Val[i]);
        #endif

        #ifdef debug
        PotValOld[i] = PotVal[i];
        #else
        CC_ValOld[i] = CC_Val[i];
        #endif

    }
  }

}
