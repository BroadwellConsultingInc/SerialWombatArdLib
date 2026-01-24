#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat
SerialWombatDebouncedInput redButton(sw);
SerialWombatDebouncedInput greenButton(sw);

#define RED_BUTTON_PIN 0
#define GREEN_BUTTON_PIN 1

// This example is explained in a video tutorial at: https://youtu.be/R1KM0J2Ug-M

void setup() {
	// put your setup code here, to run once:
	Wire.begin();   //Initialize the I2C Bus on default pins

	Serial.begin(115200);
	delay(3000);
	Serial.println("Two Button Debounce Example");


	sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

	//Optional Error handling code begin:
	if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
	if(sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_DEBOUNCE))
	{
		Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
		while(1){delay(100);}
	}
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
								      //Optional Error handling code end 


	redButton.begin(RED_BUTTON_PIN);  // initialize with default settings, including internal pull up
	greenButton.begin(GREEN_BUTTON_PIN);

}

void clearTerminal()
{
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}

int greenTransitions = 0;
int redTransitions = 0;

void loop() {
    clearTerminal();
    
    redButton.readTransitionsState();
    redTransitions += redButton.transitions;
    
    greenButton.readTransitionsState();
    greenTransitions += greenButton.transitions;
    
    Serial.print(greenTransitions);
    Serial.print(" ");
    Serial.println(greenButton.readDurationInTrueState_mS());

    Serial.print(redTransitions);
    Serial.print(" ");    
    Serial.println(redButton.readDurationInTrueState_mS());
   
    
    delay(50);
 
}

