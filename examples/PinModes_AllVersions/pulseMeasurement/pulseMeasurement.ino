#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatPulseTimer steering(sw);
SerialWombatPulseTimer throttle(sw);
SerialWombatPulseTimer button(sw);
SerialWombatPulseTimer thumbSwitch(sw);

// This example is explained in a video tutorial at: https://youtu.be/YtQWUub9gYw

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  
   steering.begin(0);
   throttle.begin(1);
   button.begin(2);
   thumbSwitch.begin(3); // Change this to a different pin on 18AB, since 3 is SDA
   

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
