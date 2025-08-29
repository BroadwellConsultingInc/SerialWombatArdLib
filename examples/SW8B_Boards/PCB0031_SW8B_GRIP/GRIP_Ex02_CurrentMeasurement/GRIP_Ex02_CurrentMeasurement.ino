#include <SerialWombat.h>

/*
This video shows how to use the PCB0031 Grip's current measurement capabilites.

It assumes that there is a servo attached to pin 0.

The sketch centers the servo.  The user can then push against the servo and observe changes in the Servo's
current draw on the Serial Monitor or Serial Plotter


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

  grip.begin(0x60);
  grip.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!grip.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}

   Serial.println("Allow the servo to move freely to center position");
   grip.gs0.writePublicData(0x8000);
    
   delay(2000);
   Serial.println("Calibrating Current...");
   grip.gs0.sensor.calibrateIdleCurrent();
   Serial.println("Calibrated");
   Serial.println("Push against the servo horn to observe changes in current.");
    
}

void loop() {

  delay(500);

  int16_t x = grip.gs0.sensor.readCurrent_mA();
 
  char c[80];
  sprintf(c," %d mA", x);

  Serial.println(c);
}
