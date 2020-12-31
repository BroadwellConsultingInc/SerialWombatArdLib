#pragma once
#include <stdint.h>
#include "SerialWombat.h"

class SerialWombat;

class SerialWombatQuadEnc
{
public:
	SerialWombatQuadEnc(SerialWombat& serialWombat, uint8_t pin, uint8_t secondPin);

	void begin();
	void begin(uint16_t debounce_mS, bool pullUpsEnabled);
	void begin(uint16_t debounce_mS, bool pullUpsEnabled, uint8_t readState);
	uint16_t readPosition();
	~SerialWombatQuadEnc();

private:
	SerialWombat& _sw;
	uint8_t _pin;
	uint8_t _secondPin;
};

