#include "SerialWombat.h"
/*
This example shows how to measure the frequency and pulse count of a high speed signal 
with the High Speed Counter pin mode 

Only one pin on the Serial Wombat 18AB chip can be configured to this mode at a time.

For this example, we will generate a high speed signal on pin 19, and
measure it on pin 18.  These pins  need to be connected with a wire.

It is suggested to use the SerialWombat18ABOscillator tuner when using
this pin mode to improve accuracy

A video demonstrating the use of the High Counter is available at:
TODO

Documentation for the SerialWombatHighCounter class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_h_s_clock.htmll
*/

SerialWombatChip sw;
SerialWombat18ABOscillatorTuner oscTun(sw);
SerialWombatHSClock hsClock(sw);
SerialWombatHSCounter hsCounter(sw);
  
#define HSCLOCK_PIN 19  //Must be an enhanced digital capability pin.
#define HSCOUNTER_PIN 18  //Must be an enhanced digital capability pin.

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


  hsClock.begin(HSCLOCK_PIN, 2000000); // 2MHz output.  Note that only integer divisors of 32MHz are possible.  See documentation
   
 hsCounter.begin( HSCOUNTER_PIN);
}



void loop() {
delay(2000);
	oscTun.update(); // Call this periodically to improve oscillator frequency accuracy.  See https://youtu.be/T2uBQM3s_JM
Serial.println(hsCounter.readFrequency());

}
