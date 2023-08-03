#include "SerialWombat.h"
#include <stdint.h>
#include "SerialWombatPS2Keyboard.h"

SerialWombatPS2Keyboard::SerialWombatPS2Keyboard(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat)
{
}

int16_t SerialWombatPS2Keyboard::begin(uint8_t clockPin, uint8_t dataPin, uint8_t queueMode ,  uint8_t bufferMode , uint16_t queueAddress , uint8_t pullUpDown)
{
	_pin = clockPin;
	_pinMode = (uint8_t)PIN_MODE_PS2KEYBOARD;

	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
						_pin,
						(uint8_t)_pinMode ,
						dataPin,
						queueMode,
						bufferMode,
						0x55,
						 pullUpDown};
	int16_t result = _sw.sendPacket(tx);
	if (result < 0)
	{
		return result;
	}

	if (queueAddress != 0xFFFF)
	{
	uint8_t tx6[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
						_pin,
						_pinMode,
						SW_LE16(queueAddress),
						0x55,
						0x55,
						0x55 };

	return _sw.sendPacket(tx6);
	}
	return (result);
}


int SerialWombatPS2Keyboard::available()
{
	uint8_t tx[8] = { 201, _pin, _pinMode, 0,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	return (rx[4]);
}

int SerialWombatPS2Keyboard::read()
{
	uint8_t tx[8] = { 202, _pin,_pinMode, 1,0x55,0x55,0x55,0x55 };
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

void SerialWombatPS2Keyboard::flush()
{
	
}

int SerialWombatPS2Keyboard::peek()
{
	uint8_t tx[8] = { 203, _pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
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

size_t SerialWombatPS2Keyboard::write(uint8_t data)
{
	
	return (1);
}

size_t SerialWombatPS2Keyboard::write(const uint8_t* buffer, size_t size)
{
	return(size);
}

int SerialWombatPS2Keyboard::availableForWrite()
{
	return(0);
}

void SerialWombatPS2Keyboard::setTimeout(long timeout_mS)
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

uint8_t SerialWombatPS2Keyboard::scanCodeToAscii(uint8_t scanCode, bool shiftActive)
{

	if (scanCode >= 0x80)
	{
		return 0;
	}
	/// This array is used to convert PS2 Code set 2 codes to Ascii. The first column is unshfited data, the second if shift is active
	const uint8_t KBSCSet2[][2] =
	{
	{ ' ' , ' ' },  /* 0 */
	{ ' ' , ' ' },  /* 1 */
	{ ' ' , ' ' },  /* 2 */
	{ ' ' , ' ' },  /* 3 */
	{ ' ' , ' ' },  /* 4 */
	{ ' ' , ' ' },  /* 5 */
	{ ' ' , ' ' },  /* 6 */
	{ ' ' , ' ' },  /* 7 */
	{ ' ' , ' ' },  /* 8 */
	{ ' ' , ' ' },  /* 9 */
	{ ' ' , ' ' },  /* A */
	{ ' ' , ' ' },  /* B */
	{ ' ' , ' ' },  /* C */
	{ 9 , 9 },  /* D */
	{ '`' , '~' },  /* E */
	{ ' ' , ' ' },  /* F */
	{ ' ' , ' ' },  /* 10 */
	{ ' ' , ' ' },  /* 11 */
	{ 0 , 0 },  /* 12 */
	{ ' ' , ' ' },  /* 13 */
	{ ' ' , ' ' },  /* 14 */
	{ 'q' , 'Q' },  /* 15 */
	{ '1' , '!' },  /* 16 */
	{ ' ' , ' ' },  /* 17 */
	{ ' ' , ' ' },  /* 18 */
	{ ' ' , ' ' },  /* 19 */
	{ 'z' , 'Z' },  /* 1A */
	{ 's' , 'S' },  /* 1B */
	{ 'a' , 'A' },  /* 1C */
	{ 'w' , 'W' },  /* 1D */
	{ '2' , '@' },  /* 1E */
	{ ' ' , ' ' },  /* 1F */
	{ ' ' , ' ' },  /* 20 */
	{ 'c' , 'C' },  /* 21 */
	{ 'x' , 'X' },  /* 22 */
	{ 'd' , 'D' },  /* 23 */
	{ 'e' , 'E' },  /* 24 */
	{ '4' , '$' },  /* 25 */
	{ '3' , '#' },  /* 26 */
	{ ' ' , ' ' },  /* 27 */
	{ ' ' , ' ' },  /* 28 */
	{ ' ' , ' ' },  /* 29 - SPACE */
	{ 'v' , 'V' },  /* 2A */
	{ 'f' , 'F' },  /* 2B */
	{ 't' , 'T' },  /* 2C */
	{ 'r' , 'R' },  /* 2D */
	{ '5' , '%' },  /* 2E */
	{ ' ' , ' ' },  /* 2F */
	{ ' ' , ' ' },  /* 30 */
	{ 'n' , 'N' },  /* 31 */
	{ 'b' , 'B' },  /* 32 */
	{ 'h' , 'H' },  /* 33 */
	{ 'g' , 'G' },  /* 34 */
	{ 'y' , 'Y' },  /* 35 */
	{ '6' , '^' },  /* 36 */
	{ ' ' , ' ' },  /* 37 */
	{ ' ' , ' ' },  /* 38 */
	{ ' ' , ' ' },  /* 39 */
	{ 'm' , 'M' },  /* 3A */
	{ 'j' , 'J' },  /* 3B */
	{ 'u' , 'U' },  /* 3C */
	{ '7' , '&' },  /* 3D */
	{ '8' , '*' },  /* 3E */
	{ ' ' , ' ' },  /* 3F */
	{ ' ' , ' ' },  /* 40 */
	{ ',' , '>' },  /* 41 */
	{ 'k' , 'K' },  /* 42 */
	{ 'i' , 'I' },  /* 43 */
	{ 'o' , 'O' },  /* 44 */
	{ '0' , ')' },  /* 45 */
	{ '9' , '(' },  /* 46 */
	{ ' ' , ' ' },  /* 47 */
	{ ' ' , ' ' },  /* 48 */
	{ '.' , '<' },  /* 49 */
	{ '/' , '?' },  /* 4A */
	{ 'l' , 'L' },  /* 4B */
	{ ';' , ':' },  /* 4C */
	{ 'p' , 'P' },  /* 4D */
	{ '-' , '_' },  /* 4E */
	{ ' ' , ' ' },  /* 4F */
	{ ' ' , ' ' },  /* 50 */
	{ ' ' , ' ' },  /* 51 */
	{ '\'' , '"' },  /* 52 */
	{ ' ' , ' ' },  /* 53 */
	{ '[' , '{' },  /* 54 */
	{ '=' , '+' },  /* 55 */
	{ ' ' , ' ' },  /* 56 */
	{ ' ' , ' ' },  /* 57 */
	{ ' ' , ' ' },  /* 58 */
	{ 0 , 0 },  /* 59 */
	{ 0xD , 0xD },  /* 5A */
	{ ']' , '}' },  /* 5B */
	{ ' ' , ' ' },  /* 5C */
	{ '\\' , '|' },  /* 5D */
	{ ' ' , ' ' },  /* 5E */
	{ ' ' , ' ' },  /* 5F */
	{ ' ' , ' ' },  /* 60 */
	{ ' ' , ' ' },  /* 61 */
	{ ' ' , ' ' },  /* 62 */
	{ ' ' , ' ' },  /* 63 */
	{ ' ' , ' ' },  /* 64 */
	{ ' ' , ' ' },  /* 65 */
	{ 8 , 8 },  /* 66 */
	{ ' ' , ' ' },  /* 67 */
	{ ' ' , ' ' },  /* 68 */
	{ '1' , '1' },  /* 69 */
	{ ' ' , ' ' },  /* 6A */
	{ '4' , '4' },  /* 6B */
	{ '7' , '7' },  /* 6C */
	{ ' ' , ' ' },  /* 6D */
	{ ' ' , ' ' },  /* 6E */
	{ ' ' , ' ' },  /* 6F */
	{ '0' , '0' },  /* 70 */
	{ ' ' , ' ' },  /* 71 */
	{ '2' , '2' },  /* 72 */
	{ '5' , '5' },  /* 73 */
	{ '6' , '6' },  /* 74 */
	{ '8' , '8' },  /* 75 */
	{ 0x1B , 0x1B },  /* 76 */
	{ ' ' , ' ' },  /* 77 */
	{ ' ' , ' ' },  /* 78 */
	{ ' ' , ' ' },  /* 79 */
	{ '3' , '3' },  /* 7A */
	{ ' ' , ' ' },  /* 7B */
	{ ' ' , ' ' },  /* 7C */
	{ '9' , '9' },  /* 7D */
	{ ' ' , ' ' },  /* 7E */
	{ ' ' , ' ' },  /* 7F */
	};

	if (shiftActive)
	{
		return ( KBSCSet2[scanCode][ 1]);
	}

	return  (  KBSCSet2[scanCode][0]);
}

bool SerialWombatPS2Keyboard::isKeyPressed(uint8_t scanCode)
{
	uint8_t buffer[5];
	int16_t result = readCurrentScanCodes(buffer, scanCode);

	if (result < 0)
	{
		return false;
	}

	if (buffer[0] == scanCode)
	{
		return true;
	}

	return false;

}

bool SerialWombatPS2Keyboard::isKeyPressed(PS2KeyboardScanCode scanCode)
{
	return isKeyPressed((uint8_t) scanCode);
}


size_t SerialWombatPS2Keyboard::readBytes(char* buffer, size_t length)
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

			uint8_t tx[8] = { 202, _pin,_pinMode, (uint8_t)bytecount,0x55,0x55,0x55,0x55 };
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

int16_t SerialWombatPS2Keyboard::readCurrentScanCodes(uint8_t* buffer, uint8_t startValue)
{
	uint8_t tx[8] = { 207, _pin,_pinMode, startValue,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	int16_t result = _sw.sendPacket(tx, rx);

	if (result < 0)
	{
		return result;
	}
	int count = 0;
	for (int i = 3; i < 8; ++i)
	{
		if (rx[i] != 0)
		{
			++count;
		}
		buffer[i - 3] = rx[i];
	}
	return count;
}


