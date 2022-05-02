#include "SerialWombatPulseOnChange.h"

SerialWombatPulseOnChange::SerialWombatPulseOnChange(SerialWombatChip& serialWombat): SerialWombatPin(serialWombat)
{
	_pinMode = PIN_MODE_PULSE_ON_CHANGE;
}

int16_t SerialWombatPulseOnChange::begin(uint8_t pin, SerialWombatPinState_t activeMode, SerialWombatPinState_t inactiveMode, uint16_t pulseOnTime, uint16_t pulseOffTime, uint8_t orNotAnd, uint16_t PWMperiod, uint16_t PWMdutyCycle)
{
	int16_t result;
	_pin = pin;

	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
						_pin,
						_pinMode,
						activeMode,
						inactiveMode,
						orNotAnd,
						0x55,
						0x55
		};
		result = _sw.sendPacket(tx);
		if (result < 0) { return result; }
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
						_pin,
						_pinMode,
						SW_LE16(pulseOnTime),
			SW_LE16(pulseOffTime),
						0x55
		};
		result = _sw.sendPacket(tx);
		if (result < 0) { return result; }
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE2,
						_pin,
						_pinMode,
						SW_LE16(PWMperiod),
			SW_LE16(PWMdutyCycle),
						0x55
		};
		result = _sw.sendPacket(tx);
		if (result < 0) { return result; }
	}
	return(0);
}

int16_t SerialWombatPulseOnChange::setEntryOnChange(uint8_t entryID, uint8_t sourcePin)
{
	return setEntryMode(entryID, sourcePin, 0);
}

int16_t SerialWombatPulseOnChange::setEntryOnIncrease(uint8_t entryID, uint8_t sourcePin)
{
	return setEntryMode(entryID, sourcePin, 1);
}

int16_t SerialWombatPulseOnChange::setEntryOnDecrease(uint8_t entryID, uint8_t sourcePin)
{
	return setEntryMode(entryID, sourcePin, 2);
}

int16_t SerialWombatPulseOnChange::setEntryOnEqualValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 3);
}

int16_t SerialWombatPulseOnChange::setEntryOnLessThanValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 4);
}

int16_t SerialWombatPulseOnChange::setEntryOnGreaterThanValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 5);
}

int16_t SerialWombatPulseOnChange::setEntryOnNotEqualValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 6);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinsEqual(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin)
{
	int16_t result = setEntryParams(entryID, secondPin, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 7);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinsNotEqual(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin)
{
	int16_t result = setEntryParams(entryID, secondPin, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 10);
}
/*
int16_t SerialWombatPulseOnChange::setEntryOnPinGTPin(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin)
{
	int16_t result = setEntryParams(entryID, secondPin, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 8);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinLTPin(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin)
{
	int16_t result = setEntryParams(entryID, secondPin, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 9);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinCrossValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 11);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinCrossAscending(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 12);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinCrossDescending(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 12);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinWithinRange(uint8_t entryID, uint8_t sourcePin, uint16_t lowValue, uint16_t highValue)
{
	int16_t result = setEntryParams(entryID, lowValue, highValue);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 14);
}

int16_t SerialWombatPulseOnChange::setEntryOnPinOutsideRange(uint8_t entryID, uint8_t sourcePin, uint16_t lowValue, uint16_t highValue)
{
	int16_t result = setEntryParams(entryID, lowValue, highValue);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 15);
}
*/
int16_t SerialWombatPulseOnChange::setEntryParams(uint8_t entryID, uint16_t firstParam, uint16_t secondParam)
{
	{
		int16_t result;
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
						_pin,
						_pinMode,
						entryID,
						SW_LE16(firstParam),
						SW_LE16(secondParam),
		};
		result = _sw.sendPacket(tx);
		if (result < 0) { return result; }
	}
	return 0;
}

int16_t SerialWombatPulseOnChange::setEntryMode(uint8_t entryID, uint8_t pin, uint8_t mode)
{
	{
		int16_t result;
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE4,
						_pin,
						_pinMode,
						entryID,
						mode,
						pin,
						
			0x55,
			0x55
		};
		result = _sw.sendPacket(tx);
		if (result < 0) { return result; }
	}
	return 0;
}
