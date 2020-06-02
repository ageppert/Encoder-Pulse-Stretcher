/*
PURPOSE: Set configuration parameters for incoming and simulated encoder.
SETUP: none
*/
 
#ifndef CONFIG_H
#define CONFIG_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t Encoder_Input_PPR = 7;
uint8_t Encoder_Output_PPR = 3;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CONFIG_H
