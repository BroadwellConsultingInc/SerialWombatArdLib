#include "SerialWombat.h"
/*
This example shows how to use the Scaled Output function to filter the output of a Scaled Output pin.

In this case the host will make occasional relatively large updates to the duty cycle of a PWM output.

The Serial Wombat 18AB Chip's Scaled Output will use a first order filter to smooth the transtion.   

This functionality is useful to limit how fast an output can change and allow smooth approach of a final position or setting while still reacting to each incoming control point as it changes

In this example the host will alternate between full off and full on every 5 seconds, but the Serial Wombat chip will smooth that output using a .98 filter constant (64224) updated every 32mS

Note that the actual output may have a different update rate (50 Hz for servos, a PWM frequency, etc) than the period selected, and this will also affect how
the output physically behaves.

A video demonstrating the use of the Scaled Output is available at:
TODO

https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_scaled_output.html
*/

SerialWombatChip sw;
SerialWombatPWM_18AB sw18ABPWM(sw);
  
#define PWM_PIN 1  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Output scaling filtered output control example");


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

  sw18ABPWM.begin(PWM_PIN);
  sw18ABPWM.writeScalingEnabled(true,PWM_PIN);
  sw18ABPWM.write1stOrderFiltering(SerialWombatAbstractScaledOutput::Period::PERIOD_1mS, 64224); 
}




void loop() {

	sw18ABPWM.writePublicData(0);
	delay(5000); 
	sw18ABPWM.writePublicData(65535);
	delay(5000); 
}
