#include "serialWombat.h"


SerialWombat::SerialWombat()
{
}



SerialWombat::~SerialWombat()
{
}

void SerialWombat::begin(HardwareSerial& serial)
{
	begin(serial, true);
}

void SerialWombat::begin(HardwareSerial& serial, bool reset)
{
	Serial = &serial;
	Serial->begin(115200);
	Serial->setTimeout(1000);
	if (reset)
	{
		hardwareReset();
	}
	sendReadyTime = millis() + 1000;
}

void SerialWombat::begin(TwoWire& wire, uint8_t i2cAddress)
{
	begin(wire, i2cAddress, true);
}
void SerialWombat::begin(TwoWire& wire, uint8_t i2cAddress, bool reset)
{
	i2cInterface = &wire;
	address = i2cAddress;
	if (reset)
	{
		hardwareReset();
	}
	sendReadyTime = millis() + 1000;
}

void SerialWombat::begin(uint8_t i2cAddress)
{
	i2cInterface = &Wire;
	address = i2cAddress;
}


int SerialWombat::sendPacket(uint8_t tx[], uint8_t rx[])
{
	if (sendReadyTime != 0 )
	{
		unsigned long currentTime = millis();
		if (currentTime < sendReadyTime)
		{
			delay(sendReadyTime - currentTime);
		}
		sendReadyTime = 0;
	}

	if (Serial != NULL)
	{
		Serial->write(tx, 8);  //TODO add addressing, CRC
		int bytesRx = Serial->readBytes(rx, 8);
		if (bytesRx < 8)
		{
			return (bytesRx + 1);
		}
		if (rx[0] == 'E')
		{
			return (-1 * ((int)rx[1] * 256 + rx[2]));
		}
		return (8);
	}

	if (i2cInterface != NULL)
	{
		int count = 8;
		//while (tx[count - 1] == 0x55)
		//{
		//	--count;
		//}
		i2cInterface->beginTransmission(address);
		i2cInterface->write(tx, 8);
		i2cInterface->endTransmission();
		//delay(3);
		i2cInterface->requestFrom(address, (uint8_t)8);

		count = 0;
		while (i2cInterface->available() && count < 8)
		{
			rx[count] = i2cInterface->read();
			++count;
		}
		if (rx[0] == 'E')
		{
			return (-1 * ((int)rx[1] * 256 + rx[2]));
		}
	}
	return(0);

}

int SerialWombat::sendPacket(uint8_t tx[])
{
	if (sendReadyTime != 0)
	{
		unsigned long currentTime = millis();
		if (currentTime < sendReadyTime)
		{
			delay(sendReadyTime - currentTime);
		}
		sendReadyTime = 0;
	}

	if (Serial != NULL)
	{
		Serial->write(tx, 8);  //TODO add addressing, CRC		
		return (8);
	}

	if (i2cInterface != NULL)
	{
		int count = 8;
	//	while (tx[count - 1] == 0x55)
		//{
		//	--count;
		//}
		i2cInterface->beginTransmission(address);
		i2cInterface->write(tx, 8);
		i2cInterface->endTransmission();
	}
	return(0);

}

char* SerialWombat::readVersion()
{
	uint8_t tx[] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	memcpy(version, &rx[1], 7);
	version[7] = '\0';
	return (version);
}

uint16_t SerialWombat::readSupplyVoltage_mV()
{
	uint16_t counts = readPublicData(66); // Get FVR counts (1.024 v)
	uint32_t mv = 1024 * 65536 / counts;

	return((uint16_t)mv);
}

void SerialWombat::hardwareReset()
{
	uint8_t tx[9] = "ReSeT!#*";
	uint8_t rx[8];
	sendPacket(tx, rx);
}

void SerialWombat::pinMode(uint8_t pin, uint8_t mode)
{
	pinMode(pin, mode, false, false);
}

void SerialWombat::pinMode(uint8_t pin, uint8_t mode, bool pullDown, bool openDrain)
{
	
	

	if (pin >= WOMBAT_MAXIMUM_PINS)
	{
		return;
	}
	_pullDown[pin] = openDrain;
	_openDrain[pin] = openDrain;
	_pinmode[pin] = mode;
	configureDigitalPin(pin, LOW);
}

void SerialWombat::digitalWrite(uint8_t pin, uint8_t val)
{
	configureDigitalPin(pin, val);
}


int SerialWombat::digitalRead(uint8_t pin)
{
	if (readPublicData(pin) > 0)
	{
		return (HIGH);
	}
	else
	{
		return (LOW);
	}
}

int SerialWombat::analogRead(uint8_t pin)
{
	uint8_t tx[] = { 200,pin,PIN_MODE_ANALOGINPUT,0,0,0,0,0 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return (readPublicData(pin) >> 6); // Scale from 16 bit value to 10 bit value.
}

void SerialWombat::analogWrite(uint8_t pin, int val)
{
	uint8_t dutyCycleLow = 0;
	if (val == 255)
	{
		dutyCycleLow = 255;
	}
	uint8_t tx[] = { CMD_SET_PIN_MODE0,pin,PIN_MODE_PWM,pin,dutyCycleLow,val,false,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
}

void SerialWombat::configureDigitalPin(uint8_t pin,uint8_t highLow)
{
	uint8_t tx[8] = { 200,pin,0,0,0,0,0,0x55 };
	uint8_t rx[8];
	switch (_pinmode[pin])
	{
	case INPUT: // Arduino input
	{
		tx[3] = 2; //Input
	}
	break;
	case OUTPUT:
	{
		if (highLow == LOW)
		{
			tx[3] = 0; //Output
		}
		else if (highLow == HIGH)
		{
			tx[3] = 1;
		}
		else 
		{ 
			return; 
		}
	}
	break;
	case INPUT_PULLUP:
	{
		tx[3] = 2; //Input
		tx[4] = 1; //Pullup on
	}
	break;
	default:
	{
		return;
	}
	}
	tx[6] = _openDrain[pin];
	tx[5] = _pullDown[pin];
	sendPacket(tx, rx);
}


uint16_t SerialWombat::readPublicData(uint8_t pin)
{
	uint8_t tx[] = { 0x81,pin,255,255,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return(rx[2] + (uint16_t)rx[3] * 256);
}

uint16_t SerialWombat::writePublicData(uint8_t pin, uint16_t value)
{
	uint8_t tx[] = { 0x82,pin,(uint8_t)(value & 0xFF),(uint8_t)(value >> 8) ,255,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return (rx[2] + rx[3] * 256);
}