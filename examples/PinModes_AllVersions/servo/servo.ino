#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatServo ContinuousServo(sw);  // Declare a Servo on pin 2 of Serial Wombat sw
SerialWombatServo StandardServo(sw);   // Declare a Servo on pin 3 of Serial Wombat sw

// A video tutorial is available which explains this example in detail at: https://youtu.be/WiciAtS1ng0
void setup() {
 
 Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial


  ContinuousServo.attach(2,500,2500,true); // Initialize a servo on pin 2, 500uS minimum pulse, 2500 us Maximum pulse, reversed
  StandardServo.attach(3);  // Initialize a servo on pin 3 using Arduino equivalent default values.  Use a different pin on SW18AB, since 3 is SDA

}

void loop() {
  
  // put your main code here, to run repeatedly:

  ContinuousServo.write(30);       // Takes a number from 0 to 180
  StandardServo.write16bit(5500);  // Takes a number from 0 to 65535:  Higher resolution
  delay(5000);
  ContinuousServo.write(140);
  StandardServo.write16bit(50000);
  delay(5000);
  
}
