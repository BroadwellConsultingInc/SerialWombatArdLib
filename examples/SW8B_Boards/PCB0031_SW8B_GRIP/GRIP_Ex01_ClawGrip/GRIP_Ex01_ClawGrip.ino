/*
This video shows how to use the PCB0031 Grip's current feedback capabilites to
automatically calibrate a servo based gripper claw.   

A good sample platform is an SG90 Servo and this 3d printed Gripper https://www.thingiverse.com/thing:5664874

In this example the library and Serial Wombat firmware automatically cycle
through the range of the servo taking current readings at various positions.

By looking for a current spike caused by motor stall the software can determine
the range of motion of the gripper and its open and closed points

The sketch then alternates between gripping and releasing.  Feedback control of
commanded servo position vs. current is used to limit how hard the gripper
squeezes.  

By checking if the servo stopped short of full closed position inference of
an object in the gripper can be made.

Every servo / gripper combination is a bit different.  There are parameters
in the example you can adjust.  These are explained in the comments.

This example assumes the servo is attached to pin 0.  For pins 1 2 or 3 change
gs0 to gs1, gs2, or gs3 .

Be sure to use pull up resistors on the I2C bus or enable the on board pull ups
on the Grip board via the solder jumper.

The Grip board must be provided with 5V electronics input for proper operation of the current sensors.

Note that this example attempts to push the servo through its full 180 degree
range in order to take current measurements.  Do not use this example if that
could cause damage to your gripper mechanics.

A video demonstrating this example is available at 
https://youtu.be/TODO

Documentation for the Grip board classes are available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_p_c_b0031___grip.html

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

	Serial.println("Calibrating the gripper.  This can take 20 seconds to measure current at 65 different positions.  The gripper may appear to stop moving for a bit.");
 grip.gs0.calibrateGripper(); // Add the parameter true to reverse servo direction if your servo opens the gripper at higher values
Serial.println("Calibration Complete.");

}

void loop() {
  
  delay(5000);

  grip.gs0.grip(32768);  //Try to grip an object.  Increase or decrease the paramter to grip harder or softer.  Max 65535

  // Note that the grip function returns immediatley and does not block or delay.  The gripping algorithm is executed on the 
  // Serial Wombat chip, not on the Arduino.
  
  delay(1500); // Allow the gripper time to settle before object detection
  
  if (grip.gs0.objectPresent(60))  // Increase the parameter to detect smaller objects.  Reduce the parmeter to reduce false positives
  {
    Serial.println("Object Grasped");
  }
  else
  {
    Serial.println("No Object");
  }
  delay(3500);
  
  grip.gs0.release();
}
