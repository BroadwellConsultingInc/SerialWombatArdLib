#include <SerialWombat.h>

SerialWombat sw6C;    //Declare a Serial Wombat
SerialWombatQuadEnc qeBasic(sw6C);
SerialWombatQuadEnc qeWithPullUps(sw6C);

// This example is explained in a video tutorial at: https://youtu.be/_wO8cOada3w



void setup() {
  // put your setup code here, to run once:

  { //I2C Initialization
    Wire.begin();
    sw6C.begin(Wire, 0x6C); //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C
  }
  qeBasic.begin(0, 1);
  qeWithPullUps.begin(2, 3);
  Serial.begin(115200);
}

void loop() {
  Serial.print(qeBasic.read());
  Serial.print(" ");
  Serial.print(qeWithPullUps.read());
  Serial.println();
  delay(50);
}
