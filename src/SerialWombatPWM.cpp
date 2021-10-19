#include "SerialWombatPWM.h"


SerialWombatPWM::SerialWombatPWM(SerialWombat& SerialWombat)
{
	_sw = &SerialWombat;
}

void SerialWombatPWM::begin(uint8_t pin)
{
	begin(pin, 0, false);
}
void SerialWombatPWM::begin(uint8_t pin, uint16_t dutyCycle)
{
	begin(pin, dutyCycle, false);
}


void SerialWombatPWM::begin(uint8_t pin, uint16_t dutyCycle, bool invert)
{
	_pin = pin;
	uint8_t tx[] = { CMD_SET_PIN_MODE0,_pin,PIN_MODE_PWM,_pin,(uint8_t)(dutyCycle & 0xFF),(uint8_t)(dutyCycle >> 8),invert,0x55 };
	_sw->sendPacket(tx);
}
void SerialWombatPWM:: writeDutyCycle(uint16_t dutyCycle)
{	
	uint8_t tx[] = { CMD_SETBUFFERBINARY,_pin,(uint8_t)(dutyCycle & 0xFF),(uint8_t)(dutyCycle >>8),255,0x55,0x55 };
	_sw->sendPacket(tx);
}

void SerialWombatPWM::setFrequency_SW4AB(Wombat4A_B_PWMFrequencyValues_t frequency)
{
	uint8_t tx[] = { 220,_pin,PIN_MODE_PWM,(uint8_t)(frequency),0x55,0x55,0x55,0x55 };
	_sw->sendPacket(tx);
}

void SerialWombatPWM::setFrequency_SW18AB_Hz(uint16_t frequency_Hz)
{
	uint8_t tx[] = { 220,_pin,PIN_MODE_PWM,SW_LE16(1000000 / frequency_Hz),0x55,0x55,0x55 };
	_sw->sendPacket(tx);

}

void SerialWombatPWM::setPeriod_SW18AB_uS(uint16_t period_uS)
{
	uint8_t tx[] = { 220,_pin,PIN_MODE_PWM,SW_LE16(period_uS),0x55,0x55,0x55 };
	_sw->sendPacket(tx);
}





SerialWombatPWM::~SerialWombatPWM()
{
}
