#include "SerialWombat.h"
#include <stdio.h>

SerialWombatChip sw;

const char testString[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789One Two Three Four Five Six Seven Eight Nine Ten Uno Dos Tres Quatro Cinco Seis Siete Ocho Nueve 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11";// The Quick Brown Fox Jumped over the Lazy Dog.The Early bird gets the worm.Never eat soggy waffles.Righty Tighty Lefty Loosey";

SerialWombatLiquidCrystal lcd0(sw, 4, 5, 0, 1, 2, 3);
SerialWombatQueue q(sw);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Liquid Crystal Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if(sw.isSW04())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
    while(1){delay(100);}
  }
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
  if(sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_LIQUIDCRYSTAL))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while(1){delay(100);}
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
                      //Optional Error handling code end 

  lcd0.begin(20,4);
  
  for (int i = 0; i < 0x40 + 40 ; i +=4)
  {
	  char s[5];
	  sprintf(s, "x%02X ", i);
	  lcd0.command(0x80 | i);
	  lcd0.write(s);
  }
}

void loop() {

}
