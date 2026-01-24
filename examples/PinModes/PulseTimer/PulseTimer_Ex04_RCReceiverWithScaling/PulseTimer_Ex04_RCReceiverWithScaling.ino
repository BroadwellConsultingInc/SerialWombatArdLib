/*
This example shows how to measure servo pulses from a radio control reciever on a Serial Wombat 8B or 18AB chip.  

IMPORTANT:   This example requires firmware version 2.1.1 or later to work.

This example assumes 6 channels of RC receiver hooked up to pins 14 through 19.  The measured pulse length  is
printed to Serial as a proportion of its range (0 to 65535).

This sketch makes use of the SerialWombat18ABOscillatorTuner to improve the meaurement accuracy.  About 1 minute of
runtime is required after reset to achieve full accuracy improvement.

A video demonstrating the use of the SerialWombatPulseTimer_18AB class for RC measurement  on the Serial Wombat 18AB chip is available at:
TBD

Documentation for the SerialWombatPulseTimer_18AB Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_pulse_timer__18_a_b.html

Documentation for the SerialWombatAbstractProcessedInput class that provides the scaling for SerialWombatPulseTimer_18AB
is available here:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_processed_input.html


*/

#include <SerialWombat.h>

SerialWombatChip sw;

SerialWombatPulseTimer_18AB rcWheel(sw),rcThrottle(sw),rcSwitch3(sw),rcSwitch4(sw),rcKnob5(sw),rcKnob6(sw);

SerialWombat18ABOscillatorTuner oscTun(sw);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("6 Channel RC Receiver Pulse Measurement with scaling Example");


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
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_PULSETIMER))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  rcWheel.begin(14);
  rcWheel.writeProcessedInputEnable(true);
  rcWheel.writeTransformScaleRange(925,2115);  // See YouTube video for origin of numbers
  
  rcThrottle.begin(15);
  rcThrottle.writeProcessedInputEnable(true);
  rcThrottle.writeTransformScaleRange(890,2100);
  
  rcSwitch3.begin(16);
  rcSwitch3.writeProcessedInputEnable(true);
  rcSwitch3.writeTransformScaleRange(1500,1501);
  
  rcSwitch4.begin(17);
  rcSwitch4.writeProcessedInputEnable(true);
  rcSwitch4.writeTransformScaleRange(1350,1650);
  
  rcKnob5.begin(18);
  rcKnob5.writeProcessedInputEnable(true);
  rcKnob5.writeTransformScaleRange(925,2075);
  
  rcKnob6.begin(19);
  rcKnob6.writeProcessedInputEnable(true);
  rcKnob6.writeTransformScaleRange(925,2075);

}

void loop() {
  // put your main code here, to run repeatedly:
  char s[80];
  sprintf(s,"%5d %5d %5d %5d %5d %5d",rcWheel.readPublicData(), rcThrottle.readPublicData(), rcSwitch3.readPublicData(),
        rcSwitch4.readPublicData(),rcKnob5.readPublicData(),rcKnob6.readPublicData());
  Serial.println(s);
        delay(200);
  oscTun.update();

}
