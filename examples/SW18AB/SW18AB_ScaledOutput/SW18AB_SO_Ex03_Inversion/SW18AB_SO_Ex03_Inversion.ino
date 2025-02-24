#include "SerialWombat.h"
/*
This example shows how to use the Scaled Output function to make the output
of one pin (in this case a PWM pin) derive from the public value of another
pin (in this case an analog input) on the Serial Wombat 18AB chip.   Unlike
the prior example, in this example the output is inverted.

In this example a PWM output on pin 19 will vary duty cycle based on the 
value measured by an Analog Input on pin 0.  As the voltage on that pin 
approaches the Serial Wombat 18AB chip's source voltage the public data will
approach 65535.   This value will be read every mS by the PWM output pin's
scaled output module and the PWM will be updated leading to an output that will
effectively (when filtered) move in an opposite direction of the input voltage. 


A video demonstrating the use of the Scaled Output is available at:
TODO

Documentation for the SerialWombatAbstractScaledOutput class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_scaled_output.html
*/

SerialWombatChip sw;
SerialWombatAnalogInput_18AB sw18ABAnalog(sw);
SerialWombatPWM_18AB sw18ABPWM(sw);
  
#define PWM_PIN 19  
#define ANALOG_INPUT_PIN 0  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  sw.begin(Wire,i2cAddress); 
  versionCheck(); //see showErrorNotFound tab


  sw18ABAnalog.begin(ANALOG_INPUT_PIN);

  sw18ABPWM.begin(PWM_PIN);
  sw18ABPWM.writeScalingEnabled(true,ANALOG_INPUT_PIN);
sw18ABPWM.writeScalingInvertedInput(true); // Invert the incoming signal
   
}


//Loop code is empty.  All processing is begin done on the Serial Wombat 18AB chip

void loop() {

}
