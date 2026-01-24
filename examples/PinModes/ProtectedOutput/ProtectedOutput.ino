#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip

SerialWombatProtectedOutput swpo(sw);
SerialWombatAnalogInput Feedback(sw);

// This example is explained in a video tutorial at: https://youtu.be/p8CO04C1q_Y

void setup() {
	// put your setup code here, to run once:
	Wire.begin();   //Initialize the I2C Bus on default pins

	Serial.begin(115200);
	delay(3000);
	Serial.println("Protected Output Example");


	sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

	//Optional Error handling code begin:

	if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
	if(sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_ANALOGINPUT) && sw.isPinModeSupported(PIN_MODE_PROTECTED_OUTPUT)))
	{
		Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
		while(1){delay(100);}
	}
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
								      //Optional Error handling code end 



	swpo.begin(3,1);  // Controlling pin 3.   Feedback from pin 1.
	Feedback.begin(1);  // Begin analog reading on pin 1
}


int i;
void loop() {
      if(swpo.isInSafeState())
     {
      Serial.println("Protected Output Fault Detected, Output set to Safe State!");
     }
     if (i & 0x01)
     {
        swpo.configure(PO_FAULT_IF_FEEDBACK_GREATER_THAN_EXPECTED,8000,10,SW_HIGH,SW_LOW);
        Serial.println("On");
     }
     else
     {
        swpo.digitalWrite(LOW);
        Serial.println("Off");
     }

     delay(100);
     Serial.print ("counts at drain: ");
     Serial.println(Feedback.readCounts());
     Serial.print(Feedback.readVoltage_mV());
     Serial.println(" mV");
     Serial.println();
     
     delay(3000);
     ++i;
}
