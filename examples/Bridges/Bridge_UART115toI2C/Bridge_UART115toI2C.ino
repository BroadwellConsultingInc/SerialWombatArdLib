#include <SerialWombat.h>

/*
  This sketch is designed to allow UART communication with an I2C based Serial Wombat chip.

  It discards any 0x55, ' ' or 'x' initial bytes, then waits until 8 bytes are received,
  sends that as an I2C packet, and then sends the response back to the UART.
*/

SerialWombatChip SWC;
uint8_t i2cAddress = 0;
uint8_t tx[8], rx[8], count;

void setup() {

#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(100);

  i2cAddress = SWC.find(true);
  SWC.begin(Wire, i2cAddress);
  Serial.flush();
  count = 0;
}

void loop() {


  int x =  Serial.read();

  while (x >= 0)
  {
    if (count > 0)
    {
      tx[count] = x;
      ++count;
      if (count >= 8)
      {
        SWC.sendPacket(tx, rx);
        count = 0;
        Serial.write(rx, 8);
      }
    }
    else
    {
      if (x != 0x55 && x != 'x' && x != ' ')
      {
        tx[count] = x;
        ++count;
      }
    }
    x = Serial.read();
  }
}
