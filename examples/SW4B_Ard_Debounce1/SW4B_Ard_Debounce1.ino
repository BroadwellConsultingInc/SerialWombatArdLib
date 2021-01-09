#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat
SerialWombatDebouncedInput redButton(sw);
SerialWombatDebouncedInput greenButton(sw);


// This example is explained in a video tutorial at: https://youtu.be/R1KM0J2Ug-M

void setup() {
  // put your setup code here, to run once:

  {//I2C Initialization
    Wire.begin();
    sw.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C.
  }
  
   redButton.begin(0);
   greenButton.begin(1);
  
   Serial.begin(115200);
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
