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
  
#define PWM_PIN 1  
#define ANALOG_INPUT_PIN 0  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Output scaling pin to pin with inversion example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (sw.isSW04())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_PWM) && sw.isPinModeSupported(PIN_MODE_ANALOGINPUT)))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end


  sw18ABAnalog.begin(ANALOG_INPUT_PIN);

  sw18ABPWM.begin(PWM_PIN);
  sw18ABPWM.writeScalingEnabled(true,ANALOG_INPUT_PIN);
sw18ABPWM.writeScalingInvertedInput(true); // Invert the incoming signal
   
}


//Loop code is empty.  All processing is begin done on the Serial Wombat chip

void loop() {

}
