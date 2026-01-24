#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatQuadEnc qeBasic(sw);
SerialWombatQuadEnc qeWithPullUps(sw);

// This example is explained in a video tutorial at: https://youtu.be/_wO8cOada3w



void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("1Hz Blink Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
 
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
  if(sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_QUADRATUREENCODER))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while(1){delay(100);}
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
                      //Optional Error handling code end 


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
