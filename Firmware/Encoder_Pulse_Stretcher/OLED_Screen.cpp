#include <stdint.h>
#include <stdbool.h>
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "version.h"
#include <Wire.h>   // Default is SCL0 and SDA0 on pins 19/18 of Teensy LC
//#define Pin_I2C_Bus_Data       18    // Default is SCL0 and SDA0 on pins 19/18 of Teensy LC. #define not needed, as Wire.h library takes care of this pin configuration.
//#define Pin_I2C_Bus_Clock      19    // Default is SCL0 and SDA0 on pins 19/18 of Teensy LC. #define not needed, as Wire.h library takes care of this pin configuration.
#include "Analog_Input.h"
#include "HardwareIOMap.h"

#include <Adafruit_GFX.h>
//#include "src/Adafruit-GFX-Library-1.4.8/Adafruit_GFX.h" // ToDo: include this and use a later version.
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t TopLevelStateLocal = 0;

uint8_t Encoder_Input_PPR = 7; // TO DO move this to another file
uint8_t Encoder_Output_PPR = 3; // TO DO move this to another file
uint8_t Encoder_Input_Speed = 0; // TO DO move this to another file
uint8_t Encoder_Input_Dir = 1;   // TO DO move this to another file

void OLEDScreenSplash() {
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("    Encoder Pulse    "));
  display.println(F("  S T R E T C H E R  "));
  display.println(F("   by Andy Geppert   "));
  display.display();
}

void OLEDScreenStatus() {
  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner

  display.println(F("    Encoder Pulse    "));
  display.println(F("  S T R E T C H E R  "));
  display.println(F("   by Andy Geppert   "));
  display.print(F("      Vin: "));
  display.println(GetVoltagemV(),DEC);

  display.print(F("HW: "));  
  display.println(F(HARDWARE_VERSION));
  display.print(F("FW: "));
  display.println(F(FIRMWARE_VERSION));
  display.print(F("State: "));
  display.print(TopLevelStateLocal,DEC); 
  display.print(F("  PPR: "));
  display.print(Encoder_Input_PPR,DEC); 
  display.print(F(" -> "));
  display.println(Encoder_Output_PPR,DEC); 
  display.print(F("Dir: "));
  display.print(Encoder_Input_Dir,DEC);  
  display.print(F(" Speed: "));
  display.println(Encoder_Input_Speed,DEC);  

  display.display();
}

void OLEDScreenSetup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64 
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  OLEDScreenSplash();
}

void OLEDScreenUpdate() {
  static unsigned long UpdatePeriodms = 250;  
  static unsigned long NowTime = 0;
  static unsigned long UpdateTimer = 0;
  NowTime = millis();
  if ((NowTime - UpdateTimer) >= UpdatePeriodms)
  {
    UpdateTimer = NowTime;
    OLEDScreenStatus();
  }
}

void OLEDScreenClear() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.display();
}

void OLEDSetTopLevelState(uint8_t state) {
  TopLevelStateLocal = state;
}
