/*
	PURPOSE: Define/assign/log the version scheme and number.
	SETUP: Manually update the field assigned as VERSION
*/

/*	VERSION SCHEME
	major.minor.date-iteration
	major.minor is locked to hardware version
	date is last two digits of year, two digit month, two digit day of the month
	iteration increments from build to build
*/

// TO DO: connect version number to a value that the firmware can interact with and display.

//                    "HW: 45678901234567890" 20 [small font 1] characters combined maximum
#define HARDWARE_VERSION  "0.0.1 Breadboard"   		
/* 
 * V0.1 the first batch of 5 prototype green boards. 
 * V0.2 hand re-worked updates on V0.1 for first bring-up.
*/
//                    "FW: 45678901234567890" 20 [small font 1] characters combined maximum
#define FIRMWARE_VERSION  "0.0.16 VCal 10.35"

const String FirmwareVersion = FIRMWARE_VERSION;
const String HardwareVersion = HARDWARE_VERSION;
