/*
This example shows how to use the PCB0031 Grip's rate limiting control to offload servo movement speed control to the Serial Wombat firmware

This example assumes the servo is attached to pin 0.  For pins 1 2 or 3 change
gs0 to gs1, gs2, or gs3 .


Be sure to use pull up resistors on the I2C bus or enable the on board pull ups
on the Grip board via the solder jumper.


A video demonstrating this example is available at 
https://youtu.be/TODO

Documentation for the Grip board classes are available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_p_c_b0031___grip.html


*/
#include <SerialWombat.h>

#define GRIP_I2C_ADDRESS 0x60
PCB0031_Grip grip;

#define SERVO_MOVEMENT_INCREASE_RATE 1000
#define SERVO_MOVEMENT_DECREASE_RATE 200


void setup() {
  Wire.begin();

  Serial.begin(115200);

  delay(500);

  grip.begin(0x60, true);
  grip.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if (!grip.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }



  // Output Scaling configurations for this pin
  grip.gs0.writeScalingEnabled(true, //Enabled
                               grip.gs0.pin()); //DataSource
  //put this line in setup.
  grip.gs0.writeRateControl(SerialWombatAbstractScaledOutput::Period::PERIOD_16mS, // Sampling Period
                            (uint16_t)SERVO_MOVEMENT_INCREASE_RATE, //Maximum Increase Rate
                            (uint16_t)SERVO_MOVEMENT_DECREASE_RATE); //Maximum Decrease Rate

}

void loop() {
  grip.gs0.writePublicData(0);
  delay(5000);
  grip.gs0.writePublicData(65535);
  delay(5000);
}
