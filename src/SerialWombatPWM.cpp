#include "serialWombatPWM.h"


SerialWombatPWM::SerialWombatPWM(SerialWombat& SerialWombat, uint8_t pin)
{
	_sw = &SerialWombat;
	_pin = pin;
	
}

void SerialWombatPWM::begin()
{
	begin(0, false);
}
void SerialWombatPWM::begin(uint16_t dutyCycle)
{
	begin(dutyCycle, false);
}


void SerialWombatPWM::begin(uint16_t dutyCycle, bool invert)
{
	uint8_t tx[] = { CMD_SET_PIN_MODE0,_pin,PIN_MODE_PWM,_pin,(uint8_t)(dutyCycle & 0xFF),(uint8_t)(dutyCycle >> 8),invert,0x55 };
	uint8_t rx[8];
	_sw->sendPacket(tx, rx);
}
void SerialWombatPWM:: writeDutyCycle(uint16_t dutyCycle)
{	
	uint8_t tx[] = { CMD_SETBUFFERBINARY,_pin,(uint8_t)(dutyCycle & 0xFF),(uint8_t)(dutyCycle >>8),255,0x55,0x55 };
	uint8_t rx[8];
	_sw->sendPacket(tx, rx);
}



SerialWombatPWM::~SerialWombatPWM()
{
}
