/*
Copyright 2021 Broadwell Consulting Inc.

Serial Wombat is a registered trademark of Broadwell Consulting Inc. in the United States.

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
#include "SerialWombat.h"

SerialWombatTM1637::SerialWombatTM1637(SerialWombatChip& serialWombat): SerialWombatPin(serialWombat)
{
}

int16_t SerialWombatTM1637::begin(uint8_t clkPin, uint8_t dioPin, uint8_t digits, SWTM1637Mode mode, uint8_t dataSourcePin,   uint8_t brightness0to7)
{
	_pin = clkPin;
	uint8_t tx_200[8] = {
		200, // Pin Set command
		clkPin,
		11, // TM1637
		dioPin,
		digits,
		(uint8_t)mode,
		dataSourcePin,
		brightness0to7,
	};
	int16_t retval = _sw.sendPacket(tx_200);
    return retval;
}

int16_t SerialWombatTM1637::writeDigitOrder(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t fifth, uint8_t sixth)
{
	uint8_t tx_201[8] = {
		201, // Pin Set command
		_pin,
		11, // TM1637
		first,
		second,
		third,
		fourth,
		fifth,
	};
	int retval = _sw.sendPacket(tx_201);
	if (retval < 0)
	{
		return retval;
	}
	uint8_t tx_202[8] = {
		202, // Pin Set command
		_pin,
		11, // TM1637
		sixth,
		0x55,
		0x55,
		0x55,
		0x55,
	};
	return _sw.sendPacket(tx_202);
}

int16_t SerialWombatTM1637::writeArray(uint8_t data[6])
{
	uint8_t tx_204[8] = {
		204, // Pin Set command
		_pin,
		11, // TM1637
		data[0],
		data[1],
		data[2],
		data[3],
		data[4],
	};
	int retval = _sw.sendPacket(tx_204);
	if (retval < 0)
	{
		return retval;
	}
	uint8_t tx_205[8] = {
		205, // Pin Set command
		_pin,
		11, // TM1637
		data[5],
		0x55,
		0x55,
		0x55,
		0x55,
	};
	return _sw.sendPacket(tx_205);
}

int16_t SerialWombatTM1637::writeDecimalBitmap(uint8_t decimalBitmapLSBleftDigit)
{
	uint8_t tx_206[8] = {
		206, // Pin Set command
		_pin,
		11, // TM1637
		decimalBitmapLSBleftDigit,
		0x55,
		0x55,
		0x55,
		0x55,
	};
	return _sw.sendPacket(tx_206);
}

int16_t SerialWombatTM1637::writeBrightness(uint8_t brightness0to7)
{
	uint8_t tx_203[8] = {
		203, // Pin Set command
		_pin,
		11, // TM1637
		brightness0to7,
		0x55,
		0x55,
		0x55,
		0x55,
	};
	return _sw.sendPacket(tx_203);
}

int16_t SerialWombatTM1637::writeAnimation(uint16_t bufferIndex, uint16_t delay, uint8_t numberOfFrames, uint8_t data[][6])
{
	int result = _sw.writeUserBuffer(bufferIndex, (uint8_t*)data, numberOfFrames * 6);

	if (result < 0)
	{
		return result;
	}
	uint8_t settings[] = { SW_LE16(bufferIndex), SW_LE16(delay), numberOfFrames, 0 };
	return writeArray(settings);

}

int16_t SerialWombatTM1637::suppressLeadingZeros(bool suppress)
{
	uint8_t tx_203[8] = {
		203, // Pin Set command
		_pin,
		11, // TM1637
		0x55, // Don't change brightness
		suppress?(uint8_t)1:(uint8_t)0,
		0x55,
		0x55,
		0x55,
	};
	return _sw.sendPacket(tx_203);
}

int16_t SerialWombatTM1637::writeBlinkBitmap(uint8_t decimalBitmapLSBleftDigit)
{
	uint8_t tx_206[8] = {
		207, // Pin Set command
		_pin,
		11, // TM1637
		decimalBitmapLSBleftDigit,
		0x55,
		0x55,
		0x55,
		0x55,
	};
	return _sw.sendPacket(tx_206);
}

size_t SerialWombatTM1637::write(uint8_t data)
{
	uint8_t tx[8] = {
		(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE8, // Pin Set command
		_pin,
		PIN_MODE_TM1637, // TM1637
		1,
		data,
		0x55,
		0x55,
		0x55,
	};
	if (_sw.sendPacket(tx) < 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

size_t SerialWombatTM1637::write(const uint8_t* buffer, size_t size)
{
	size_t initialSize = size;
	if (size > 6)
	{
		buffer += size - 6;
		size = 6;
	}
	if (size > 4)
	{
		uint8_t tx[8] = {
		(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE8, // Pin Set command
		_pin,
		PIN_MODE_TM1637, // TM1637
		4,
		buffer[0],
		buffer[1],
		buffer[2],
		buffer[3],
		};
		_sw.sendPacket(tx);
		size -= 4;
		buffer += 4;
	}

	{
		uint8_t tx[8] = {
		(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE8, // Pin Set command
		_pin,
		PIN_MODE_TM1637, // TM1637
		(uint8_t)size,
		buffer[0],
		size > 1 ? buffer[1] : (uint8_t)0x55,
		size > 2 ? buffer[2] : (uint8_t)0x55,
	    size > 3 ? buffer[3] : (uint8_t)0x55,
		};
		_sw.sendPacket(tx);
	}
	return size_t(initialSize);
}
