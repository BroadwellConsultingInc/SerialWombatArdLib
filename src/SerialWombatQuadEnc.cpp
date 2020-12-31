#include "serialWombatQuadEnc.h"

SerialWombatQuadEnc::SerialWombatQuadEnc(SerialWombat& serialWombat, uint8_t pin, uint8_t secondPin) :_sw(serialWombat)
{
	_sw = serialWombat;
	_pin = pin;
	_secondPin = secondPin;
}

void SerialWombatQuadEnc::begin()
{
	begin(20, true, false);
}

void SerialWombatQuadEnc::begin(uint16_t debounce_mS, bool pullUpsEnabled)
{
	begin(debounce_mS, pullUpsEnabled, false);
}

void SerialWombatQuadEnc::begin(uint16_t debounce_mS, bool pullUpsEnabled, uint8_t readState)
{
	uint8_t tx[] = { 200,_pin,PIN_MODE_QUADRATUREENCODER,SW_LE16(debounce_mS), _secondPin,readState,pullUpsEnabled };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
}

uint16_t SerialWombatQuadEnc::readPosition()
{
	return _sw.readPublicData(_pin);
}
