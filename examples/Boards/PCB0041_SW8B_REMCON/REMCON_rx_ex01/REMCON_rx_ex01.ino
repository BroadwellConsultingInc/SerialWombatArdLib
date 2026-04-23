/*  This example shows how to use the Serial Wombat PCB0041 Remcon
 *    It assumes an NEC compatible IR Transmitter (most inexpensive Arduino kit remotes and many commercial remotes are compatible)

     This example reads incoming commmands and prints their hex values to Serial. 

     Video on PCB0041 RemCon:

     TODO coming soon

     SerialWombatIRRx pin mode documentation:

     TODO
*/

#include <SerialWombat.h>

PCB0041_Remcon remcon;

#define PCB0041_I2C_ADDRESS 0x60

#ifndef PCB0041_I2C_ADDRESS
#error "Comment in and set the Address define on line 19.  The default address is 0x60"
#endif

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("PCB0041 IR Receive Example.  Point remote at PCB0041 and press a key");


  
  remcon.begin(PCB0041_I2C_ADDRESS);

   //Optional Error Checking Code starts here
  if (!remcon.isSW08())
  {
    Serial.println("This Example requires a PCB0041 Remcon board");
    while (1) {
      delay(100);
    }
  }
  if (!remcon.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if ( !(remcon.isPinModeSupported(PIN_MODE_IRRX) &&  remcon.isPinModeSupported(PIN_MODE_BLINK)) )
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  remcon.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  delay(1000);
}

void loop() {
  int receivedData = remcon.irrx.read();

  if (receivedData >= 0)
  {
    char s[50];
    sprintf(s,"0x%X : 0x%X",remcon.irrx.readAddress() , receivedData);
    Serial.println(s);
  }

}
