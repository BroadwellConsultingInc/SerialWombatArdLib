// There is an tutorial video to go with this example at https://youtu.be/uFLlIoolQ_M

#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip


#define GREEN_LED_SW_PIN 2
void setup() {
	// put your setup code here, to run once:
	Wire.begin();   //Initialize the I2C Bus on default pins

	Serial.begin(115200);
	delay(3000);
	Serial.println("Digital Output Example (Arduino Style)");


	sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

	//Optional Error handling code begin:
	if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
	if(!sw.isPinModeSupported(PIN_MODE_DIGITALIO))
	{
		Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
		while(1){delay(100);}
	}
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
								      //Optional Error handling code end 


	sw.pinMode(GREEN_LED_SW_PIN,OUTPUT);

}

void loop() {
  
  // put your main code here, to run repeatedly:

  sw.digitalWrite(GREEN_LED_SW_PIN,HIGH);
  delay(1000);
  sw.digitalWrite(GREEN_LED_SW_PIN,LOW);
  delay(1000);
  
}
