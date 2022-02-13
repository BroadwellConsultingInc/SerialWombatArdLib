#include "SerialWombat.h"
#include <stdint.h>
#include "SerialWombatMatrixKeypad.h"

SerialWombatMatrixKeypad::SerialWombatMatrixKeypad(SerialWombatChip& serialWombat):_sw(serialWombat)
{
	_sw = serialWombat;
}

int16_t SerialWombatMatrixKeypad::begin(uint8_t controlPin, uint8_t row0pin, uint8_t row1pin, uint8_t row2pin, uint8_t row3pin, uint8_t column0pin, uint8_t column1pin, uint8_t column2pin, uint8_t column3pin, uint8_t bufferMode, uint8_t queueMode)
{
	_pin = controlPin;

	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
						_pin,
						(uint8_t)PIN_MODE_MATRIX_KEYPAD ,
						row0pin,
						row1pin,
						row2pin,
						row3pin,
						column0pin };
	int16_t result = _sw.sendPacket(tx);
	if (result < 0)
	{
		return result;
	}

	uint8_t tx5[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE5,
						_pin,
						(uint8_t)PIN_MODE_MATRIX_KEYPAD ,
						column1pin,
						column2pin,
						column3pin,
						bufferMode,
						queueMode };
	return _sw.sendPacket(tx5);
}

int16_t SerialWombatMatrixKeypad::writeQueueMask(uint16_t mask)
{
	uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE7,
		_pin, (uint8_t)PIN_MODE_MATRIX_KEYPAD,
		SW_LE16(mask),0x55,0x55,0x55 };
	return _sw.sendPacket(tx);
}

int SerialWombatMatrixKeypad::available()
{
	uint8_t tx[8] = { 201, _pin, (uint8_t)PIN_MODE_MATRIX_KEYPAD, 0,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	return (rx[4]);
}

int SerialWombatMatrixKeypad::read()
{
	uint8_t tx[8] = { 202, _pin,(uint8_t)PIN_MODE_MATRIX_KEYPAD, 1,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	if (_sw.sendPacket(tx, rx) < 0)
	{
		return -1;
	}

	if (rx[3] != 0)
	{
		return (rx[4]);
	}
	else
	{
		return (-1);
	}
}

void SerialWombatMatrixKeypad::flush()
{
	
}

int SerialWombatMatrixKeypad::peek()
{
	uint8_t tx[8] = { 203, _pin,(uint8_t)PIN_MODE_MATRIX_KEYPAD,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	if (rx[4] > 0)
	{
		return (rx[5]);
	}
	else
	{
		return (-1);
	}
}

size_t SerialWombatMatrixKeypad::write(uint8_t data)
{
	
	return (1);
}

size_t SerialWombatMatrixKeypad::write(const uint8_t* buffer, size_t size)
{
	return(size);
}

int SerialWombatMatrixKeypad::availableForWrite()
{
	return(0);
}

void SerialWombatMatrixKeypad::setTimeout(long timeout_mS)
{
	if (timeout_mS == 0)
	{
		timeout = 0x80000000;
	}
	else
	{
		timeout = timeout_mS;
	}
}

SerialWombatMatrixKeypad SerialWombatMatrixKeypad::operator=(SerialWombatMatrixKeypad& kp)
{
	return kp;
}

size_t SerialWombatMatrixKeypad::readBytes(char* buffer, size_t length)
{
	int index = 0;
	int bytesAvailable = 0;
	uint32_t timeoutMillis = millis() + timeout;
	while (length > 0 && timeoutMillis > millis())
	{
		int bytecount = 4;
		if (length < 4)
		{
			bytecount = length;
		}
		{

			uint8_t tx[8] = { 202, _pin,(uint8_t)PIN_MODE_MATRIX_KEYPAD, (uint8_t)bytecount,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			_sw.sendPacket(tx, rx);
			bytesAvailable = rx[3];

			if (bytesAvailable == 0)
			{
				continue;
			}
			else
			{
				timeoutMillis = millis() + timeout;
			}
			uint8_t bytesReturned = bytecount;
			if (rx[3] < bytecount)
			{
				bytesReturned = rx[3];
			}
			for (int i = 0; i < bytesReturned; ++i)
			{
				buffer[index] = rx[i + 4];
				++index;
				--bytesAvailable;
				--length;

			}
		}

	}
	return (index);
}


SerialWombatMatrixButton::SerialWombatMatrixButton(SerialWombatMatrixKeypad& kp, uint8_t keyIndex):_keypad(kp),SerialWombatAbstractButton()
{
	_keypad = kp;
	_keyIndex = keyIndex;
}

bool SerialWombatMatrixButton::digitalRead()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
					_keypad._pin,
					PIN_MODE_MATRIX_KEYPAD,
					0,
					_keyIndex,0x55,0x55,0x55 };
	uint8_t rx[8];
	int result = _keypad._sw.sendPacket(tx, rx);
	if (result >= 0)
	{
		transitions = rx[4] + 256 * rx[5];

			return (rx[3] > 0);
	}
	return(0);
}

uint16_t SerialWombatMatrixButton::readDurationInFalseState_mS()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
					_keypad._pin,
					PIN_MODE_MATRIX_KEYPAD,
					0,
					_keyIndex,0x55,0x55,0x55 };
	uint8_t rx[8];
	int result = _keypad._sw.sendPacket(tx, rx);
	if (result >= 0)
	{
		transitions = rx[4] + 256 * rx[5];
		uint16_t time = rx[6] + 256 * rx[7];
		if (rx[3] == 0)
		{
			return(time);
		}	
	}
	return(0);
}

uint16_t SerialWombatMatrixButton::readDurationInTrueState_mS()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
					_keypad._pin,
					PIN_MODE_MATRIX_KEYPAD,
					0,
					_keyIndex,0x55,0x55,0x55 };
	uint8_t rx[8];
	int result = _keypad._sw.sendPacket(tx, rx);
	if (result >= 0)
	{
		transitions = rx[4] + 256 * rx[5];
		uint16_t time = rx[6] + 256 * rx[7];
		if (rx[3] == 1)
		{
			return(time);
		}
	}
	return(0);
}

bool SerialWombatMatrixButton::readTransitionsState()
{
	return digitalRead();
}