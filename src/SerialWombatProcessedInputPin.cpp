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


SerialWombatProcessedInputPin::SerialWombatProcessedInputPin(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat),SerialWombatAbstractProcessedInput(serialWombat)
{
}

int16_t SerialWombatProcessedInputPin::begin(uint8_t pin, uint8_t dataSourcePin)
{
	delay(500); //TODO
	_pin = pin;
	_pinMode = (uint8_t)SerialWombatPinMode_t::PIN_MODE_INPUT_PROCESSOR;
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
	_pin,
	_pinMode,
	dataSourcePin,
	0x55,
	0x55,
	0x55,
	0x55 };
	return _sw.sendPacket(tx);
}