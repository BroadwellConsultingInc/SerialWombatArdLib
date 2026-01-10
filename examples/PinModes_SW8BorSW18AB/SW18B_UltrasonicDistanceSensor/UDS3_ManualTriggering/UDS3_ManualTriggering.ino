#include "SerialWombat.h"
/*
This example shows how to manually trigger ultrasonic distance sensor measurements.  This is useful if you don't want
multiple sensors runing simultaneously as they may interfere with each other.

In this example we will use 6 sensors, one for straight ahead, one for forward left, one for left, one for foward right,
and one for reverse.  

The host will trigger each sensor in succession and print the name of the sensor if an object is near (less than 100mm)

This sketch assumes an 18AB chip due to the number of pins required, but could be adapted to use 4 sensors on a Serial Wombat 8B

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
SerialWombatUltrasonicDistanceSensor rearSensor(sw),leftSensor(sw),flSensor(sw),frontSensor(sw),frSensor(sw),rightSensor(sw);
  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

 //Find the Serial Wombat Chip on the I2C bus and display its firmware version
  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
   
  //Initialize all the sensors without autotrigger
  leftSensor.begin(10, // Echo pin is on pin 10
		       SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
			16,// Trigger pin on pin 11.
			false);no Autotrigger,      no parameters for  pullUp (false)

  flSensor.begin(11, // Echo pin is on pin 11
           SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
      17,// Trigger pin on pin 17.
      false);no Autotrigger,      no parameters for  pullUp (false)

  frontSensor.begin(12, // Echo pin is on pin 12
           SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
      18,// Trigger pin on pin 18.
      false);no Autotrigger,      no parameters for  pullUp (false)

  flSensor.begin(14, // Echo pin is on pin 14
           SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
      13,// Trigger pin on pin 13.
      false);no Autotrigger,      no parameters for  pullUp (false)

  rightSensor.begin(15, // Echo pin is on pin 14
           SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
      19,// Trigger pin on pin 19.
      false);no Autotrigger,      no parameters for  pullUp (false)

  
}


#define DETECTION_DISTANCE_MM 100
#define MEASUREMENT_TIME_mS 50

// In the loop we will constantly read the average value, and print it to Serial When it changes
void loop() {
      leftSensor.manualTrigger();
      delay(MEASUREMENT_TIME_mS);  // Allow time for measurement
      if (leftSensor.readPublicData() < DETECTION_DISTANCE_MM) Serial.print("OBJECT LEFT ");

      flSensor.manualTrigger();
      delay(MEASUREMENT_TIME_mS);  // Allow time for measurement
      if (flSensor.readPublicData() < DETECTION_DISTANCE_MM) Serial.println("OBJECT FRONTLEFT ");

      frontSensor.manualTrigger();
      delay(MEASUREMENT_TIME_mS);  // Allow time for measurement 
      if (frontSensor.readPublicData() < DETECTION_DISTANCE_MM) Serial.println("OBJECT FRONT ");

      frSensor.manualTrigger();
      delay(MEASUREMENT_TIME_mS);  // Allow time for measurement 
      if (frSensor.readPublicData() < DETECTION_DISTANCE_MM) Serial.println("OBJECT FRONTRIGHT ");

      rightSensor.manualTrigger();
      delay(MEASUREMENT_TIME_mS);  // Allow time for measurement 
      if (rightSensor.readPublicData() < DETECTION_DISTANCE_MM) Serial.println("OBJECT RIGHT ");

      rearSensor.manualTrigger();
      delay(MEASUREMENT_TIME_mS);  // Allow time for measurement 
      if (rearSensor.readPublicData() < DETECTION_DISTANCE_MM) Serial.println("OBJECT REAR ");


}
