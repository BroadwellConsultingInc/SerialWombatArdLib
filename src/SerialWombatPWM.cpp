#include "SerialWombatPWM.h"


SerialWombatPWM::SerialWombatPWM(SerialWombatChip& serialWombatChip)
{
	_sw = &serialWombatChip;
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
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,_pin,PIN_MODE_PWM,_pin,(uint8_t)(dutyCycle & 0xFF),(uint8_t)(dutyCycle >> 8),invert,0x55 };
	_sw->sendPacket(tx);
}
void SerialWombatPWM:: writeDutyCycle(uint16_t dutyCycle)
{	
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_SET_PIN_BUFFFER,_pin,(uint8_t)(dutyCycle & 0xFF),(uint8_t)(dutyCycle >>8),255,0x55,0x55 };
	_sw->sendPacket(tx);
}

void SerialWombatPWM::setFrequency_SW4AB(Wombat4A_B_PWMFrequencyValues_t frequency)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE_HW_0,_pin,PIN_MODE_PWM,(uint8_t)(frequency),0x55,0x55,0x55,0x55 };
	_sw->sendPacket(tx);
}

void SerialWombatPWM::setFrequency_SW18AB_Hz(uint32_t frequency_Hz)
{
	uint8_t tx[] = { 220,_pin,PIN_MODE_PWM,SW_LE32(1000000 / frequency_Hz),0x55 };
	_sw->sendPacket(tx);

}

void SerialWombatPWM::setPeriod_SW18AB_uS(uint32_t period_uS)
{
	uint8_t tx[] = { 220,_pin,PIN_MODE_PWM,SW_LE32(period_uS),0x55 };
	_sw->sendPacket(tx);
}





SerialWombatPWM::~SerialWombatPWM()
{
}
