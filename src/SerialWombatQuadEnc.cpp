#include "SerialWombatQuadEnc.h"

SerialWombatQuadEnc::SerialWombatQuadEnc(SerialWombat& serialWombat) :_sw(serialWombat)
{
	_sw = serialWombat;
	_pin = 255;
	_secondPin = 255;
}

void SerialWombatQuadEnc::begin(uint8_t pin, uint8_t secondPin)
{
	begin(pin,secondPin,10, true, QE_ONBOTH_POLL);
}

void SerialWombatQuadEnc::begin(uint8_t pin, uint8_t secondPin, uint16_t debounce_mS, bool pullUpsEnabled)
{
	begin(pin,secondPin, debounce_mS, pullUpsEnabled, QE_ONBOTH_POLL);
}

void SerialWombatQuadEnc::begin(uint8_t pin, uint8_t secondPin, uint16_t debounce_mS, bool pullUpsEnabled, QE_READ_MODE_t readState)
{
	_pin = pin;
	_secondPin = secondPin;
	uint8_t tx[] = { 200,_pin,PIN_MODE_QUADRATUREENCODER,SW_LE16(debounce_mS), _secondPin,(uint8_t)readState,pullUpsEnabled };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
}

uint16_t SerialWombatQuadEnc::read()
{
	return _sw.readPublicData(_pin);
}

uint16_t SerialWombatQuadEnc::read(uint16_t replacementValue)
{
	return _sw.writePublicData(_pin, replacementValue);
}

void SerialWombatQuadEnc::write(uint16_t value)
{
	_sw.writePublicData(_pin, value);
}
