#include "SerialWombat.h"
/*
  This example shows how to generate varying frequencies with the Serial Wombat 18AB's frequency generator pin mode.


  A video demonstrating the use of the Frequency Output is  available at:
  TODO

  Documentation for the SerialWombatHighSpeedClock class is available at:
  https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_h_s_clock.html
*/

SerialWombatChip sw;
SerialWombatFrequencyOutput freqOutput(sw);
SerialWombat18ABOscillatorTuner oscTun(sw);

#define FREQUENCY_PIN 7

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("1Hz Blink Example");


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
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_FREQUENCY_OUTPUT))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end


  freqOutput.begin(FREQUENCY_PIN); //
}



void loop() {
  oscTun.update(); // Call this periodically to improve oscillator frequency accuracy.  See https://youtu.be/T2uBQM3s_JM

  Serial.println("2000 Hz");
  freqOutput.writePublicData(2000);
  delay(1000);
  Serial.println("1000 Hz");
  freqOutput.writePublicData(1000);
  delay(1000);
  Serial.println("Off");
  freqOutput.writePublicData(0);
  delay(10000);


}
