#pragma once
#include <stdint.h>
#include "SerialWombat.h"

class SerialWombat;

typedef enum
{
	QE_ONLOW_INT = 0,
	QE_ONHIGH_INT = 1,
	QE_ONBOTH_INT = 2,
	QE_ONLOW_POLL = 4,
	QE_ONHIGH_POLL = 5,
	QE_ONBOTH_POLL = 6
} QE_READ_MODE_t;
class SerialWombatQuadEnc
{
public:
	SerialWombatQuadEnc(SerialWombat& serialWombat);

	void begin(uint8_t pin, uint8_t secondPin);
	void begin(uint8_t pin, uint8_t secondPin, uint16_t debounce_mS, bool pullUpsEnabled);
	void begin(uint8_t pin, uint8_t secondPin, uint16_t debounce_mS, bool pullUpsEnabled, QE_READ_MODE_t readState);
	uint16_t read();
	uint16_t read(uint16_t replacementValue);
	void write(uint16_t value);
private:
	SerialWombat& _sw;
	uint8_t _pin = 255;
	uint8_t _secondPin;
};

