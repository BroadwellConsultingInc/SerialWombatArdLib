/*
This example shows how to use the PCB0031 Grip's servo voltage measurement capabilites.

It assumes that the Pin 3 Voltage Measurement solder jumpers have been soldered closed.  This is not the default.

Be sure to use pull up resistors on the I2C bus or enable the on board pull ups
on the Grip board via the solder jumper.

The Grip board must be provided with 5V electronics input for proper operation of the current sensors.

A video demonstrating this example is available at 
https://youtu.be/TODO

Documentation for the Grip board classes are available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_p_c_b0031___grip.html


*/


#include <SerialWombat.h>

#define GRIP_I2C_ADDRESS 0x60
PCB0031_Grip grip;


void setup() {
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(500);

  grip.begin(0x60,true); // Note that this must be true to enable voltage measurement
  grip.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!grip.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}

  Serial.println("Vary the servo input voltage to observe it change.");
}

void loop() {

  delay(500);

  uint16_t logicVoltage = grip.readSupplyVoltage_mV();
  uint16_t servoVoltage = grip.readPowerVoltage_mv();
 
  char c[80];
  sprintf(c,"Logic: %d mV  servo: %d mV", logicVoltage, servoVoltage);

  Serial.println(c);
}
