#include "SerialWombatDebouncedInput.h"


SerialWombatDebouncedInput::SerialWombatDebouncedInput(SerialWombat& serialWombat):_sw(serialWombat)
{
	_sw = serialWombat;
}
void SerialWombatDebouncedInput::begin(uint8_t pin)
{
	begin(pin, 30, true, true);
}

void SerialWombatDebouncedInput::begin(uint8_t pin, uint16_t debounce_mS, bool invert, bool usePullUp)
{
	_pin = pin;
	uint8_t tx[8] = { 200,_pin,10,SW_LE16(debounce_mS),invert,0,usePullUp };
	_sw.sendPacket(tx);
}

bool SerialWombatDebouncedInput::digitalRead()
{
	return (_sw.readPublicData(_pin) > 0);
}


uint16_t SerialWombatDebouncedInput::readDurationInTrueState_mS()
{
	
	uint8_t tx[8] = { 201,_pin,10,1,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx,rx);

	transitions +=  (256 * rx[5] + rx[4]);
	if (rx[3] == 0)
	{
		return (0);
	}
	else
	{
		return(256 * rx[7] + rx[6]);
	}
}

uint16_t SerialWombatDebouncedInput::readDurationInFalseState_mS()
{

	uint8_t tx[8] = { 201,_pin,10,1,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);

	transitions += (256 * rx[5] + rx[4]);

	if (rx[3] == 1)
	{
		return (0);
	}
	else
	{
		return(256 * rx[7] + rx[6]);
	}
}

bool SerialWombatDebouncedInput::readTransitionsState()
{
	uint8_t tx[8] = { 201,_pin,10,1,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	transitions = (256 * rx[5] + rx[4]);
	return (rx[3] > 0);
}

SerialWombatButtonCounter::SerialWombatButtonCounter( SerialWombatDebouncedInput* serialWombatDebouncedInput)
{
	_debouncedInput = serialWombatDebouncedInput;
}

void SerialWombatButtonCounter::begin(long* variableToIncrement, long slowIncrement,unsigned long slow_mS_betweenIncrements,  uint16_t slowToMediumTransition_mS, long mediumIncrement, unsigned long medium_mS_betweenIncrements, uint16_t mediumToFastTransistion_mS, long fastIncrement, unsigned long fast_mS_betweenIncrements)
{
	_variableToIncrement = variableToIncrement;
	
	_slowIncrement = slowIncrement;
	_slow_mS_betweenIncrements = slow_mS_betweenIncrements;

	_slowToMediumTransition_mS = slowToMediumTransition_mS;
	
	_mediumIncrement = mediumIncrement;
	_medium_mS_betweenIncrements = medium_mS_betweenIncrements;

	_mediumToFastTransistion_mS = mediumToFastTransistion_mS;

	_fastIncrement = fastIncrement;
	_fast_mS_betweenIncrements = fast_mS_betweenIncrements;

	_lastPressDuration = 0;

}

bool SerialWombatButtonCounter::update()
{
	uint16_t pressDuration = _debouncedInput->readDurationInTrueState_mS();
	int increments = 0;
	bool incremented = false;
	bool pressed = false;
	if (pressDuration > 0)
	{
		if (_lastPressDuration >= pressDuration)
		{
			_lastPressDuration = 0;
		}

		if (pressDuration > _mediumToFastTransistion_mS)
		{
			// Increment fast
			increments = (pressDuration - _lastPressDuration) / _fast_mS_betweenIncrements;
			*_variableToIncrement += _fastIncrement * increments;
			_lastPressDuration += _fast_mS_betweenIncrements * increments;
		}
		else if (pressDuration > _slowToMediumTransition_mS)
		{
			// Increment medium
			increments = (pressDuration - _lastPressDuration) / _medium_mS_betweenIncrements;
			*_variableToIncrement += _mediumIncrement * increments;
			_lastPressDuration += _medium_mS_betweenIncrements * increments;
		}
		else
		{
			//Increment slow
			increments = (pressDuration - _lastPressDuration) / _slow_mS_betweenIncrements;
			*_variableToIncrement += _slowIncrement * increments;
			_lastPressDuration += _slow_mS_betweenIncrements * increments;
			incremented = increments > 0;  // An increment happened
		}
		if (incremented)
		{
			_debouncedInput->transitions = 0;  // Get rid of false->true transition so that final release doesn't cause and increment
		}
		pressed = true;
	}
	else 
	{
		// Button isn't currently pressed.  if there were other transitions, add them
		_lastPressDuration = 0;
		int presses = _debouncedInput->transitions / 2;
		 *_variableToIncrement += _slowIncrement * presses;
		 _debouncedInput->transitions -= presses * 2;
	}

	if (*_variableToIncrement > highLimit)
	{
		*_variableToIncrement = highLimit;
	}
	if (*_variableToIncrement < lowLimit)
	{
		*_variableToIncrement = lowLimit;
	}

	return (pressed);
}
