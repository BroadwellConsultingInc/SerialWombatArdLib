#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat
SerialWombatPulseTimer steering(sw);
SerialWombatPulseTimer throttle(sw);
SerialWombatPulseTimer button(sw);
SerialWombatPulseTimer thumbSwitch(sw);

// This example is explained in a video tutorial at: https://youtu.be/YtQWUub9gYw

void setup() {
  // put your setup code here, to run once:

  {//I2C Initialization
    Wire.begin();
    sw.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C.
  }
  
   steering.begin(0);
   throttle.begin(1);
   button.begin(2);
   thumbSwitch.begin(3);
   
   Serial.begin(115200);
}

void clearTerminal()
{
    Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}

int i;
void loop() {
    clearTerminal();
    Serial.println(steering.readHighCounts());
    Serial.println(throttle.readHighCounts());
    Serial.println(button.readHighCounts());
    Serial.println(thumbSwitch.readHighCounts());
    
    delay(50);
 
}
