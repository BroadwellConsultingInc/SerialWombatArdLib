#pragma once
#include <stdint.h>
#include "SerialWombat.h"

class SerialWombat;

class SerialWombatPWM
{
public:
	SerialWombatPWM(SerialWombat& serialWombat, uint8_t pin);

	void begin();
	void begin(uint16_t dutyCycle);
	void begin(uint16_t dutyCycle,bool invert);
	void writeDutyCycle(uint16_t dutyCycle);
	~SerialWombatPWM();

private:
	SerialWombat* _sw;
	uint8_t _pin = 255;
};

