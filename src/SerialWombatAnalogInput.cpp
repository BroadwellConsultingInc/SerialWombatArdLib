#include "SerialWombatAnalogInput.h"


SerialWombatAnalogInput::SerialWombatAnalogInput( SerialWombatChip& serialWombat):SerialWombatPin(serialWombat)
{

}


int16_t SerialWombatAnalogInput::begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant, AnalogInputPublicDataOutput output)
{
	_pin = pin;
	_pinMode = PIN_MODE_ANALOGINPUT;
	uint8_t tx[] = { 200,_pin,PIN_MODE_ANALOGINPUT,0,0,0,0,0 };
	int16_t result = _sw.sendPacket(tx);
	if (result < 0) { return result; }
	uint8_t tx1[] = { 201,_pin,PIN_MODE_ANALOGINPUT,SW_LE16(averageSamples) ,SW_LE16(filterConstant),(uint8_t)output };
	return _sw.sendPacket(tx1);
}




uint16_t SerialWombatAnalogInput::readVoltage_mV()
{
	
	uint16_t reading = _sw.readPublicData(_pin);
	
	uint32_t x = ((uint32_t) reading ) * _sw._supplyVoltagemV;
	
	uint16_t returnval = x >> 16;
	
	return (returnval);
}

uint16_t SerialWombatAnalogInput::readCounts()
{
	return (_sw.readPublicData(_pin));
}

uint16_t SerialWombatAnalogInput::readFiltered_mV()
{
	uint32_t x = readFilteredCounts(); // Counts ranging from 0 to 65535

	
	
	return ((uint16_t)(x >> 16));
}

uint16_t SerialWombatAnalogInput::readFilteredCounts()
{
	uint8_t tx[] = { 204,_pin,PIN_MODE_ANALOGINPUT,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	_sw.sendPacket(tx, rx);

	return(rx[5] + rx[6] * 256);
}

uint16_t SerialWombatAnalogInput::readAveraged_mV()
{
	uint32_t x = readAveragedCounts(); // Counts ranging from 0 to 65535

	x *= _sw._supplyVoltagemV;
	return ((uint16_t)(x >> 16));
}

uint16_t SerialWombatAnalogInput::readAveragedCounts()
{
	uint8_t tx[] = { 204,_pin,PIN_MODE_ANALOGINPUT,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	_sw.sendPacket(tx, rx);

	return(rx[3] + rx[4] * 256);
}





uint16_t SerialWombatAnalogInput::updateSupplyVoltage_mV()
{
	return  _sw.readSupplyVoltage_mV();
}

SerialWombatAnalogInput::~SerialWombatAnalogInput()
{
}


uint16_t SerialWombatAnalogInput::readMaximumCounts(bool resetAfterRead)
{
	uint8_t tx[] = { 203,_pin,PIN_MODE_ANALOGINPUT,0,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	if (resetAfterRead)
	{
		tx[3] = 1;
	}
	_sw.sendPacket(tx, rx);

	return(rx[5] + rx[6] * 256);
}

uint16_t SerialWombatAnalogInput::readMinimumCounts(bool resetAfterRead)
{
	uint8_t tx[] = { 203,_pin,PIN_MODE_ANALOGINPUT,0,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];

	if (resetAfterRead)
	{
		tx[3] = 1;
	}
	_sw.sendPacket(tx, rx);

	return(rx[3] + rx[4] * 256);
}




uint16_t SerialWombatAnalogInput::readMaximum_mV(bool resetAfterRead)
{
	uint32_t x = readMaximumCounts(resetAfterRead); // Counts ranging from 0 to 65535

	x *= _sw._supplyVoltagemV;
	return ((uint16_t)(x >> 16));
}


uint16_t SerialWombatAnalogInput::readMinimum_mV(bool resetAfterRead)
{
	uint32_t x = readMinimumCounts(resetAfterRead); // Counts ranging from 0 to 65535

	x *= _sw._supplyVoltagemV;
	return ((uint16_t)(x >> 16));
}

SerialWombatAnalogInput_18AB::SerialWombatAnalogInput_18AB(SerialWombatChip& serialWombat):SerialWombatAnalogInput(serialWombat),SerialWombatAbstractProcessedInput(serialWombat)
{
}

int16_t SerialWombatAnalogInput_18AB::setInputSource(uint8_t inputSource)
{
	uint8_t tx[] = { 202,_pin,PIN_MODE_ANALOGINPUT,inputSource,0x55,0x55,0x55,0x55 };
	return(_sw.sendPacket(tx));

}

int16_t SerialWombatAnalogInput_18AB::setQueue(uint16_t queueIndex, uint16_t msBetweenQueues)
{
	uint8_t tx[8] = { 218,_pin,PIN_MODE_ANALOGINPUT,SW_LE16(queueIndex), SW_LE16(msBetweenQueues),0x55 };
	return (_sw.sendPacket(tx));
}