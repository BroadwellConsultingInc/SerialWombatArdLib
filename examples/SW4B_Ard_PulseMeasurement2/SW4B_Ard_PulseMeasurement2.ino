#include <SerialWombat.h>

SerialWombat sw6C;    //Declare a Serial Wombat
SerialWombatPulseTimer steering(sw6C);
SerialWombatPulseTimer throttle(sw6C);
SerialWombatPulseTimer button(sw6C);
SerialWombatPulseTimer thumbSwitch(sw6C);

SerialWombat sw6D; // Declare a second Serial Wombat
SerialWombatPulseTimer leftKnob(sw6D);
SerialWombatPulseTimer rightKnob(sw6D);

// This example is explained in a video tutorial at: https://youtu.be/YtQWUub9gYw

void setup() {
  // put your setup code here, to run once:

  {//I2C Initialization
    Wire.begin();
    sw6C.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C.
    sw6D.begin(Wire,0x6D);  // Initialize the second Serial Wombat on address 6D
  }
  
   steering.begin(0); // On sw6C
   throttle.begin(1);
   button.begin(2);
   thumbSwitch.begin(3);

   leftKnob.begin(0); // On sw6D
   rightKnob.begin(1);
   
   Serial.begin(115200);
}

void clearTerminal()
{
    Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.println("[H");     // cursor to home command
}


void loop() {
    clearTerminal();
    Serial.println(steering.readHighCounts());
    Serial.println(throttle.readHighCounts());
    Serial.println(button.readHighCounts());
    Serial.println(thumbSwitch.readHighCounts());
    Serial.println(leftKnob.readHighCounts());
    Serial.println(rightKnob.readHighCounts());
    
    delay(50);
 
}
