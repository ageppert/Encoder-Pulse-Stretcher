/*
PURPOSE: Test the ANANLOG input pins that are planned for reading battery voltage and/or current with non-blocking code
SETUP:
- Configure in Analog_Input_Test.c
*/
 
#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <stdint.h>

//#ifdef __cplusplus
//extern "C" {
//#endif
uint16_t GetVoltagemV();
void AnalogSetup();
void AnalogUpdate();
void ReadAnalogVoltage();

//#ifdef __cplusplus
//} // extern "C"
//#endif

#endif
