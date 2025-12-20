// There is an tutorial video to go with this example at https://youtu.be/uFLlIoolQ_M

#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip


#define GREEN_LED_SW_PIN 2
void setup() {

	Serial.begin(115200);
	Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
	sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.

	// Check for proper Serial Wombat version and pin mode, set error handler.  Not necessary, but useful for debugging problems
	errorChecking();  

	sw.pinMode(GREEN_LED_SW_PIN,OUTPUT);

}

void loop() {
  
  // put your main code here, to run repeatedly:

  sw.digitalWrite(GREEN_LED_SW_PIN,HIGH);
  delay(1000);
  sw.digitalWrite(GREEN_LED_SW_PIN,LOW);
  delay(1000);
  
}
