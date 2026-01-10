#include "SerialWombat.h"
/*
This example builds on Example 1.  It sets the Serial Wombat Input Processor to average 100 pulses, and return that average.

This sketch was last tested with version 2.2.2 of the firmware.

The Serial Wombat 8B must be loaded with the UltrasonicDistanceSensor firmware build, or other build that includes
ultrasonic distance sensor and servo pin modes.

An HC_SR04 sensor needs to be powered by 5V, and outputs a 5V signal.  The echo pin should be connected to one of the 
Serial Wombat 18AB chip's 5V tolerant pins (9,10,11,12, 14 and 15).  The trigger pin can be any pin.  

A Serial Wombat 8B chip should be powered by 5V to accomodate the 5V input.

See this video on combining 5V SW8B's with 3.3V logic (such as ESP32):
https://www.youtube.com/watch?v=kaUU5FH0hvc


A video demonstrating the use of the UltrasonicDistanceSensor pin mode on the Serial Wombat 18AB chip is available at:
https://youtu.be/Mv7zrP8mtjo

Documentation for the SerialWombatUltrasonicDistanceSensor class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_ultrasonic_distance_sensor.html

For reference, the source code to the firmware (looking at this isn't required, but is interesting) is available here:
https://github.com/BroadwellConsultingInc/SerialWombat/blob/main/SerialWombatPinModes/ultrasonicDistance.c

*/

SerialWombatChip sw;
SerialWombatUltrasonicDistanceSensor distanceSensor(sw);
  
//Comment the following in for SW18AB  (you can use most pins, but be careful with 5V ones)

//#define UDS_ECHO_PIN 10 //On SW18AB this should be a 5V capable pin
//#define UDS_TRIGGER_PIN 11 //  Can be 5 or 3.3V pin

//Comment the following in for SW8B (Run the chip on 5V, since there's a 5V input).  
// Any pin can be used for any function, but the 50k pull down on pin 0 may 
// be problematic if the echo pin has a weak pull up, so suggest using 0 for 
// trigger rather than Echo.

#define UDS_ECHO_PIN 4 // Probably best not to use pin 0 for this one
#define UDS_TRIGGER_PIN 5 

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

  //Find the Serial Wombat Chip on the I2C bus and display its firmware version
  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}


   

  distanceSensor.begin(UDS_ECHO_PIN,
           SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
      UDS_TRIGGER_PIN); //    no parameters for autoTrigger (true) and pullUp (false)
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
