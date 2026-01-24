#include <SerialWombat.h>


// This example is explained in a video tutorial at: https://youtu.be/jVkQ1YoqcpI


SerialWombatChip sw;
SerialWombatPWM pwm(sw);
SerialWombatAnalogInput pot(sw);
#define PWMPIN 1
#define ANALOGPIN 2
#define POTHIGHPIN 3
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial


  pwm.begin(PWMPIN,0x8000);
  pot.begin(ANALOGPIN);
  sw.pinMode(POTHIGHPIN,OUTPUT);
  delay(5000);

  Serial.begin(115200); // For debug output.
}

void loop() {
  // put your main code here, to run repeatedly:

  sw.digitalWrite(POTHIGHPIN,LOW);  //Turn off POT high-side before going to sleep.
  sw.sleep();
  delay(5000);
  
  sw.wake();
  sw.digitalWrite(POTHIGHPIN,HIGH); //Turn on POT High side
  delay(250); // Give the Serial Wombat chip time to make readings
  Serial.println(pot.readVoltage_mV());
  delay(5000);

}
