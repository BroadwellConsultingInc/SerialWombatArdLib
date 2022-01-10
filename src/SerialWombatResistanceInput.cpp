#include "SerialWombatResistanceInput.h"


SerialWombatResistanceInput::SerialWombatResistanceInput( SerialWombatChip& serialWombatChip):_sw(serialWombatChip)
{
	_sw = serialWombatChip;
}

int16_t SerialWombatResistanceInput::begin(uint8_t pin)
{
	_pin = pin;
	uint8_t tx[] = { 200,_pin,PIN_MODE_RESISTANCEINPUT,0,0,0,0,0 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	uint8_t tx1[] = { 201,_pin,PIN_MODE_RESISTANCEINPUT,64,0,0xFF,0x80,0 };
	return _sw.sendPacket(tx1, rx);
}

int16_t SerialWombatResistanceInput::begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant, ResistanceInputPublicDataOutput output)
{
	begin(pin);
	uint8_t tx[] = { 201,_pin,PIN_MODE_RESISTANCEINPUT,SW_LE16(averageSamples) ,SW_LE16(filterConstant),(uint8_t)output };
	return _sw.sendPacket(tx);
}

int16_t SerialWombatResistanceInput::begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant)
{
	begin(pin);
	uint8_t tx[] = { 201,_pin,PIN_MODE_RESISTANCEINPUT,SW_LE16(averageSamples) ,SW_LE16(filterConstant),0 };
	return _sw.sendPacket(tx);
}



uint16_t SerialWombatResistanceInput::readFilteredOhms()
{
	uint8_t tx[] = { 204,_pin,PIN_MODE_RESISTANCEINPUT,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	_sw.sendPacket(tx, rx);

	return(rx[5] + rx[6] * 256);
}


uint16_t SerialWombatResistanceInput::readAveragedOhms()
{
	uint8_t tx[] = { 204,_pin,PIN_MODE_RESISTANCEINPUT,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	_sw.sendPacket(tx, rx);

	return(rx[3] + rx[4] * 256);
}



uint16_t SerialWombatResistanceInput::readMaximumOhms(bool resetAfterRead)
{
	uint8_t tx[] = { 203,_pin,PIN_MODE_RESISTANCEINPUT,0,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	if (resetAfterRead)
	{
		tx[3] = 1;
	}
	_sw.sendPacket(tx, rx);

	return(rx[5] + rx[6] * 256);
}

uint16_t SerialWombatResistanceInput::readMinimumOhms(bool resetAfterRead)
{
	uint8_t tx[] = { 203,_pin,PIN_MODE_RESISTANCEINPUT,0,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	if (resetAfterRead)
	{
		tx[3] = 1;
	}
	_sw.sendPacket(tx, rx);

	return(rx[3] + rx[4] * 256);
}



