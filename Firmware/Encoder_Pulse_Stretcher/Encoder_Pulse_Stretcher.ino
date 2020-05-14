/*
 * Encoder Pulse Stretcher
 * 2020 Andy Geppert
 * Arduino Nano Every (Arduino MegaAVR Boards must be added through boards manager)
 */

#include <stdint.h>
#include <stdbool.h>

#include "Heart_Beat.h"
#include "Serial_Debug.h"
#include "OLED_Screen.h"
#include "Buttons.h"


// #define DEBUG 1

uint8_t TopLevelState;   // Master State Machine
bool TopLevelStateChanged = false;
enum TopLevelState
{
  STATE_START = 0,         //  0 
  STATE_RUN,              //  1 
  STATE_LAST                       //   last one, return to 0.
} ;



  /*                      *********************
                          ***     Setup     ***
                          *********************
  */
void setup() {
  HeartBeatSetup();
  SerialDebugSetup();
    Serial.begin(115200);  // Need to move this serial stuff into the Serial_Debug.c file out of here!
    //while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only
    Serial.println();
    Serial.println("Serial Debug Port Started at ");
  OLEDScreenSetup();
  ButtonsSetup();
  TopLevelState = STATE_START;
}

void loop() {
  static uint8_t ColorFontSymbolToDisplay = 2;
  static bool ButtonReleased = true;
  static uint32_t Button1HoldTime = 0;
  static uint8_t coreToTest = 0;

  /*                      *********************
                          *** Housekeepting ***
                          *********************
  */
  HeartBeat();
  #ifdef DEBUG
  Serial.println("DEBUG enabled."); // Need to abstract this debug stuff
  #endif

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
    ColorFontSymbolToDisplay++;
    if(ColorFontSymbolToDisplay>3) { ColorFontSymbolToDisplay = 0; }
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
      
      break;

    default:
      Serial.print("Ignoring unknown command: ");
      Serial.println(c);
    }
  }
}
