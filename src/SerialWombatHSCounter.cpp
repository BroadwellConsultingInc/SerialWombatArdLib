#include "SerialWombatHSCounter.h"

SerialWombatHSCounter::SerialWombatHSCounter(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat), SerialWombatAbstractProcessedInput(serialWombat)
{
}

int16_t SerialWombatHSCounter::begin(uint8_t pin, SerialWombatHSCounter::publicDataOutput publicDataOutput, uint16_t framesBetweenUpdates, uint16_t publicOutputDivisor)
{
	_pin = pin;
	_pinMode = PIN_MODE_HS_COUNTER;

	uint8_t tx[8] =
	{
	(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
	_pin,
	_pinMode,
	SW_LE16(framesBetweenUpdates),
	SW_LE16(publicOutputDivisor),
	(uint8_t)publicDataOutput };

	return(_sw.sendPacket(tx));

}

uint32_t SerialWombatHSCounter::readCounts(bool resetCounts)
{
	uint8_t tx[8] =
	{
	(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
	_pin,
	_pinMode,
	(uint8_t)(resetCounts?1:0),
	0x55,
	0x55,
	0x55,
	0x55};
	uint8_t rx[8];
	int16_t result = _sw.sendPacket(tx, rx);
	if (result < 0)
	{
		return 0;
	}
	uint32_t returnval = (((uint32_t)rx[6]) << 24) + (((uint32_t)rx[5]) << 16) + (((uint16_t)rx[4]) << 8) + rx[3];
	return(returnval);
}

int32_t SerialWombatHSCounter::readFrequency()
{
	uint8_t tx[8] =
	{
	(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
	_pin,
	_pinMode,
	0x55,
	0x55,
	0x55,
	0x55,
	0x55 };
	uint8_t rx[8];
	int16_t result = _sw.sendPacket(tx, rx);
	if (result < 0)
	{
		return result;
	}
	uint32_t returnval = (((uint32_t)rx[6]) << 24) + (((uint32_t)rx[5]) << 16) + (((uint16_t)rx[4]) << 8) + rx[3];
	return(returnval);
}

int16_t SerialWombatHSCounter::disable()
{
	uint8_t tx[] =
	{
		(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE_DISABLE,
		_pin,
		_pinMode,
		0x55,0x55,0x55,0x55,0x55
	};
	return _sw.sendPacket(tx);
}
