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
   Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
 if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  sw.begin(Wire,i2cAddress); 
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  
 sw.queryVersion();
  Serial.println();
  Serial.print("Version "); Serial.println((char*)sw.fwVersion);
  versionCheck(); //see showErrorNotFound tab
  Serial.println("SW18AB Found.");


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
