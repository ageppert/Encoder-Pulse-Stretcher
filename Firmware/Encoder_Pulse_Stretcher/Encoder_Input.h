/*
PURPOSE: read an encoder
SETUP:
- Configure in HardwareIOMap.h
*/
 
#ifndef ENCODER_INPUT_H
#define ENCODER_INPUT_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <stdint.h>

//#ifdef __cplusplus
//extern "C" {
//#endif

void EncoderInputSetup();
void EncoderInputTest();
int32_t GetEncoderInputPosition();

//#ifdef __cplusplus
//} // extern "C"
//#endif

#endif
