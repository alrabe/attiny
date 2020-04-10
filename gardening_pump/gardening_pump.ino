#include "tinysnore.h"
#include "PinChangeInterrupt.h"

#define WATERING_DELAY_SEC 60
#define WATERING_TIME_SEC 12
#define SNORE_TIME_SEC 4
#define PIN_PUMP 0
#define PIN_BUTTON 2

int currentInterval = 0;
volatile bool maualStartRequested = false;
volatile bool pumpIsRunning = false;

void setup() {
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(PIN_BUTTON), buttonPressed, FALLING);
}

void loop() {
  while(1) {
    if (isTimeToStart()) {
      pumpWater();
      resetTimeToStart();
    }
    else if(maualStartRequested) {           
      pumpWater();
      substractPumpTime();
      maualStartRequested = false;
    }
    else {
      snore(SNORE_TIME_SEC*1000);
    }
  }
}

void substractPumpTime() {
  int skipedIntervals = (WATERING_TIME_SEC / SNORE_TIME_SEC);
  currentInterval += skipedIntervals;
}

bool isTimeToStart() {
  return (currentInterval++ * SNORE_TIME_SEC) >= WATERING_DELAY_SEC; 
}

void resetTimeToStart() {
  currentInterval = (WATERING_TIME_SEC / SNORE_TIME_SEC);
}

void buttonPressed() {
  if(!pumpIsRunning)
    maualStartRequested = true;
}

void pumpWater() {
  disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(PIN_BUTTON));
  pumpOn();
  delay(WATERING_TIME_SEC * 1000);
  pumpOff();
  enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(PIN_BUTTON));
}
 
void pumpOn() {
  pumpIsRunning = true;
  digitalWrite(PIN_PUMP, HIGH);
}

void pumpOff() {
  digitalWrite(PIN_PUMP, LOW);
  pumpIsRunning = false;
}
