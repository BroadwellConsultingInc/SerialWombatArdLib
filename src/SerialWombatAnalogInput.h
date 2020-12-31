#pragma once
#include <stdint.h>
#include "SerialWombat.h"

class SerialWombat;

class SerialWombatAnalogInput
{
public:
	SerialWombatAnalogInput(SerialWombat& SerialWombat);
	void begin(uint8_t pin);
	void begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant);
	int16_t readVoltage_mV();
	uint16_t readCounts();
	int16_t readFiltered_mV();
	uint16_t readFilteredCounts();
	int16_t readAveraged_mV();
	uint16_t readAveragedCounts();
	void updateSupplyVoltage();
	~SerialWombatAnalogInput();

private:
	SerialWombat &_sw ;
	
	uint8_t _pin = 0;
	uint16_t supplyVoltagemV = 0;
	
};

