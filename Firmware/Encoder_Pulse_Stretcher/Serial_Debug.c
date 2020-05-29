#include <stdint.h>
#include <stdbool.h>
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

void SerialDebugSetup() {
  //Serial.begin(115200);
  // while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only
  //Serial.println();
  //Serial.println("Serial Debug Port Started at ");
}

void SerialDebugProcess() {

}
