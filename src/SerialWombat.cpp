#include "SerialWombat.h"

/*! \file SerialWombat.cpp
*/


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
		sendReadyTime = millis() + 1000;
	}
	else
	{
		initialize();
	}
	
	
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
		sendReadyTime = millis() + 1000;
	}
	else
	{
		sendReadyTime = 0;
		initialize();
		
	}
}

void SerialWombat::begin(uint8_t i2cAddress)
{
	//i2cInterface = &Wire;
	//address = i2cAddress;
	begin(Wire, i2cAddress);
}

void SerialWombat::initialize()
{
	readVersion();
	readSupplyVoltage_mV();
	readUniqueIdentifier();
	readDeviceIdentifier();
}

void SerialWombat::readUniqueIdentifier()
{
	uniqueIdentifierLength = 0;
	if (version[0] == 'S' && version[1] == '0' && version[2] == '4')
	{ //16F15214
		for (uint32_t address = 0x8100; address <= 0x8108; ++address)
		{
			uint32_t data = readFlashAddress(address);
			uniqueIdentifier[uniqueIdentifierLength] = (uint8_t)data;
			++uniqueIdentifierLength;
			/* Always zero... leave out
			uniqueIdentifier[uniqueIdentifierLength] = (uint8_t)(data>>8);
			++uniqueIdentifierLength;
			*/
		}
	}
}

void SerialWombat::readDeviceIdentifier()
{
	if (version[0] == 'S' && version[1] == '0' && version[2] == '4')
	{ //16F15214
		
			uint32_t data = readFlashAddress(0x8006);
			deviceIdentifier = (uint16_t)data;
			data = readFlashAddress(0x8005);
			deviceRevision = (uint16_t)data;
	}
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
		initialize();
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
		delayMicroseconds(50);
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
	uint8_t rx[8];

	if (_asleep)
	{
		_asleep = false;
		uint8_t txw[8] = { '!','!','!','!','!','!','!','!' };  //TODO adapt for UART
		sendPacket(txw);
		delayMicroseconds(200);
	}

	return(sendPacket(tx,rx));

	if (sendReadyTime != 0)
	{
		unsigned long currentTime = millis();
		if (currentTime < sendReadyTime)
		{
			delay(sendReadyTime - currentTime);
			
		}
		sendReadyTime = 0;
		initialize();
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
	_supplyVoltagemV = (uint16_t) mv;
	return(_supplyVoltagemV);
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
	configureDigitalPin(pin, mode);
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
	uint8_t tx[] = { CMD_SET_PIN_MODE0,pin,PIN_MODE_PWM,pin,dutyCycleLow,(uint8_t) val,false,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
}

bool SerialWombat::queryVersion()
{
	uint8_t tx[8] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	if (rx[0] == 'V' && rx[1] == 'S')
	{
		model[0] = rx[2];
		model[1] = rx[3];
		model[2] = rx[4];
		model[3] = 0;
		fwVersion[0] = rx[5];
		fwVersion[1] = rx[6];
		fwVersion[2] = rx[7];
		fwVersion[3] = 0;
		return (true);
	}
	return (false);
}

uint32_t SerialWombat::readFramesExecuted()
{
	uint8_t tx[8] = { 0x81,67,68,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	uint32_t returnval = rx[2] + (((uint32_t)rx[3]) << 8) + (((uint32_t)rx[4]) << 16) + (((uint32_t)rx[5]) << 24);
		return (returnval);
}

uint16_t SerialWombat::readOverflowFrames()
{
	return readPublicData(69);
}

void SerialWombat::jumpToBoot()
{
	uint8_t tx[] = "BoOtLoAd";
	sendPacket(tx);
}

uint8_t SerialWombat::readRamAddress(uint16_t address)
{
	uint8_t tx[8] = { 0xA0,SW_LE16(address),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return(rx[3]);
}

void SerialWombat::writeRamAddress(uint16_t address, uint8_t value)
{
	uint8_t tx[8] = { 0xA3,SW_LE16(address),0,0,value,0x55,0x55};
	sendPacket(tx);
}

uint32_t SerialWombat::readFlashAddress(uint32_t address)
{
	uint8_t tx[8] = { 0xA1,SW_LE16(address),0,0,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return(rx[4] + (rx[5] <<8));
}

void SerialWombat::sleep()
{
	uint8_t tx[8] = { 'S','l','E','e','P','!','#','*'};
	sendPacket(tx);
	_asleep = true;
}

void SerialWombat::wake()
{
	uint8_t tx[8] = { '!','!','!','!','!','!','!','!' };
	sendPacket(tx);
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
