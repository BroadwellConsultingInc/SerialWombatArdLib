#include "SerialWombat.h"
/*
This example shows how to generate varying frequencies with the Serial Wombat 18AB's frequency generator pin mode.  

Only one pin on the Serial Wombat 18AB chip can be configured to this mode at a time.

A video demonstrating the use of the High Speed Clock Generator is available at:
TODO

Documentation for the SerialWombatHighSpeedClock class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_h_s_clock.htmll
*/

SerialWombatChip sw;
SerialWombatFrequencyOutput freqOutput(sw);
SerialWombat18ABOscillatorTuner oscTun(sw);
  
#define FREQUENCY_PIN 19  //Must be an enhanced digital capability pin.

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

  //Find the Serial Wombat Chip on the I2C bus and display its firmware version
  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  sw.begin(Wire,i2cAddress); 
  sw.queryVersion();
  Serial.println();
  Serial.print("Version "); Serial.println((char*)sw.fwVersion);
  versionCheck(); //see showErrorNotFound tab
  Serial.println("SW18AB Found.");


  freqOutput.begin(19); // 
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
