#include "SerialWombat.h"
/*
This example shows how to set up a pin-to-pin interaction within the Serial Wombat Chip using
the Ultrasonic Distance Sensor and a strip of WS2812 LEDs. This example builds on example 2
by adding the WS2812 strip.

The LEDs will light depending on the distance measurement from the sensor.

This sketch was last tested with version 2.2.2 of the firmware.

An HC_SR04 sensor needs to be powered by 5V, and outputs a 5V signal.  The echo pin should be connected to one of the 
Serial Wombat 18AB chip's 5V tolerant pins (9,10,11,12, 14 and 15)

A video demonstrating the use of the UltrasonicDistanceSensor pin mode on the Serial Wombat 18AB chip is available at:
https://youtu.be/Mv7zrP8mtjo

Documentation for the SerialWombatUltrasonicDistanceSensor class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_ultrasonic_distance_sensor.html

For reference, the source code to the firmware (looking at this isn't required, but is interesting) is available here:
https://github.com/BroadwellConsultingInc/SerialWombat/blob/main/SerialWombatPinModes/ultrasonicDistance.c

*/

SerialWombatChip sw;
SerialWombatUltrasonicDistanceSensor distanceSensor(sw);
SerialWombatWS2812 lightStrip(sw);

#define WS2812_PIN 19  // Must be an enhanced performance pin: 0,1,2,3,4,7,9,10-19
#define NUMBER_OF_LEDS 20
#define WS2812_USER_BUFFER_INDEX 0x0000  // Set this to an index into the on-chip user buffer.  Can't overlap with area used by other pins.

  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

 //Find the Serial Wombat Chip on the I2C bus and display its firmware version
  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
   

  distanceSensor.begin(10, // Echo pin is on pin 10
           SerialWombatUltrasonicDistanceSensor::HC_SR04,  // HC_SR04 driver
      11); // Trigger pin on pin 11.    no parameters for autoTrigger (true) and pullUp (false)

  distanceSensor.writeAveragingNumberOfSamples(100); //Inherited from SerialWombatAbstractProcessedInput

    lightStrip.begin(WS2812_PIN,
    NUMBER_OF_LEDS,
    WS2812_USER_BUFFER_INDEX);

    lightStrip.barGraph(10,  // Get the bar graph data from pin 10, the Ultrasonic Sensor
    0x00050000,   // Off value = dim red
    0x00000020,   // on value = blue
    0,         // Min value 0
    300);       // Max value 300mm

}

uint16_t lastMeasurement;

// In the loop we will constantly read the average value, and print it to Serial When it changes
// Note that the loop does nothing to update the WS2812 strip.  This happens completely on the
// Serial Wombat Chip.

void loop() {
  uint16_t newMeasurement = distanceSensor.readAverage();
  if (newMeasurement != lastMeasurement)
  {
    Serial.print(newMeasurement);
    Serial.println (" mm");   
    lastMeasurement = newMeasurement;
  }
  
}
