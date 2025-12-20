#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat
SerialWombatDebouncedInput redButton(sw);
SerialWombatDebouncedInput greenButton(sw);

#define RED_BUTTON_PIN 0
#define GREEN_BUTTON_PIN 1

// This example is explained in a video tutorial at: https://youtu.be/R1KM0J2Ug-M

void setup() 
{
	// put your setup code here, to run once:

	Serial.begin(115200);
	Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
	sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.

	// Check for proper Serial Wombat version and pin mode, set error handler.  Not necessary, but useful for debugging problems
	errorChecking();  


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

