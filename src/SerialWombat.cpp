#include "SerialWombat.h"
/*
Copyright 2020-2021 Broadwell Consulting Inc.

Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
*/

/*! \file SerialWombat.cpp
*/


SerialWombatChip::SerialWombatChip()
{
}



SerialWombatChip::~SerialWombatChip()
{
}


int16_t SerialWombatChip::begin(HardwareSerial& serial)
{
	return begin(serial, true);
}

int16_t SerialWombatChip::begin(HardwareSerial& serial, bool reset)
{
	Serial = &serial;
	Serial->begin(115200);
	Serial->setTimeout(1000);
	if (reset)
	{
		hardwareReset();
		sendReadyTime = millis() + 1000;
		return(1);
	}
	else
	{
		return initialize();
	}
	
	
}

int16_t SerialWombatChip::begin(TwoWire& wire, uint8_t i2cAddress)
{
	return begin(wire, i2cAddress, true);

}
int16_t SerialWombatChip::begin(TwoWire& wire, uint8_t i2cAddress, bool reset)
{
	i2cInterface = &wire;
	address = i2cAddress;

	Wire.beginTransmission(i2cAddress);
	int error = Wire.endTransmission();


	if (error != 0)
	{
		return(-1);
	}

	if (reset)
	{
		hardwareReset();
		sendReadyTime = millis() + 1000;
		return(1);
	}
	else
	{
		sendReadyTime = 0;
		return initialize();
		
	}
}

int16_t SerialWombatChip::begin(uint8_t i2cAddress)
{
	//i2cInterface = &Wire;
	//address = i2cAddress;
	return begin(Wire, i2cAddress);
}

int16_t SerialWombatChip::initialize()
{
	int16_t retvalue = -1;
	lastErrorCode = 0;
	readVersion();
	
	readSupplyVoltage_mV();

	readUniqueIdentifier();
	
	readDeviceIdentifier();
	
	return(lastErrorCode);

}

void SerialWombatChip::readUniqueIdentifier()
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
	else if (isSW18())
	{
		for (uint32_t address = 0x801600; address <= 0x801608; address += 2)
		{
			uint32_t data = readFlashAddress(address);
			uniqueIdentifier[uniqueIdentifierLength] = data;
			++uniqueIdentifierLength;
			uniqueIdentifier[uniqueIdentifierLength] = data >> 8;
			++uniqueIdentifierLength;
			uniqueIdentifier[uniqueIdentifierLength] = data >> 16;
			++uniqueIdentifierLength;
		}
	}
}

void SerialWombatChip::readDeviceIdentifier()
{
	if (version[0] == 'S' && version[1] == '0' && version[2] == '4')
	{ //16F15214
		
			uint32_t data = readFlashAddress(0x8006);
			deviceIdentifier = (uint16_t)data;
			data = readFlashAddress(0x8005);
			deviceRevision = (uint16_t)data;
	}
	else if (isSW18())
	{
		uint32_t data = readFlashAddress(0xFF0000);
		deviceIdentifier = (uint16_t)data;
		data = readFlashAddress(0xFF0002);
		deviceRevision = (uint16_t)data & 0xF;
	}
}

int SerialWombatChip::sendPacket(uint8_t tx[], uint8_t rx[])
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
		while (Serial->read() >= 0);
		Serial->write(tx, 8);  //TODO add addressing, CRC
		int bytesRx = Serial->readBytes(rx, 8);
		if (bytesRx < 8)
		{
			return (bytesRx + 1);
		}
		if (rx[0] == 'E')
		{
			lastErrorCode = -1 * ((int)rx[1] * 256 + rx[2]);
			++errorCount;
			return (lastErrorCode);
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
			lastErrorCode = -1 * ((int)rx[1] * 256 + rx[2]);
			++errorCount;
			return (lastErrorCode);
		}
	}
	return(0);

}
int SerialWombatChip::sendPacketNoResponse(uint8_t tx[])
{

	if (Serial != NULL)
	{
		Serial->write(tx, 8);  //TODO add addressing, CRC
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
	}
	return(0);

}

int SerialWombatChip::sendPacket(uint8_t tx[])
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

char* SerialWombatChip::readVersion()
{
	uint8_t tx[] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	memcpy(version, &rx[1], 7);
	version[7] = '\0';
	memcpy(model, &rx[1], 3);
	model[3] = '\0';
	fwVersion[0] = rx[5];
	fwVersion[1] = rx[6];
	fwVersion[2] = rx[7];


	return (version);
}

uint16_t SerialWombatChip::readSupplyVoltage_mV()
{
	if (isSW18())
	{
		_supplyVoltagemV = readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_VCC_mVOLTS);
	}
	else
	{
		uint16_t counts = readPublicData(66); // Get FVR counts (1.024 v)
		if (counts > 0)
		{
			uint32_t mv = 1024 * 65536 / counts;
			_supplyVoltagemV = (uint16_t)mv;
		}
		else
		{
			_supplyVoltagemV = 0;
		}
	}
	return(_supplyVoltagemV);
}

int16_t SerialWombatChip::readTemperature_100thsDegC(void)
{
	if (isSW18())
	{
		int32_t result = readPublicData(70);
		if (result >= 32768)
		{
			result = result - 65536;
		}
		return ((int16_t)result);
	}
	else
	{
		return 2500;
	}
}

void SerialWombatChip::hardwareReset()
{
	uint8_t tx[9] = "ReSeT!#*";
	uint8_t rx[8];
	sendPacket(tx, rx);
}

void SerialWombatChip::pinMode(uint8_t pin, uint8_t mode)
{
	pinMode(pin, mode, false, false);
}

void SerialWombatChip::pinMode(uint8_t pin, uint8_t mode, bool pullDown, bool openDrain)
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

void SerialWombatChip::digitalWrite(uint8_t pin, uint8_t val)
{
	configureDigitalPin(pin, val);
}


int SerialWombatChip::digitalRead(uint8_t pin)
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

int SerialWombatChip::analogRead(uint8_t pin)
{
	uint8_t tx[] = { 200,pin,PIN_MODE_ANALOGINPUT,0,0,0,0,0 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return (readPublicData(pin) >> 6); // Scale from 16 bit value to 10 bit value.
}

void SerialWombatChip::analogWrite(uint8_t pin, int val)
{
	uint8_t dutyCycleLow = 0;
	if (val == 255)
	{
		dutyCycleLow = 255;
	}
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,pin,PIN_MODE_PWM,pin,dutyCycleLow,(uint8_t) val,false,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
}

bool SerialWombatChip::queryVersion()
{
	uint8_t tx[8] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	if (rx[0] == 'V' && (rx[1] == 'S' || rx[1] == 'B'))
	{
		model[0] = rx[1];
		model[1] = rx[2];
		model[2] = rx[3];
		model[3] = 0;
		fwVersion[0] = rx[5];
		fwVersion[1] = rx[6];
		fwVersion[2] = rx[7];
		fwVersion[3] = 0;

		inBoot = (rx[1] == 'B');
		return (true);
	}
	return (false);
}

uint32_t SerialWombatChip::readFramesExecuted()
{
	uint8_t tx[8] = { 0x81,67,68,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	uint32_t returnval = rx[2] + (((uint32_t)rx[3]) << 8) + (((uint32_t)rx[4]) << 16) + (((uint32_t)rx[5]) << 24);
		return (returnval);
}

uint16_t SerialWombatChip::readOverflowFrames()
{
	return readPublicData(69);
}

void SerialWombatChip::jumpToBoot()
{
	uint8_t tx[] = "BoOtLoAd";
	sendPacket(tx);
}

uint8_t SerialWombatChip::readRamAddress(uint16_t address)
{
	uint8_t tx[8] = { 0xA0,SW_LE16(address),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return(rx[3]);
}

int16_t SerialWombatChip::writeRamAddress(uint16_t address, uint8_t value)
{
	uint8_t tx[8] = { 0xA3,SW_LE16(address),0,0,value,0x55,0x55};
	return sendPacket(tx);
}

uint32_t SerialWombatChip::readFlashAddress(uint32_t address)
{
	uint8_t tx[8] = { 0xA1,SW_LE32(address),0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return(rx[4] + (rx[5] <<8) + (rx[6] <<16) + (rx[7] <<24));
}

void SerialWombatChip::sleep()
{
	uint8_t tx[8] = { 'S','l','E','e','P','!','#','*'};
	sendPacket(tx);
	_asleep = true;
}

void SerialWombatChip::wake()
{
	uint8_t tx[8] = { '!','!','!','!','!','!','!','!' };
	sendPacket(tx);
}

bool SerialWombatChip::isSW18()
{
	return ( model[1] == '1' && model[2] == '8');
}

int16_t SerialWombatChip::eraseFlashPage(uint32_t address)
{
	uint8_t tx[8] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_WRITE_FLASH,
		0, //Erase Page
		SW_LE32(address),
		0x55,0x55 };
	return sendPacket(tx);
}

int16_t SerialWombatChip::writeFlashRow(uint32_t address)
{
	uint8_t tx[8] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_WRITE_FLASH,
		1, // Write entire row
		SW_LE32(address),0x55,0x55 };
	return sendPacket(tx);
}

int16_t SerialWombatChip::setThroughputPin(uint8_t pin)
{
	uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,pin,PIN_MODE_FRAME_TIMER,0x55,0x55,0x55,0x55,0x55 };
	return sendPacket(tx);
}

uint8_t SerialWombatChip::find()
{
	return(find(false));
}

uint8_t SerialWombatChip::find(bool keepTrying)
{
	do
	{
		for (int i2cAddress = 0x68; i2cAddress <= 0x6F; ++i2cAddress)
		{
			Wire.beginTransmission(i2cAddress);
			int error = Wire.endTransmission();


			if (error == 0)
			{
				uint8_t tx[8] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
				uint8_t rx[8];
				Wire.beginTransmission(i2cAddress);
				Wire.write(tx, 8);
				Wire.endTransmission();
				Wire.requestFrom((uint8_t)i2cAddress, (uint8_t)8);

				int count = 0;
				while (Wire.available() && count < 8)
				{
					rx[count] = Wire.read();
					++count;
				}
				if (count == 8)
				{
					if (rx[0] = 'V' && rx[1] == 'S')
					{
						return(i2cAddress); // Found one.
					}
				}
			}
		}
		delay(0);
	}while (keepTrying);
			return(0);  // Didn't find one.
}

void SerialWombatChip::configureDigitalPin(uint8_t pin,uint8_t highLow)
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

uint16_t SerialWombatChip::readPublicData(SerialWombatDataSource dataSource)
{
	return (readPublicData((uint8_t)dataSource));
}

uint16_t SerialWombatChip::readPublicData(uint8_t pin)
{
	uint8_t tx[] = { 0x81,pin,255,255,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return(rx[2] + (uint16_t)rx[3] * 256);
}

uint16_t SerialWombatChip::writePublicData(uint8_t pin, uint16_t value)
{
	uint8_t tx[] = { 0x82,pin,(uint8_t)(value & 0xFF),(uint8_t)(value >> 8) ,255,0x55,0x55,0x55 };
	uint8_t rx[8];
	sendPacket(tx, rx);
	return (rx[2] + rx[3] * 256);
}

int SerialWombatChip::writeUserBuffer(uint16_t address, uint8_t* buffer, uint16_t count)
{
	uint16_t bytesSent = 0;
	if (count == 0)
	{
		return 0;
	}

	{ // Send first packet of up to 4 bytes
		uint8_t bytesToSend = 4;
		if (count < 4)
		{
			bytesToSend = (uint8_t)count;
			count = 0;
		}
		else
		{
			count -= 4;
		}

		uint8_t tx[8] = { 0x84,SW_LE16(address), bytesToSend,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];

		uint8_t i;
		for (i = 0; i < bytesToSend; ++i)
		{
			tx[4 + i] = buffer[i];
		}
		int result = sendPacket(tx, rx);
		if (rx[0] == 'E')
		{
			return (result);
		}
		bytesSent = bytesToSend;
	}
	while (count >= 7)  // Continue sending
	{
		count -= 7;
		uint8_t tx[8] = { 0x85,0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		uint8_t i;
		for (i = 0; i < 7; ++i)
		{
			tx[1 + i] = buffer[bytesSent + i];
		}
		int result = sendPacket(tx, rx);
		if (rx[0] == 'E')
		{
			return (result);
		}
		bytesSent += 7;
	}
	if (count > 0)
	{
		{ // Send first packet of up to 4 bytes
			uint8_t bytesToSend = 4;
			if (count < 4)
			{
				bytesToSend = (uint8_t)count;
				count = 0;
			}
			else
			{
				count -= 4;
			}

			uint8_t tx[8] = { 0x84,SW_LE16(address + bytesSent), bytesToSend,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];

			uint8_t i;
			for (i = 0; i < bytesToSend; ++i)
			{
				tx[4 + i] = buffer[i + bytesSent];
			}
			int result = sendPacket(tx, rx);
			if (rx[0] == 'E')
			{
				return (result);
			}
			bytesSent += bytesToSend;
		}
		return (bytesSent);
	}
	return(0);
	

}

int16_t SerialWombatChip::enable2ndCommandInterface(bool enabled)
{
	uint8_t tx[] = { 0xA6,0,0xB2, 0xA5, 0x61, 0x73, 0xF8 ,0xA2 };
	if (enabled)
	{
		tx[1] = 1;
	}
	return sendPacket(tx);
}

SerialWombatPin::SerialWombatPin(SerialWombatChip& serialWombatChip) : _sw(serialWombatChip)
{
	_sw = serialWombatChip;
}

void SerialWombatPin::pinMode(uint8_t mode)
{
	_sw.pinMode(_pin, mode);
}

void SerialWombatPin::pinMode(uint8_t mode, bool pullDown, bool openDrain)
{
	_sw.pinMode(_pin, mode, pullDown, openDrain);
}

void SerialWombatPin::digitalWrite( uint8_t val)
{
	_sw.digitalWrite(_pin, val);
}


int SerialWombatPin::digitalRead()
{
	return (_sw.digitalRead(_pin));
}
