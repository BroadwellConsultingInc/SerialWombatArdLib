#include "SerialWombat.h"
/*
This example shows how to interface an HC_SR04 ultrasonic distance sensor to a Serial Wombat 18AB chip.
The Serial Wombat chip will constantly take measurements, starting another measurement after the 
previous one finishes.  This allows the host to simply request the most recent reading over I2C without needing to
wait for a pulse to complete.

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
}

uint16_t lastMeasurement = 0;

// In the loop we will constantly read the I2C value, and print it to Serial When it changes
void loop() {
	uint16_t newMeasurement = distanceSensor.readPublicData();
	if (newMeasurement != lastMeasurement)
	{
		Serial.print(newMeasurement);
		Serial.println (" mm");		
		lastMeasurement = newMeasurement;
	}
	
}
