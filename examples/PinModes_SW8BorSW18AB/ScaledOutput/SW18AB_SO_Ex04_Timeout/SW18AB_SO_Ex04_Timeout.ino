#include "SerialWombat.h"
/*
This example shows how to use the Scaled Output function to make an output
default to a value if the host does not periodically check in either by
explicitly resetting a timer.

This functionality is useful to set an output to a default value if the host ceases communicating.  This function comes after any feedback control such as PID, so it will override the controller's output

In this example the PWM output will be varied between about 50 percent (32000) and
nearly 100 percent (64000) by the host.  Then the host will stop communicating, and the Serial Wombat 18AB Scaled output will set the output to a predefined value of 10 percent (6554) about 250 mS after the last timer reset call.

A video demonstrating the use of the Scaled Output is available at:
TODO

https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_scaled_output.html
*/

SerialWombatChip sw;
SerialWombatPWM_18AB sw18ABPWM(sw);
  
#define PWM_PIN 1  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  sw.begin(Wire,i2cAddress); 
  versionCheck(); //see showErrorNotFound tab
sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial


  sw18ABPWM.begin(PWM_PIN);
	sw18ABPWM.writeScalingEnabled(true,PWM_PIN);
  sw18ABPWM.writeTimeout(250 , // timeout if this amount of time elapses without calling this function again
  6554); // 10 percent duty cycle on timeout
}




void loop() {

uint16_t nextPWMOutput;
       
        for (nextPWMOutput = 32768; nextPWMOutput< 64000; nextPWMOutput += 1000)
{
	sw18ABPWM.writePublicData(nextPWMOutput);
        sw18ABPWM.writeTimeout(250,6554);
	delay(100); // 10 updates per second, leading to about a 3 second ramp.
}
	delay(5000); // The scaled output timer will timeout and set the output to 10 percent

}
