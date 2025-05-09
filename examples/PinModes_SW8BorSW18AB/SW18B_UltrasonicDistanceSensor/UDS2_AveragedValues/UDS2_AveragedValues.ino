#include "SerialWombat.h"
/*
This example builds on Example 1.  It sets the Serial Wombat Input Processor to average 100 pulses, and return that average.

Serial Wombat 18AB Firmware 2.1 or later is needed to use this example.

An HC_SR04 sensor needs to be powered by 5V, and outputs a 5V signal.  The echo pin should be connected to one of the 
Serial Wombat 18AB chip's 5V tolerant pins (9,10,11,12, 14 and 15)


A video demonstrating the use of the UltrasonicDistanceSensor pin mode on the Serial Wombat 18AB chip is available at:
TODO

Documentation for the SerialWombatUltrasonicDistanceSensor class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_ultrasonic_distance_sensor.html
*/

SerialWombatChip sw;
SerialWombatUltrasonicDistanceSensor distanceSensor(sw);
  

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
   

  distanceSensor.begin(10, // Echo pin is on pin 10
		       SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
			11); // Trigger pin on pin 11.    no parameters for autoTrigger (true) and pullUp (false)

  distanceSensor.writeAveragingNumberOfSamples(100); //Inherited from SerialWombatAbstractProcessedInput
}

uint16_t lastMeasurement;

// In the loop we will constantly read the average value, and print it to Serial When it changes
void loop() {
	uint16_t newMeasurement = distanceSensor.readAverage();
	if (newMeasurement != lastMeasurement)
	{
		Serial.print(newMeasurement);
		Serial.println (" mm");		
		lastMeasurement = newMeasurement;
	}
	
}
