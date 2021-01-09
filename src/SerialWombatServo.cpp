#include "SerialWombatServo.h"
#include <stdint.h>


SerialWombatServo::SerialWombatServo(SerialWombat& serialWombat):_sw(serialWombat)
{	
	_sw = serialWombat;
}

void SerialWombatServo::attach(uint8_t pin)
{
	_pin = pin;
	initializeServo();
	

}

void SerialWombatServo::attach(uint8_t pin, bool reverse)
{
	_pin = pin;
	_reverse = reverse;
	initializeServo();
}

void SerialWombatServo::initializeServo()
{
	uint8_t tx[] = { 200,_pin,PIN_MODE_SERVO,_pin,SW_LE16(_position), _reverse,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	uint8_t tx2[] = { 201,_pin,PIN_MODE_SERVO,SW_LE16(_min), SW_LE16(_max-_min),0x55, 0x55 };
	_sw.sendPacket(tx2, rx);

}
void SerialWombatServo::attach(uint8_t pin, uint16_t min, uint16_t max)
{
	_pin = pin;
	_min = min;
	_max = max;
	initializeServo();
}

void SerialWombatServo::attach(uint8_t pin, uint16_t min, uint16_t max, bool reverse)
{
	_pin = pin;
	_min = min;
	_max = max;
	_reverse = reverse;
	initializeServo();
}
void SerialWombatServo::write(uint8_t angle)
{
	if (angle < 180)
	{
		write16bit((uint16_t)(65536uL * angle / 180));
	}
	else
	{
		write16bit(65535uL);
	}

}
uint8_t SerialWombatServo::read(void)
{
	uint32_t returnval = _position;
	returnval *= 180;
	return (uint8_t)(returnval >> 16);
}


void SerialWombatServo::write16bit(uint16_t position)
{
	_position = position;
	_sw.writePublicData(_pin, _position);
}
SerialWombatServo::~SerialWombatServo()
{
}
