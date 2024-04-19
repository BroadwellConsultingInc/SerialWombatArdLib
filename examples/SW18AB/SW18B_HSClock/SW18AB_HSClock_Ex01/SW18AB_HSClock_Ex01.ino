#include "SerialWombat.h"
/*
This example shows how to generate a high speed signal using the Serial Wombat 18AB chip's high speed clock generator.

Only one pin on the Serial Wombat 18AB chip can be configured to this mode at a time.

A video demonstrating the use of the High Speed Clock Generator is available at:
TODO

Documentation for the SerialWombatHighSpeedClock class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_h_s_clock.htmll
*/

SerialWombatChip sw;
SerialWombat18ABOscillatorTuner oscTun(sw);
SerialWombatHSClock hsClock(sw);
  
#define HSCLOCK_PIN 10  //Must be an enhanced digital capability pin.

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


  hsClock.begin(10, 2000000); // 2MHz output.  Note that only integer divisors of 32MHz are possible.  See documentation
   
  
}



void loop() {
	oscTun.update(); // Call this periodically to improve oscillator frequency accuracy.  See https://youtu.be/T2uBQM3s_JM

}
