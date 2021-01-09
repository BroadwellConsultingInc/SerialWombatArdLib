#include "SerialWombatAnalogInput.h"


SerialWombatAnalogInput::SerialWombatAnalogInput( SerialWombat& serialWombat):_sw(serialWombat)
{
	_sw = serialWombat;
}

void SerialWombatAnalogInput::begin(uint8_t pin)
{
	_pin = pin;
	uint8_t tx[] = { 200,_pin,PIN_MODE_ANALOGINPUT,0,0,0,0,0 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
}

void SerialWombatAnalogInput::begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant)
{
	begin(pin);
	uint8_t tx[] = { 201,_pin,PIN_MODE_ANALOGINPUT,SW_LE16(averageSamples) ,SW_LE16(filterConstant),0 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
}


int16_t SerialWombatAnalogInput::readVoltage_mV()
{
	
	uint32_t x = _sw.readPublicData(_pin); // Counts ranging from 0 to 65535

	if (supplyVoltagemV == 0)
	{
		supplyVoltagemV = _sw.readSupplyVoltage_mV();
	}
	x *= supplyVoltagemV;
	return ((int16_t) (x >> 16));
}

uint16_t SerialWombatAnalogInput::readCounts()
{
	return (_sw.readPublicData(_pin));
}





SerialWombatAnalogInput::~SerialWombatAnalogInput()
{
}
