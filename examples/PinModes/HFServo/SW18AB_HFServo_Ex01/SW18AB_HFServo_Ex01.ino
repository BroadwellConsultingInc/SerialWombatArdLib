#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatServo BoringNormalServo(sw);
SerialWombatHighFrequencyServo ExcitingHighFrequencyServo(sw);

void setup() {
	// put your setup code here, to run once:
	Wire.begin();   //Initialize the I2C Bus on default pins

	Serial.begin(115200);
	delay(3000);
	Serial.println("High Speed Servo Example");


	sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

	//Optional Error handling code begin:
	if(!sw.isSW18())
	{
		Serial.println("This Example is not supported on the Serial Wombat 4B or 8B chip.  An  18AB chip is required.");
		while(1){delay(100);}
	}
	if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
	if(sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_HS_SERVO))
	{
		Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
		while(1){delay(100);}
	}
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
								      //Optional Error handling code end 

ExcitingHighFrequencyServo.attach(19,500,1000);
ExcitingHighFrequencyServo.writeFrequency_Hz(560);
BoringNormalServo.attach(18);

}

uint16_t position = 0;
void loop() {
  // put your main code here, to run repeatedly:
position += 1000;
ExcitingHighFrequencyServo.write16bit(position);
BoringNormalServo.write16bit(position);
delay(50);
}
