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

#include "src/DigitalWriteFast/DigitalWriteFast.h"

#define CW  0
#define CCW 1

// From http://tronicslab.com/rotary-encoders-complete-tutorial-3/
// Rotary Encoder Code using interrupts


//
// GLOBAL VARIABLES USED BY THE FUNCTIONS IN THIS FILE
//
extern volatile bool EncoderInputDirection;
extern volatile uint32_t DiffOutputEncoderPulseTimeUs;
 
// Volatile variables because our interrupt routine changes these variables
// Local Variables

volatile uint32_t OldPosition = 999 ;                    // Something other than zero
volatile uint32_t NewPosition = 0;
volatile uint32_t EncoderInputSpeedCPmS;
volatile uint32_t EncoderInputSpeedCPuS;
volatile int lastEncoded = 0;
volatile uint32_t OldInputEncoderPulseTimeUs = 0;    // TO DO: these delta times can probably be 16 bit
volatile uint32_t NewInputEncoderPulseTimeUs = 0; // TO DO: these delta times can probably be 16 bit
volatile uint32_t DiffInputEncoderPulseTimeUs = 0;  // TO DO: these delta times can probably be 16 bit

volatile uint32_t OldOutputEncoderPulseTimeUs = 0;
volatile uint32_t NewOutputEncoderPulseTimeUs = 0;


void serviceEncoderInterrupt() {

  int signalA = digitalRead(PIN_ENCODER_IN_A);
  int signalB = digitalRead(PIN_ENCODER_IN_B);

  int encoded = (signalB << 1) | signalA;  // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded;  // adding it to the previous encoded value

  NewInputEncoderPulseTimeUs = micros();  // Every time a pulse occurs, keep track of when it occurred.
  OldPosition = NewPosition;
  if(sum == 0b0001 || sum == 0b0111 || sum == 0b1110 || sum == 0b1000) 
  {
    NewPosition ++;
    EncoderInputDirection = CW;
  }
  if(sum == 0b0010 || sum == 0b1011 || sum == 0b1101 || sum == 0b0100)
  {
    NewPosition --;
    EncoderInputDirection = CCW;
  }

  lastEncoded = encoded; // store this value for next time

  DiffInputEncoderPulseTimeUs = NewInputEncoderPulseTimeUs - OldInputEncoderPulseTimeUs;
  OldInputEncoderPulseTimeUs = NewInputEncoderPulseTimeUs; 

  // If this interrupt is active, but definition, the encoder is moving.
  // DiffOutputEncoderPulseTimeUs = DiffInputEncoderPulseTimeUs * 7 / 3;
  /*
  5x Fast Arduino division: https://forum.arduino.cc/index.php?topic=275431.0
  The equivalent of "n / 3" in a faster form is "n * x >> b" (>> is bit shift, aka division)
  Where n is the operand, x is 341, and b is 10
  With numbers, n * 341/2^10 = n * 0.33301
  */
  DiffOutputEncoderPulseTimeUs = (DiffInputEncoderPulseTimeUs * 7 * 341) >>10;
  //                           =   100 * 7 * 341 / 2^10 = 233
  // With Nano Every 20 Mhz, the system chokes with pulse spacing > 100 us (using optical encoder)
  // Ultra slow with a 3 pole encoder is about 22ms between pulses. 22,000 us
  //                           = 22000 * 7 * 341 / 2^10 = 



}



// Interrupt is called once a millisecond, 
ISR(TCA0_CMP1_vect) {

  const uint8_t EncoderOutputArrayLength = 4;
  const bool EncoderOutputArray [] [2] = {
    { 1 , 0 },
    { 1 , 1 },
    { 0 , 1 },
    { 0 , 0 }
  };
  static uint8_t EncoderOutputArrayPosition = 0;    // index through the array of output pulse sequence

    cli();

  // UPDATE THE OUTPUT ENCODER
  NewOutputEncoderPulseTimeUs = micros();
  if ((NewOutputEncoderPulseTimeUs-NewInputEncoderPulseTimeUs) < 10000UL) { // As long as it's been less then 25 ms since last input pulse, assume encoder is still rotating.
    if ( (NewOutputEncoderPulseTimeUs - OldOutputEncoderPulseTimeUs) >= DiffOutputEncoderPulseTimeUs ) { // time to update the output encoder 
      if (EncoderInputDirection == CW) {
        EncoderOutputArrayPosition++;
      }
      else {
        EncoderOutputArrayPosition--;    
      }
      // Is it time to wrap-around the array?
      if (EncoderOutputArrayPosition == EncoderOutputArrayLength) {
        EncoderOutputArrayPosition = 0;
      }
      if (EncoderOutputArrayPosition == 255) {
        EncoderOutputArrayPosition = 3;
      }
      // Update pins to reflect output encoder state
      OldOutputEncoderPulseTimeUs = NewOutputEncoderPulseTimeUs;
      digitalWriteFast(PIN_ENCODER_OUT_A, EncoderOutputArray [EncoderOutputArrayPosition] [0] );
      digitalWriteFast(PIN_ENCODER_OUT_B, EncoderOutputArray [EncoderOutputArrayPosition] [1] );
    }
  }

    TCA0.SINGLE.INTFLAGS |= bit(5);                 // Clears the interrupt flag. Rather confusingly,
                                                    // this is done by setting a bit in the register
                                                    // to 1.
                                                    // Use bit(4) for CMP0, bit(5) CMP1, bit(6) CMP2
    sei();
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

  // set out input pins as inputs
  pinMode(PIN_ENCODER_OUT_A, OUTPUT); 
  pinMode(PIN_ENCODER_OUT_B, OUTPUT);


// FROM: https://forum.arduino.cc/index.php?topic=668204
// Arduino Nano Every Timer/Counter A example
// =========================================================
// There is one type A timer/counter (TCA0) and it has three
// compare channels (CMP0, CMP1, CMP2).
//
// The clock prescaler (bits 3:1 of CTRLA register) is set to
// 64 by default and changing it breaks delay() and
// millis() (and maybe other things, not checked):
// increasing/decreasing it increases/decreases the length
// of "a second" proportionally. It also changes the PWM
// frequency of pins 5, 9, and 10, although they will still
// do PWM (increasing/decreasing the prescaler value
// decreases/increases the frequency proportionally).
//
// The period value (PER register) is set to 255 by default.
// Changing it affects the duty cycle of PWM pins (i.e. the
// range 0-255 in analogWrite no longer corresponds to a
// 0-100% duty cycle).
//
// PWM pin 5 uses compare channel 2 (CMP2), pin 9 channel
// 0 (CMP0), and pin 10 channel 1 (CMP1).
//
// If you don't mind losing PWM capability (or at least
// changing the frequency and messing up the duty cycle)
// on pins 5, 9, or 10, you can set up these channels for
// your own use.
//
// If you don't mind messing up delay() and millis(), you
// can change the prescaler value to suit your needs. In
// principle, since the prescaler affects millis() and
// delay() in an inversely proportional manner, this could
// be compensated for in your sketch, but this is getting
// messy.
//
// If the prescaler is not changed, the lowest interrupt
// frequency obtainable (by setting PER and CMPn to 65535)
// is about 3.8 Hz.
//
// The interrupt frequency (Hz) given a prescaler value
// and PER value is:
// f = 16000000 / (PER + 1) / prescaler
//
// The PER value for a required frequency f is:
// PER = (16000000 / prescaler / f) - 1
//
// The prescaler can take a setting of 1, 2, 4, 8,
// 16, 64, 256, or 1024. See the ATmega 4809 datasheet
// for the corresponding bit patterns in CTRLA.
//
// http://ww1.microchip.com/downloads/en/DeviceDoc/megaAVR-0-series-Family-Data-Sheet-40002015C.pdf
//
// The program below demonstrates TCA0 interrupts on
// compare channel 1, with and without changing the
// prescaler (change the value of CHANGE_PRESCALER
// from 0 to 1).
//
// PER is the TOP value for the counter. When it
// reaches this value it is reset back to 0. The
// CMP1 value is set to the same value, so that an
// interrupt is generated at the same time that the
// timer is reset back to 0. If you use more than
// one compare channels at the same time, all CMPn
// registers must have a value smaller or equal to PER or
// else they will not generate an interrupt. The
// smallest obtainable frequency is governed by the
// value of PER.
//
// With CHANGE_PRESCALER left at 0, the prescale
// factor is left at its default value of 64, and the
// value of PER is set to the maximum (65535). The
// interrupts fire approx 3.8 times per second while
// the main program prints text to the terminal
// exactly once per second using millis().
//
// When CHANGE_PRESCALER is changed to 1, the prescaler
// factor is changed to 256. The value of PER is calculated
// so that the interrupts will fire exactly once per
// second. The main program still uses millis() to
// print text to the terminal once every 1000 milliseconds
// but now a "second" takes four seconds. This could be
// compensated for by waiting for only 250 milliseconds.
//
// Change CMP1 to CMP0 or CMP2 as required. Change indicated
// bit values too.



#define CHANGE_PRESCALER    0
volatile bool i = false;


    unsigned int per_value;
    cli();
#   if CHANGE_PRESCALER == 0                        // Use default 64 prescale factor
        // per_value = 0xFFFF;                         // Use maximum possible PER/CMP value (65535)
        per_value = 65;                         // Use maximum possible PER/CMP value (65535)
        TCA0.SINGLE.PER = per_value;                // Set period register
        TCA0.SINGLE.CMP1 = per_value;               // Set compare channel match value
        TCA0.SINGLE.INTCTRL |= bit(5);              // Enable channel 1 compare match interrupt.
                                                    // Use bit(4) for CMP0, bit(5) CMP1, bit(6) CMP2
#   elif CHANGE_PRESCALER == 1                      // Change prescale factor
        TCA0.SINGLE.CTRLA = B00001101;              // Prescaler set to 256. Use the following for
                                                    // other prescalers:
                                                    // B00000001    1
                                                    // B00000011    2
                                                    // B00000101    4
                                                    // B00000111    8
                                                    // B00001001    16
                                                    // B00001011    64
                                                    // B00001101    256
                                                    // B00001111    1024
        per_value = 0xF423;                         // Value required for 1 Hz (62499)
        TCA0.SINGLE.PER = per_value;                // Set period register
        TCA0.SINGLE.CMP1 = per_value;               // Set compare channel match value
        TCA0.SINGLE.INTCTRL |= bit(5);              // Enable channel 1 compare match interrupt.
                                                    // Use bit(4) for CMP0, bit(5) CMP1, bit(6) CMP2
#   endif
    sei();

}

void EncoderInputPositionCountsTest() {
  NewPosition = GetEncoderInputPositionCounts();                  // TO DO: Does this library work for UINT and not INT?
  if (NewPosition != OldPosition) {
    OldPosition = NewPosition;
    // Serial.print(NewPosition);
    // Serial.print("    ");
    // Serial.println(DiffInputEncoderPulseTimeUs);
  }
}

uint16_t GetEncoderInputSpeedCPmS() {
  EncoderInputSpeedCPmS = 0;
  uint32_t StartPositionCounts = GetEncoderInputPositionCounts();
  delay(8);                                             // Fixed time period for reading at 8ms for test
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

uint16_t GetEncoderInputSpeedCPuS() {
  EncoderInputSpeedCPuS = 0;
  if (NewPosition > OldPosition)
  {
    EncoderInputSpeedCPuS = (NewPosition - OldPosition) / DiffInputEncoderPulseTimeUs; 
  }
  else
  {
    EncoderInputSpeedCPuS = (OldPosition - NewPosition) / DiffInputEncoderPulseTimeUs;
  }
  return (EncoderInputSpeedCPuS);       // TO DO: Add speed range bounds, error handling
}

uint32_t GetEncoderInputPulseTimeUs() {
  uint32_t NowTimeus = micros();
  // if ( (NowTimeus-OldInputEncoderPulseTimeUs) >= 30000UL ) DiffInputEncoderPulseTimeUs = 0; // TO DO: Doesn't work?
  return (DiffInputEncoderPulseTimeUs);
}

uint32_t GetEncoderInputPositionCounts() {
  return (NewPosition);
}

uint32_t GetDiffInputEncoderPulseTimeUs() {
  return (DiffInputEncoderPulseTimeUs);
}

uint32_t GetDiffOutputEncoderPulseTimeUs() {
  return (DiffOutputEncoderPulseTimeUs);
}

bool GetEncoderInputDirection() {
  return (EncoderInputDirection);
}
