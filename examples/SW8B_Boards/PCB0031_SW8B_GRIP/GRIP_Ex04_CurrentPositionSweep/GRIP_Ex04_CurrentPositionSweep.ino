/*
This example shows how to use the PCB0031 Grip's current measurement capabilites.

It assumes that there is a servo attached to pin 0.

This sketch sweeps the servo across its range and takes current measurements at
each point.  This is useful for determining the range of motion of a physical
system that has end stops inside of the servo range of motion or for characterizing the expected current draw of a robot arm that moves vertically where the
force required varies with the angle due to gravity and lever arm angle

This example assumes the servo is attached to pin 0.  For pins 1 2 or 3 change
gs0 to gs1, gs2, or gs3 .

Be sure to use pull up resistors on the I2C bus or enable the on board pull ups
on the Grip board via the solder jumper.

The Grip board must be provided with 5V electronics input for proper operation of the current sensors.

A video demonstrating this example is available at 
https://youtu.be/TODO

Documentation for the Grip board classes are available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_t_m1637.html#details

*/

#include <SerialWombat.h>

#define GRIP_I2C_ADDRESS 0x60
PCB0031_Grip grip;


void setup() {
  
  Wire.begin();
  
  Serial.begin(115200);

  delay(500);

  grip.begin(0x60, true);
  grip.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if (!grip.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }

  uint32_t position = 0;

  uint16_t minCurrent = 65535;
  uint16_t minCurrentPosition = 0;
  for (position = 0; position <= 65000; position += 1000)
  {
    grip.gs0.writePublicData((uint16_t)position);
    delay(250);
    uint16_t current = grip.gs0.sensor.readAveragedCounts();
    int16_t current_mA = grip.gs0.sensor.readCurrent_mA();
    if (current < minCurrent)
    {
      minCurrent = current;
      minCurrentPosition = position;
    }
    char s[50];
    sprintf(s,"Pos: %lu Current: %u counts, %d mA",position,current, current_mA );
    Serial.println(s);  
  }

	//Return the servo to its minimum current rest position.
   
    grip.gs0.writePublicData(minCurrentPosition);
  

}

void loop() {
 
}
