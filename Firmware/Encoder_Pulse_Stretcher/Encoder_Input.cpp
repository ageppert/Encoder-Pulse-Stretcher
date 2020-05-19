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
#include <Encoder.h>

static long oldPosition  = -999 ;


Encoder myEnc(PIN_ENCODER_IN_A, PIN_ENCODER_IN_B);

void EncoderInputSetup() {
  // Nothing to do here
}

void EncoderInputTest() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}

int32_t GetEncoderInputPosition() {
  return (myEnc.read());
}

