#include "SerialWombat.h"
/*
This example shows how to configure a Serial Wombat 8B or 18AB chip to blink an LED any
time the Serial Wombat chip receives an I2C command 

This sketch was last tested with version 2.2.2 of the firmware.


A video demonstrating the Blink pin mode is available here:
TODO

Documentation for the SerialWombatBlink class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_blink.html

For reference, the source code to the firmware (looking at this isn't required, but is interesting) is available here:
https://github.com/BroadwellConsultingInc/SerialWombat/blob/main/SerialWombatPinModes/blink.c

*/


SerialWombatChip sw;
SerialWombatBlink blinkPin(sw);


#define BLINK_PIN 1 // SW8B PCB0029 On Board LED is on pin 1 (must close solder jumper)


void setup() {
	// put your setup code here, to run once:
	Wire.begin();   //Initialize the I2C Bus on default pins

	Serial.begin(115200);
	delay(3000);
	Serial.println("I2C Command Blink Example");


	sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

	//Optional Error handling code begin:
	if(sw.isSW04())
	{
		Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
		while(1){delay(100);}
	}
	if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
	if(!sw.isPinModeSupported(PIN_MODE_BLINK))
	{
		Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
		while(1){delay(100);}
	}
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
								      //Optional Error handling code end 

	blinkPin.begin(BLINK_PIN, SerialWombatDataSource::SW_DATA_SOURCE_PACKETS_RECEIVED   );  // Blink when the Serial Wombat chip receives a
												// Communications packet addressed to it
}




// In the loop we read the source voltage.  But we don't tell the LED to blink.  This
// happens in response to the command.
void loop() {
  Serial.println(sw.readSupplyVoltage_mV());
  delay(3000);
}
