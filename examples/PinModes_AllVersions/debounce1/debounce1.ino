#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat
SerialWombatDebouncedInput redButton(sw);
SerialWombatDebouncedInput greenButton(sw);


// This example is explained in a video tutorial at: https://youtu.be/R1KM0J2Ug-M

void setup() {
  // put your setup code here, to run once:

    Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial

  
   redButton.begin(0);
   greenButton.begin(1);
  
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
