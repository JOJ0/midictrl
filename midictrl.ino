#include <advancedSerial.h>
#include <Ewma.h>
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
//int PotSmooth[CC_COUNT] = {0,0,0,0,0,0,0,0};         // the readings from the analog input
float PotSmooth[CC_COUNT] = {0,0,0,0,0,0,0,0};         // the readings from the analog input
int readIndex[CC_COUNT] = {0,0,0,0,0,0,0,0}; // the index of the current reading
int total[CC_COUNT] = {0,0,0,0,0,0,0,0};     // the running total
int average[CC_COUNT] = {0,0,0,0,0,0,0,0};   // the average
// smoothing refactor using Ewma library
Ewma adcFilter1(1); // smooth val 0-1024
//Ewma adcFilter1(0.5); // smooth val 0-513
//Ewma adcFilter1(0.1); // smooth val 0-179
//Ewma adcFilter1(0.05); // smooth val 0-151
//Ewma adcFilter1(0.01); // smooth val 0-132


void setup() {
  pinMode(PIN_LED_INT, OUTPUT);
  //pinMode(PIN_SW_BANK, INPUT_PULLUP);
  while(!Srl); // wait until Serial is accessible
  #ifdef debug
      Srl.begin(9600); // common serial rate -> hairless midi bridge or debugging
  #else
      Srl.begin(31250); // MIDI serial rate -> cheap USB MIDI cable hack
  #endif
}

void loop() {
    for (int i=0; i < CC_COUNT; i++) {
        PotVal[i] = analogRead(PIN_POT[i]);
        PotSmooth[i] =adcFilter1.filter(PotVal[i]);

        CC_Val[i] = map(PotSmooth[i],0,1023,0,127);
        //CC_Val[i] = map(PotSmooth[i],0,513,0,127);
        //CC_Val[i] = map(PotSmooth[i],0,179,0,127);
        //CC_Val[i] = map(PotSmooth[i],0,155,0,127);

        #ifdef debug
        if (PotVal[i] != PotValOld[i]) {
        #else
        if (CC_Val[i] != CC_ValOld[i]) {
        #endif

            #ifdef debug
            p("\tPotVal,PotSmooth,CC_num,CC_val: ");
            p(PotVal[i]);
            p("\t");
            p(PotSmooth[i]);
            p("\t");
            p(CC_NUM[i]);
            p("\t");
            pln(CC_Val[i]);
            #else
            Srl.write(MIDI_CMD);
            Srl.write(CC_NUM[i]);
            Srl.write(CC_Val[i]);
            #endif

        }
        PotValOld[i] = PotVal[i];
        CC_ValOld[i] = CC_Val[i];
    }

}
