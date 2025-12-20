#include <SerialWombat.h>

/*
  This sketch is designed to allow UART communication with an I2C based Serial Wombat chip.

  It discards any 0x55, ' ' or 'x' initial bytes, then waits until 8 bytes are received,
  sends that as an I2C packet, and then sends the response back to the UART.
*/

SerialWombatChip SWC;
uint8_t i2cAddress = 0;
uint8_t tx[9], rx[8], count;

void setup() {

#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif
  Wire.setTimeout(1000);
  Serial.begin(115200);

  delay(100);
  Serial.flush();
  count = 0;
  i2cAddress = SWC.find(true);
}

uint32_t lastReceive = 0;
#define RECEIVETIMEOUT 2000
void loop() {


  int x =  Serial.read();

  while (x >= 0)
  {
    lastReceive = millis();
    if (count > 0)
    {
      tx[count] = x;
      ++count;
      if (count >= 9)
      {
        if (tx[1] != 0x55 && tx[1] != 'x' && tx[1] != ' ')
        {
          if (tx[0] != 0xFF)
          {
            Wire.beginTransmission(tx[0]);
          }
          else
          {
            Wire.beginTransmission(i2cAddress);
          }
          Wire.write(&tx[1], 8);
          Wire.endTransmission();
          delayMicroseconds(100);
          if (tx[0] != 0xFF)
          {
            Wire.requestFrom(tx[0], (uint8_t)8);
          }
          else
          {
            Wire.requestFrom(i2cAddress, (uint8_t)8);
          }
          count = 0;
          int r = 0;
          while (r >= 0 && count < 8)
          {
            r = Wire.read();

            if (r >= 0)
            {
              rx[count] = (uint8_t) r;
              ++count;
            }
            else
            {
              break;
            }
          }

          Serial.write(rx, 8);
        }
        count = 0;
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
  if (millis() > lastReceive + RECEIVETIMEOUT)
  {
    count = 0;
  }
}
