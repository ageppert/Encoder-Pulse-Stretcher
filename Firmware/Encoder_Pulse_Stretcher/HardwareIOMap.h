/*
PURPOSE: Map out all of the IO
SETUP:
- 
*/
 
#ifndef HARDWARE_IO_MAP_H
#define HARDWARE_IO_MAP_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


// Hardware for Built-in LED
	#define PIN_BUILT_IN_LED           13   // Ever present Arduino blink LED

// Hardware for voltage monitor
	#define PIN_BATTERY_VOLTAGE        A7    // Voltage monitor is 1/2 Vin

// Hardware for incoming (real) encoder
  	#define PIN_ENCODER_IN_A           9
  	#define PIN_ENCODER_IN_B           10    

// Hardware for outgoing (emulated) encoder
	#define PIN_ENCODER_OUT_A           10  
	#define PIN_ENCODER_OUT_B           11   


// Hardware for Buttons
	#define PIN_BUTTON                 2    // INPUT  DIGITAL

#ifdef __cplusplus
} // extern "C"
#endif

#endif
