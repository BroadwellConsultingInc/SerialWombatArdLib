#include "SerialWombat.h"
/*
This example shows how to use the Scaled Output function to smooth the output of a Scaled Output pin.

In this case the host will make occasional relatively large updates to the duty cycle of a PWM output.

The Serial Wombat 18AB Chip's Scaled Output unit will limit how many counts the output can change within a given period
(in this case 33 in 1ms, effectivly allowing a full scale transition of 0-65535 in two seconds).

This functionality is useful to limit how fast an output can change.  This could poentially be used to limit the speed of a servo, smooth out
jerky motor power transitions, automatically dim LEDs, etc.   

In this example the host will alternate between full off and full on every 5 seconds, but the Serial Wombat chip will smooth that output over 2 seconds.

Note that the actual output may have a different update rate (50 Hz for servos, a PWM frequency, etc) than the period selected, and this will also affect how
the output physically behaves.

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
  sw18ABPWM.writeTimeout(SerialWombatAbstractScaledOutput::Period::PERIOD_1mS, 33); // Allow 33 counts of change per ms.
}




void loop() {

	sw18ABPWM.writePublicData(0);
	delay(5000); 
	sw18ABPWM.writePublicData(65535);
	delay(5000); 
}
