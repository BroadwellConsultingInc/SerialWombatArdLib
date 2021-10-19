#include "SerialWombatTM1637.h"
/*
Copyright 2021 Broadwell Consulting Inc.

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

SerialWombatTM1637::SerialWombatTM1637(SerialWombat& serialWombat)
{
	_sw = &serialWombat;
}

int SerialWombatTM1637::begin(uint8_t clkPin, uint8_t dioPin, uint8_t digits, SWTM1637Mode mode, uint8_t dataSourcePin,   uint8_t brightness0to7)
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
	int retval = _sw->sendPacket(tx_200);	
    return retval;
}

int SerialWombatTM1637::orderDigits(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t fifth, uint8_t sixth)
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
	int retval = _sw->sendPacket(tx_201);
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
	return _sw->sendPacket(tx_202);
}

int SerialWombatTM1637::setArray(uint8_t data[6])
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
	int retval = _sw->sendPacket(tx_204);
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
	return _sw->sendPacket(tx_205);
}

int SerialWombatTM1637::setDecimalBitmap(uint8_t decimalBitmapLSBleftDigit)
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
	return _sw->sendPacket(tx_206);
}

int SerialWombatTM1637::setBrightness(uint8_t brightness0to7)
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
	return _sw->sendPacket(tx_203);
}

int SerialWombatTM1637::setAnimation(uint16_t bufferIndex, uint16_t delay, uint8_t numberOfFrames, uint8_t data[][6])
{
	int result = _sw->writeUserBuffer(bufferIndex, (uint8_t*)data, numberOfFrames * 6);

	if (result < 0)
	{
		return result;
	}
	uint8_t settings[] = { bufferIndex & 0xFF,bufferIndex >> 8, delay & 0xFF, delay >> 8, numberOfFrames, 0 };
	setArray(settings);

}

