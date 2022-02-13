#include "SerialWombatThroughputConsumer.h"
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

SerialWombatThroughputConsumer::SerialWombatThroughputConsumer(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat)
{
	_sw = serialWombat;
}

int16_t SerialWombatThroughputConsumer::begin(uint8_t pin)
{
	_pin = pin;
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,pin,
					(uint8_t)PIN_MODE_THROUGHPUT_CONSUMER,
					0x55,0x55,0x55,0x55,0x55 };
	return _sw.sendPacket(tx);
}

int16_t SerialWombatThroughputConsumer::writeAll(uint16_t delay)
{
	for (int i = 0; i < 16; ++i)
	{
		uint8_t tx[] = {(uint8_t) SerialWombatCommands::CONFIGURE_PIN_MODE1,
						_pin,
						(uint8_t) PIN_MODE_THROUGHPUT_CONSUMER ,
						(uint8_t)i,
						SW_LE16(delay), 0x55,0x55 };
		int16_t result = _sw.sendPacket(tx);
		if (result < 0)
		{
			return (result);
		}
	}
	return 0;
}

int16_t SerialWombatThroughputConsumer::write(uint8_t frame, uint16_t delay)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
						_pin,
						(uint8_t)PIN_MODE_THROUGHPUT_CONSUMER ,
						frame,
						SW_LE16(delay), 0x55,0x55 };
	return( _sw.sendPacket(tx));
}

int16_t SerialWombatThroughputConsumer::delayInCommProcessing(uint16_t delay)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE2,
						_pin,
						(uint8_t)PIN_MODE_THROUGHPUT_CONSUMER ,
						SW_LE16(delay), 0x55,0x55, 0x55 };
	return(_sw.sendPacket(tx));
}
