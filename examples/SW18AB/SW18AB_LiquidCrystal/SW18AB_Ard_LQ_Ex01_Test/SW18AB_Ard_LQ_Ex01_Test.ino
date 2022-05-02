#include "SerialWombat.h"
#include <stdio.h>

SerialWombatChip sw;

const char testString[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789One Two Three Four Five Six Seven Eight Nine Ten Uno Dos Tres Quatro Cinco Seis Siete Ocho Nueve 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11";// The Quick Brown Fox Jumped over the Lazy Dog.The Early bird gets the worm.Never eat soggy waffles.Righty Tighty Lefty Loosey";

SerialWombatLiquidCrystal lcd0(sw, 4, 5, 0, 1, 2, 3);
SerialWombatQueue q(sw);

void setup() {
  // put your setup code here, to run once:
  sw.begin(Serial,false);

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
