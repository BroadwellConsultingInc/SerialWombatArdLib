#include <SerialWombat.h>
SerialWombatChip sw;
SerialWombatUltrasonicDistanceSensor distanceSensor(sw);
SerialWombatHBridge_18AB Pin0HBridge(sw); // Your serial wombat chip may be named something else than sw

//Put this line before setup()
SerialWombatMatrixKeypad Pin12Keypad(sw); // Your serial wombat chip may be named something else than sw

#define TOP_FLOOR 110
#define MID_FLOOR 80
#define BOTTOM_FLOOR 50

#define MOTOR_OFF 32768
#define GO_UP 65535
#define GO_DOWN 0
void setup() {
  Wire.begin();
  Serial.begin(115200);
  sw.begin(Wire, 0x6B);


  distanceSensor.begin (10,  SerialWombatUltrasonicDistanceSensor::driver::HC_SR04, 11);



  //Add this line to  setup():
  Pin12Keypad.begin(12, //Control pin
                    12, //Row 0 pin
                    13, // Row 1 pin
                    14, // Row 2 pin
                    15, // Row 3 pin
                    16, // Col 0 pin
                    17, // Col 1 pin
                    18, // Col 2 pin
                    19, // Col 3 pin
                    0,  //Public data mode
                    1); //Queue Mode


  //Put this line before setup()

  //Add this to  setup():
  Pin0HBridge.begin(0, //Pin Number
                    1, // Second Pin
                    1000,// PWM Period in uS
                    DRV8833);   // Driver

  //put this line in setup.
  Pin0HBridge.writeHysteresis(87, //Low Limit
                              MOTOR_OFF, //Low Value
                              88, //High Limit
                              MOTOR_OFF, //High Value
                              0); // Initial output
  //put this line in setup.  Make this the last line after other
  // Output Scaling configurations for this pin
  Pin0HBridge.writeScalingEnabled(true, //Enabled
                                  10); //DataSource

  delay(500);

  if (distanceSensor.readPublicData() > BOTTOM_FLOOR)
  {
    Pin0HBridge.writeHysteresis(BOTTOM_FLOOR  + 2, //Low Limit
                                MOTOR_OFF, //Low Value
                                BOTTOM_FLOOR + 3, //High Limit
                                0, //High Value
                                GO_DOWN); // Initial output
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  int16_t i = Pin12Keypad.read();
  if (i > 0 )
  {
    i-= '0';
    Serial.println(i);
  }

  Serial.println(distanceSensor.readPublicData());
  switch (i)
  {
    case 1:  // Go to bottom floor
      if (distanceSensor.readPublicData() > BOTTOM_FLOOR)
      {
        // Going down. 
        Pin0HBridge.writeHysteresis(BOTTOM_FLOOR  + 2, //Low Limit
                                    32768, //Low Value
                                    BOTTOM_FLOOR + 3, //High Limit
                                    0, //High Value
                                    GO_DOWN); // Initial output
      }
      else
      {
        //Going up
        Pin0HBridge.writeHysteresis(BOTTOM_FLOOR  - 3, //Low Limit
                                    GO_UP, //Low Value
                                    BOTTOM_FLOOR - 2, //High Limit
                                    MOTOR_OFF, //High Value
                                    0); // Initial output
      }
      break;

    case 2:  // Go to middle floor
      if (distanceSensor.readPublicData() > MID_FLOOR)
      {
        Pin0HBridge.writeHysteresis(MID_FLOOR  + 2, //Low Limit
                                    32768, //Low Value
                                    MID_FLOOR + 3, //High Limit
                                    0, //High Value
                                    GO_DOWN); // Initial output
      }
      else
      {
        Pin0HBridge.writeHysteresis(MID_FLOOR  - 3, //Low Limit
                                    GO_UP, //Low Value
                                    MID_FLOOR - 2, //High Limit
                                    MOTOR_OFF, //High Value
                                    0); // Initial output
      }
      break;

    case 3:  //Go to top floor
      if (distanceSensor.readPublicData() > TOP_FLOOR)
      {
        Pin0HBridge.writeHysteresis(TOP_FLOOR  + 2, //Low Limit
                                    32768, //Low Value
                                    TOP_FLOOR + 3, //High Limit
                                    0, //High Value
                                    GO_DOWN); // Initial output
      }
      else
      {
        Pin0HBridge.writeHysteresis(TOP_FLOOR  - 3, //Low Limit
                                    GO_UP, //Low Value
                                    TOP_FLOOR - 2, //High Limit
                                    MOTOR_OFF, //High Value
                                    0); // Initial output
      }
      break;



  }
}
