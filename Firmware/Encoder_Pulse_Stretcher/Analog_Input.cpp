#include <stdint.h>
#include <stdbool.h>
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "HardwareIOMap.H"
#include "Analog_Input.h"

static uint16_t BatterymV = 0 ;

uint16_t GetVoltagemV() {
  return (BatterymV);
}

void ReadAnalogVoltage() {
/*
  Arduino Nano Every Wiring: Vin - 47K - 15K - GND
  so 15 / (47+15) = .242 (24.2%)
  Arduino Nano Every AnalogSetup() to use reference of 2.5V 
  2500 mV per 1023 counts (10-bit) = 2.444 (mV/count) will show 24.2% of actual ADC pin voltage.
  One more step, now divide the scalar above by .242 to see actual voltage = 10.1 (mV/count)
  ADC (counts) * 10.1 = theoretical voltage in mV
  Reading range of Vin: 2.5 to 10.3

  Scalar | Multimeter (V) | OLED Display (mv) | Scalar Adjustment needed
  10.1   | 4.72           | 5413              | reduce by 15%
   8.6   | 4.72           | 4610              | increase by 2%
   8.8   | 4.72           | 4712              | PASS!

*/
  static unsigned long BatteryHalfADC = 0 ;
  static float BatteryScalarADCtomV = 8.8 ; // Adjusted to match at 4.72V
  BatteryHalfADC = analogRead(PIN_BATTERY_VOLTAGE);
  BatterymV = (uint16_t)(BatteryHalfADC * BatteryScalarADCtomV);
}

void AnalogSetup() {
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);        // Not required, but makes it clear what this pin is for.
  ReadAnalogVoltage();
  
  #ifdef ARDUINO_AVR_NANO_EVERY
    analogReference(INTERNAL2V5); // Use one of the references
  #else
    Serial.println("Warning: For Nano Every only - stoped!");
    While(true) delay(10);
  #endif
}

void AnalogUpdate() {
  static unsigned long ReadPeriodms = 300;
  static unsigned long NowTime = 0;
  static unsigned long AnalogReadTimer = 0;
  
  NowTime = millis();
  if ((NowTime - AnalogReadTimer) >= ReadPeriodms)
  {
    AnalogReadTimer = NowTime;
    ReadAnalogVoltage();
//    Serial.print("Battery ");
//    Serial.print(GetVoltagemV());
//    Serial.println(" mV.");
  }
}
