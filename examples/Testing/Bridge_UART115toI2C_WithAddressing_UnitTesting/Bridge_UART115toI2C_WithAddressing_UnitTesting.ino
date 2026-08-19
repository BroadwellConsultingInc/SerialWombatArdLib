#include <SerialWombat.h>
#include <SoftWire.h>
/*
  This sketch is designed to allow UART communication with an I2C based Serial Wombat chip.
  It is a superset of the the normal version that includes a failure toggle pin, and support for MCP4728 chips

  It discards any 0x55, ' ' or 'x' initial bytes, then waits until 8 bytes are received,
  sends that as an I2C packet, and then sends the response back to the UART.
*/

SerialWombatChip SWC;
uint8_t i2cAddress = 0;
uint8_t tx[9], rx[8], count;
SoftWire softWire(2,3);

#define MCP4728_ADDR 0x60
#define SW_FAILURE_PIN 8  // If this #define is enabled, this pin will toggle when a 0x40 unit test failure packet is sent
                          // This is designed for internal unit testing with a specialized PCB board.

void setAnalogRatio(uint8_t pin, uint16_t ratio)
{
  switch (pin)
  {
    case 0:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x40);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;

    case 7:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x42);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;

    case 9:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x44);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;

    case 6:
      softWire.beginTransmission(MCP4728_ADDR);
      softWire.write(0x46);
      softWire.write((uint8_t)(ratio >> 12));
      softWire.write((uint8_t)(ratio >> 4));
      softWire.endTransmission();
      break;


    case 16:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x40);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_A, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;

    case 17:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x42);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_B, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;

    case 18:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x44);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_C, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;

    case 19:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x46);
      Wire.write((uint8_t)(ratio >> 12));
      Wire.write((uint8_t)(ratio >> 4));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_D, ratio >> 4, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_NORMAL);
      break;
  }

}

void analogShutdown()
{
  /*
    volt16171819.begin();
    volt16171819.setChannelValue(MCP4728_CHANNEL_A, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
    volt16171819.setChannelValue(MCP4728_CHANNEL_B, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
    volt16171819.setChannelValue(MCP4728_CHANNEL_C, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
    volt16171819.setChannelValue(MCP4728_CHANNEL_D, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_500K);
  */
  for (uint8_t x = 0x40; x <= 0x46; x += 2)
  {
    softWire.beginTransmission(MCP4728_ADDR);
    softWire.write(x);
    softWire.write(0x60);
    softWire.write(0x00);
    softWire.endTransmission();


    Wire.beginTransmission(MCP4728_ADDR);
    Wire.write(x);
    Wire.write(0x60);
    Wire.write(0x00);
    Wire.endTransmission();
  }

}

void analog1k(uint8_t pin)
{
  switch (pin)
  {
    case 16:

      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x40);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_A, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;

    case 17:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x42);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_B, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;

    case 18:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x44);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_C, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;

    case 19:
      Wire.beginTransmission(MCP4728_ADDR);
      Wire.write(0x46);
      Wire.write((uint8_t)(0x20));  //TODO Double check this...
      Wire.write((uint8_t)(0));
      Wire.endTransmission();
      //volt16171819.setChannelValue(MCP4728_CHANNEL_D, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K);
      break;
  }

}


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

#ifdef SW_FAILURE_PIN  // This is used when this bridge is part of Jon's Unit test fixture
#warning FAILURE PIN IS ENABLED
  pinMode(SW_FAILURE_PIN,OUTPUT);
#endif



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
        if (tx[1] == 251)
        {
          analog1k(tx[2]);
        }
        else if (tx[1] == 252)
        {
          analogShutdown();
        }
        else if (tx[1] == 253)
        {

          setAnalogRatio(tx[2],((uint16_t)tx[4]) * 256 + tx[3]);
        }
        
        else if (tx[1] != 0x55 && tx[1] != 'x' && tx[1] != ' ')
        {
          #ifdef SW_FAILURE_PIN
          if (tx[1] == 0x40) 
          {
            digitalWrite(SW_FAILURE_PIN,!digitalRead(SW_FAILURE_PIN));            
          }
          #endif
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
