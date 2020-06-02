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

#ifdef __cplusplus
extern "C" {
#endif

void EncoderInputSetup();
void EncoderInputPositionCountsTest();
uint16_t GetEncoderInputSpeedCPmS();
uint16_t GetEncoderInputSpeedCPuS();
uint32_t GetEncoderInputPositionCounts();
uint32_t GetEncoderInputPulseTimeUs();
uint32_t GetDiffInputEncoderPulseTimeUs();
uint32_t GetDiffOutputEncoderPulseTimeUs();
bool GetEncoderInputDirection();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ENCODER_INPUT_H
