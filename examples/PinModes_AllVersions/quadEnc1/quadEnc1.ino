#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatQuadEnc qeBasic(sw);
SerialWombatQuadEnc qeWithPullUps(sw);

// This example is explained in a video tutorial at: https://youtu.be/_wO8cOada3w



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial


  qeBasic.begin(0, 1);  // Initialize a QE on pins 0 and 1
  qeWithPullUps.begin(2, 3);  // Initialize a QE on pins 2 and 3  Change these to different pins (say 18 and 19) on the SW18AB, since 3 is SDA
}

void loop() {
  Serial.print(qeBasic.read());
  Serial.print(" ");
  Serial.print(qeWithPullUps.read());
  Serial.println();
  delay(50);
}
