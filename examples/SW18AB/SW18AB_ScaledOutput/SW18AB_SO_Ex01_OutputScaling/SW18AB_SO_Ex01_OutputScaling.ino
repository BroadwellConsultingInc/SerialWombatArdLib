#include "SerialWombat.h"
/*
This example shows how to use the Scaled Output function to scale the output of a PWM pin
so that an input range drives a different output range.  In this example an input  value of 0 to 65535
will cause a duty cycle output of  0 to 27306 (0 to 41 percent).  This would be useful if a 5V motor was
being driven from 12V through a FET.  In this case 0 to 65535 input would still represent "full scale"
but full scale would now be an output between 0 and 5V.


A video demonstrating the use of the Scaled Output is available at:
TODO

https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_scaled_output.html
*/

SerialWombatChip sw;
SerialWombatPWM_18AB sw18ABPWM(sw);
  
#define PWM_PIN 19  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  sw.begin(Wire,i2cAddress); 
  versionCheck(); //see showErrorNotFound tab


  sw18ABPWM.begin(PWM_PIN);
	sw18ABPWM.writeScalingEnabled(true,PWM_PIN);
	sw18ABPWM.writeOutputScaling(0, // Minimum output	
 			27306); //Maximum output 27306 = 65535 * 12 / 5
  
   
}



// This loop will cause a 3 second ramp of the PWM from 0 to 5V equivalent

uint16_t nextPWMOutput = 0;

void loop() {
	nextPWMOutput += 1000; // Will roll over after 65535, leading to ramping output
	sw18ABPWM.writePublicData(nextPWMOutput);
	delay(50); // 20 updates per second, leading to about a 3 second ramp.

}
