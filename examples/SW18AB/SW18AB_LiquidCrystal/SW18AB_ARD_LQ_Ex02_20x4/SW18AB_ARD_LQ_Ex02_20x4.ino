#include "SerialWombat.h"
#include <stdio.h>

SerialWombatChip sw;

const char testString[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789One Two Three Four Five Six Seven Eight Nine Ten Uno Dos Tres Quatro Cinco Seis Siete Ocho Nueve 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11";// The Quick Brown Fox Jumped over the Lazy Dog.The Early bird gets the worm.Never eat soggy waffles.Righty Tighty Lefty Loosey";

SerialWombatLiquidCrystal lcd0(sw, 5, 4, 3, 2, 1, 0);
SerialWombatQueue q(sw);

void setup() {
  // put your setup code here, to run once:
  sw.begin(Serial,false);

  lcd0.begin(20, 4);
  
  lcd0.setRowOffsets(0, 0x40, 0x14, 0x54);
  lcd0.initializeBufferCopy(8, 20);
 

  q.begin(0, 80, SerialWombatQueueType::QUEUE_TYPE_RAM_BYTE_SHIFT);

}

void loop() {
	static int i = 0;
  // put your main code here, to run repeatedly:
	q.write(testString[i]);
	delay(250);
	++i;
	if (i > strlen(testString))
	{
		i = 0;
	}

}
