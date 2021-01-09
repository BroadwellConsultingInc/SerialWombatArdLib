#pragma once

#include "SerialWombat.h"
#include "limits.h"
class SerialWombatDebouncedInput
{
public:
	SerialWombatDebouncedInput(SerialWombat& serialWombat);
	void begin(uint8_t pin);
	void begin(uint8_t pin, uint16_t debounce_mS, bool invert, bool usePullUp);
	bool digitalRead();
	uint16_t readDurationInTrueState_mS();
	uint16_t readDurationInFalseState_mS();
	uint16_t transitions = 0;
	bool readTransitionsState();



private:
	SerialWombat& _sw;
	uint8_t _pin = 255;
	

};

class SerialWombatButtonCounter
{
public:
	 SerialWombatButtonCounter( SerialWombatDebouncedInput* serialWombatDebouncedInput);

	 void begin(long* variableToIncrement,
		 long slowIncrement, unsigned long slow_mS_betweenIncrements,
		 uint16_t slowToMediumTransition_mS, 
		 long mediumIncrement,	 unsigned long medium_mS_betweenIncrements, 
		 uint16_t mediumToFastTransistion_mS, 
		 long fastIncrement, unsigned long fast_mS_betweenIncrements);

	bool update();
	long highLimit = LONG_MAX;
	long lowLimit = LONG_MIN;

private:
	SerialWombatDebouncedInput* _debouncedInput;
	long* _variableToIncrement;

	long _slowIncrement;
	unsigned long _slow_mS_betweenIncrements;
	
	uint16_t _slowToMediumTransition_mS;

	long _mediumIncrement; 
	unsigned long _medium_mS_betweenIncrements;
	
	uint16_t _mediumToFastTransistion_mS;

	long _fastIncrement;
	unsigned long _fast_mS_betweenIncrements;

	unsigned long _lastPressDuration;
	
};

