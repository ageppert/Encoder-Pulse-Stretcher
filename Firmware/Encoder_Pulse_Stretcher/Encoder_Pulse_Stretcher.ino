/*
 * Encoder Pulse Stretcher
 * 2020 May/June Andy Geppert
 * Arduino Nano Every (Arduino MegaAVR Boards must be added through boards manager)
 * 
 * Counts incoming encoder A & B channel with interrupts.
 * Determines period between channel A and B pulse changes, and the direction of rotation.
 * Updates outgoing virtual encoder A & B channels with 1ms timer interrupt, with increased period per in/out ratio.
 * 
 * 
 * 
 */
// LIBRARY DEPENDENCIES FROM ARDUINO IDE, used here or in other files.
#include <stdint.h>
#include <stdbool.h>

// MAYBE:
// <DigitalWriteFast.h>   
// #include <DigitalIO.h>
// <DigitalIO.h> by Bill Greiman 1.0.0 (defaults to slow for unknown architectures)
// NO: 
// <FastDigitalPin.h>     Romans Audio FastDigitalPin Library by Michael Romans 1.0.1

// OTHER PROJECT FILES IN COMMON FOLDER
#include "HardwareIOMap.h"
#include "Heart_Beat.h"
#include "Serial_Debug.h"
#include "OLED_Screen.h"
#include "Buttons.h"
#include "Analog_Input.h"
#include "Encoder_Input.h"
// YES #include "src/DigitalWriteFast/DigitalWriteFast.h"

#define DEBUG 1

//
// GLOBAL VARIABLES (all must be defined here, and preferably declared too, then use EXTERN in files where they are to be used)
//
// since interrupts change these variables, use volatile in definition.
volatile uint32_t DiffOutputEncoderPulseTimeUs = 0; // TO DO: these delta times can probably be 16 bit
volatile bool EncoderInputDirection = 0;

uint8_t TopLevelState;   // Master State Machine
bool TopLevelStateChanged = false;
enum TopLevelState
{
  STATE_START = 0,        //  0 
  STATE_RUN,              //  1 
  STATE_LAST              //  last one, return to 0.
} ;

/*                      *********************
                        ***     Setup     ***
                        *********************/
void setup() {
  HeartBeatSetup();
  AnalogSetup();
  SerialDebugSetup();
    Serial.begin(115200);  // Need to move this serial stuff into the Serial_Debug.c file out of here!
    //while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only
    Serial.println();
    Serial.print("Serial Debug Port Started at ");
    Serial.println(SERIAL_PORT_SPEED);
  OLEDScreenSetup();
  ButtonsSetup();
  EncoderInputSetup();
  TopLevelState = STATE_START;
}

void loop() {
  //
  // LOCAL VARIABLES for the main loop
  //
  static uint8_t ColorFontSymbolToDisplay = 2;
  static bool ButtonReleased = true;
  static uint32_t Button1HoldTime = 0;
  static uint8_t coreToTest = 0;

  /*                      *********************
                          *** Housekeepting ***
                          *********************/
  HeartBeat();
  AnalogUpdate();
  OLEDScreenUpdate();
  EncoderInputPositionCountsTest();

//  uint16_t EncoderSpeedCPuS = GetEncoderInputSpeedCPuS();
//  if (EncoderSpeedCPuS)
//  {
//     Serial.print("      ");
//     Serial.println(EncoderSpeedCPuS);
//  }
  /*                      ************************
                          *** User Interaction ***
                          ************************
  */
/*
  // MODE SWITCHING
  // If button is pressed, go to next logo symbol and send core array pulse test.
  // Must be released and pressed again for subsequent action.
  Button1HoldTime = Button1State(0);
  if ( (ButtonReleased == true) && (Button1HoldTime >= 500) ){
    Button1State(1); // Pause between presses, clear the duration
    ButtonReleased = false;
    TopLevelState++;
    TopLevelStateChanged = true; // User application has one time to use this before it is reset.
}
  else {
    if (Button1HoldTime == 0) {
      ButtonReleased = true;
      TopLevelStateChanged = false;
      }
  }

  switch(TopLevelState)
  {
  case STATE_START:
    OLEDSetTopLevelState(TopLevelState);
    OLEDScreenUpdate();
    break;

  case STATE_RUN:  
    OLEDSetTopLevelState(TopLevelState);
    OLEDScreenUpdate();
    break;

  case STATE_LAST:
    OLEDSetTopLevelState(TopLevelState);
    OLEDScreenUpdate();
    TopLevelState = STATE_START;   
    break;

  default:
    Serial.println("Invalid TopLevelState");
  }
*/
}

void CheckForSerialCommand() {
  char c;
  if(Serial.available() > 0)
  {
    c = Serial.read();
    Serial.write(c);
    Serial.println();
    switch(c)
    {
    case 'c':
      Serial.println("c");
      break;

    default:
      Serial.print("Ignoring unknown command: ");
      Serial.println(c);
    }
  }
}
