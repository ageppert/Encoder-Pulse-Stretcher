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
  // ADC reading is 1/2 Vin, scaled relative to 5V Analog Reference. 15K/15K voltage divider between GND-ADC-VIN
  // 5000 mV per 1023 counts = 4.888 [1/2 half battery] milliVolts/count
  // Multiply above result by 2 = 9.775 [full battery] milliVolts/count
  // ADC reading * 9.775 = battery voltage in milliVolts (theoretical scalar)
  static unsigned long BatteryHalfADC = 0 ;
  static float BatteryScalarADCtomV = 9.33 ; // Adjusted to match at 5.7V
  BatteryHalfADC = analogRead(PIN_BATTERY_VOLTAGE);
  BatterymV = (uint16_t)(BatteryHalfADC * BatteryScalarADCtomV);
}

void AnalogSetup() {
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);        // Not required, but makes it clear what this pin is for.
  ReadAnalogVoltage();
}

void AnalogUpdate() {
  static unsigned long ReadPeriodms = 250;
  static unsigned long NowTime = 0;
  static unsigned long AnalogReadTimer = 0;
  
  NowTime = millis();
  if ((NowTime - AnalogReadTimer) >= ReadPeriodms)
  {
    AnalogReadTimer = NowTime;
    ReadAnalogVoltage();
    /*
    Serial.print("Battery ");
    Serial.print(GetBatteryVoltagemV());
    Serial.println(" mV.");
    */
  }
}
