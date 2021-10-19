#include "SerialWombat18CapTouch.h"

SerialWombat18CapTouch::SerialWombat18CapTouch(SerialWombat& serialWombat) :_sw(serialWombat)
{
    _sw = serialWombat;
}

int16_t SerialWombat18CapTouch::begin(uint8_t pin, uint16_t chargeTime)
{
    begin(pin, chargeTime, 10);
}

int16_t SerialWombat18CapTouch::begin(uint8_t pin, uint16_t chargeTime, uint16_t delay)
{
    _pin = pin;

    uint8_t tx[8] = { 200,_pin, 22, SW_LE16(chargeTime), SW_LE16(delay),0x55 };

    return _sw.sendPacket(tx);
}

int16_t SerialWombat18CapTouch::makeDigital(uint16_t touchLimit, uint16_t noTouchLimit)
{
    return makeDigital(touchLimit, noTouchLimit, 1, 0, 0, 0);
}

int16_t SerialWombat18CapTouch::makeDigital(uint16_t touchLimit, uint16_t noTouchLimit, uint16_t touchValue, uint16_t noTouchValue, bool invert, uint16_t debounceCount)
{
    uint8_t tx1[8] = { 201,_pin,22,SW_LE16(touchLimit), SW_LE16(noTouchLimit),0x55 };
    int16_t result = _sw.sendPacket(tx1);
    if (result < 0)
    {
        return (result);
    }

    _trueOutput = touchValue;
    _falseOutput = noTouchValue;
    uint8_t tx2[8] = { 202,_pin,22,SW_LE16(touchValue), SW_LE16(noTouchValue),0x55 };
    result = _sw.sendPacket(tx2);
    if (result < 0)
    {
        return (result);
    }
    uint8_t tx3[8] = { 203,_pin,22,1, invert?1:0,SW_LE16(debounceCount),0x55 };
    result = _sw.sendPacket(tx3);
    if (result < 0)
    {
        return (result);
    }
    return(0);


}

int16_t SerialWombat18CapTouch::makeAnalog()
{
    uint8_t tx3[8] = { 203,_pin,22,0, 0x55,0x55,0x55,0x55 };
   return _sw.sendPacket(tx3);
}

bool SerialWombat18CapTouch::digitalRead()
{
    uint8_t tx[8] = { 204,_pin,22,0,0x55,0x55,0x55,0x55 };
    uint8_t rx[8];
    if (_sw.sendPacket(tx, rx) >= 0)
    {
        return(rx[3] > 0);
    }
    return(false);

}

uint16_t SerialWombat18CapTouch::readDurationInTrueState_mS()
{
    uint8_t tx[8] = { 204,_pin,22,1,0x55,0x55,0x55,0x55 };
    uint8_t rx[8];
    _sw.sendPacket(tx, rx);

    transitions += (256 * rx[5] + rx[4]);
    if (rx[3] == 0)
    {
        return (0);
    }
    else
    {
        return(256 * rx[7] + rx[6]);
    }
}

uint16_t SerialWombat18CapTouch::readDurationInFalseState_mS()
{
    uint8_t tx[8] = { 204,_pin,22,1,0x55,0x55,0x55,0x55 };
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

bool SerialWombat18CapTouch::readTransitionsState()
{
    uint8_t tx[8] = { 204,_pin,22,1,0x55,0x55,0x55,0x55 };
    uint8_t rx[8];
    _sw.sendPacket(tx, rx);
    transitions = (256 * rx[5] + rx[4]);
    return (rx[3] > 0);
}











SerialWombat18CapTouchCounter::SerialWombat18CapTouchCounter(SerialWombat18CapTouch* capTouch)
{
	_capTouch = capTouch;
}

void SerialWombat18CapTouchCounter::begin(long* variableToIncrement, long slowIncrement, unsigned long slow_mS_betweenIncrements, uint16_t slowToMediumTransition_mS, long mediumIncrement, unsigned long medium_mS_betweenIncrements, uint16_t mediumToFastTransistion_mS, long fastIncrement, unsigned long fast_mS_betweenIncrements)
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

bool SerialWombat18CapTouchCounter::update()
{
	uint16_t pressDuration = _capTouch->readDurationInTrueState_mS();
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
			_capTouch->transitions = 0;  // Get rid of false->true transition so that final release doesn't cause and increment
		}
		pressed = true;
	}
	else
	{
		// Button isn't currently pressed.  if there were other transitions, add them
		_lastPressDuration = 0;
		int presses = _capTouch->transitions / 2;
		*_variableToIncrement += _slowIncrement * presses;
		_capTouch->transitions -= presses * 2;
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

