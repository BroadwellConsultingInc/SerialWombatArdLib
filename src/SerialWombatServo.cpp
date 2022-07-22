#include "SerialWombatServo.h"
#include <stdint.h>


SerialWombatServo::SerialWombatServo(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat)
{	
	_pinMode = PIN_MODE_SERVO;
}



void SerialWombatServo::attach(uint8_t pin, bool reverse)
{
	_pin = pin;
	_pinMode = PIN_MODE_SERVO;
	_reverse = reverse;
	initializeServo();
}

void SerialWombatServo::initializeServo()
{
	uint8_t tx[] = { 200,_pin,_pinMode,_pin,SW_LE16(_position), _reverse,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	uint8_t tx2[] = { 201,_pin,_pinMode,SW_LE16(_min), SW_LE16(_max-_min),0x55, 0x55 };
	_sw.sendPacket(tx2, rx);

}

void SerialWombatServo::attach(uint8_t pin, uint16_t min, uint16_t max, bool reverse)
{
	_pin = pin;
	_min = min;
	_max = max;
	_reverse = reverse;
	_pinMode = PIN_MODE_SERVO;
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

SerialWombatServo_18AB::SerialWombatServo_18AB(SerialWombatChip& serialWombat):SerialWombatAbstractScaledOutput(serialWombat),SerialWombatServo(serialWombat)
{
}

uint8_t SerialWombatServo_18AB::pin()
{

	return SerialWombatPin::_pin;
}

uint8_t SerialWombatServo_18AB::swPinModeNumber()
{
	return SerialWombatPin::_pinMode;
}

SerialWombatHighFrequencyServo::SerialWombatHighFrequencyServo(SerialWombatChip& serialWombat) :SerialWombatServo_18AB(serialWombat)
{
}

void SerialWombatHighFrequencyServo::attach(uint8_t pin, uint16_t min, uint16_t max, bool reverse)
{
	_pin = pin;
	_min = min;
	_max = max;
	_reverse = reverse;
	_pinMode = PIN_MODE_HS_SERVO;
	initializeServo();
}

int16_t SerialWombatHighFrequencyServo::writeFrequency_Hz(uint16_t frequency)
{
	return writePeriod_uS((uint16_t)(1000000ul / frequency));
}

int16_t SerialWombatHighFrequencyServo::writePeriod_uS(uint16_t period)
{
	uint8_t tx[] = { 203,_pin,_pinMode,SW_LE16(period), 0x55,0x55,0x55 };
	uint8_t rx[8];
	return _sw.sendPacket(tx, rx);
}
