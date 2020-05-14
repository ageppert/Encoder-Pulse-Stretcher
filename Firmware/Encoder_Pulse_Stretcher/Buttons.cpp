#include <stdint.h>
#include <stdbool.h>
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "HardwareIOMap.H"

void ButtonsSetup() {
  pinMode(PIN_BUTTON, INPUT);
}

uint32_t Button1State(uint32_t clear_duration) { // send a 1 or more to clear, 0 to use normally)
  static uint32_t lasttime;
  static uint32_t thistime = 0;
  static uint32_t duration = 0;
  static uint32_t delta = 0;
  bool state;
  // clear_duration = 1;
  // duration = 0 ;
  if(clear_duration == 1) { duration = 0 ;}
  thistime = millis(); // 100
  state = digitalRead(PIN_BUTTON);
  if(state!=true) {
    delta = thistime - lasttime ;
    duration = duration + delta ;
    }
  else {
    duration = 0; 
    }
  /*
  Serial.print("state,duration, delta, thistime, lasttime ");
  Serial.print(state);
  Serial.print(", ");
  Serial.print(duration);
  Serial.print(", ");
  Serial.print(delta);
  Serial.print(", ");
  Serial.print(thistime);
  Serial.print(", ");
  Serial.print(lasttime);
  Serial.println(" ms");
  */
  lasttime = thistime;
  return duration; // in ms
}
