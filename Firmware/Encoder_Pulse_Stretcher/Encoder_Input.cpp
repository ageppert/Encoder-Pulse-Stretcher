#include <stdint.h>
#include <stdbool.h>
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <stdint.h>
#include <stdbool.h>

#include "HardwareIOMap.h"
#include "Encoder_Input.h"

#define CW  0
#define CCW 1

// From http://tronicslab.com/rotary-encoders-complete-tutorial-3/
// Rotary Encoder Code using interrupts
 
// Volatile variables because our interrupt routine changes these variables
// Local Variables

volatile uint32_t oldPosition = 999 ;                    // Something other than zero
volatile uint32_t newPosition = 0;
volatile bool EncoderInputDirection = CW;
volatile uint32_t EncoderInputSpeedCPmS;
volatile int lastEncoded = 0;


void serviceEncoderInterrupt() {
  int signalA = digitalRead(PIN_ENCODER_IN_A);
  int signalB = digitalRead(PIN_ENCODER_IN_B);

  int encoded = (signalB << 1) | signalA;  // converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

  if(sum == 0b0001 || sum == 0b0111 || sum == 0b1110 || sum == 0b1000) newPosition ++;
  if(sum == 0b0010 || sum == 0b1011 || sum == 0b1101 || sum == 0b0100) newPosition --;

  lastEncoded = encoded; // store this value for next time
}

void EncoderInputSetup() {
  // set out input pins as inputs
  pinMode(PIN_ENCODER_IN_A, INPUT); 
  pinMode(PIN_ENCODER_IN_B, INPUT);

  // enable pullup resistors on interrupt pins
  digitalWrite(PIN_ENCODER_IN_A, HIGH);
  digitalWrite(PIN_ENCODER_IN_B, HIGH);

  // call updateEncoder() when any change detected on pin 2/3 interrupt 0/1 pins 
  attachInterrupt(PIN_ENCODER_IN_A, serviceEncoderInterrupt, CHANGE); 
  attachInterrupt(PIN_ENCODER_IN_B, serviceEncoderInterrupt, CHANGE);
}

void EncoderInputTest() {
  newPosition = GetEncoderInputPositionCounts();                  // TO DO: Does this library work for UINT and not INT?
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}

uint16_t GetEncoderInputSpeedCPmS() {
  EncoderInputSpeedCPmS = 0;
  // uint32_t StartTimems = micros();
  uint32_t StartPositionCounts = GetEncoderInputPositionCounts();
  delay(8);                                             // Fixed time period for reading at 8ms for test
  // uint32_t EndTimems = micros();
  uint32_t EndPositionCounts = GetEncoderInputPositionCounts();
  if (EndPositionCounts > StartPositionCounts)
  {
    EncoderInputSpeedCPmS = (EndPositionCounts - StartPositionCounts)>>3; 
  }
  else
  {
    EncoderInputSpeedCPmS = (StartPositionCounts - EndPositionCounts)>>3;
  }
  return (EncoderInputSpeedCPmS);       // TO DO: Add speed range bounds, error handling
}

uint32_t GetEncoderInputPositionCounts() {
  return (newPosition);
}
