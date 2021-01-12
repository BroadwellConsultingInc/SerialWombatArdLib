#pragma once
#include <stdint.h>
#include "SerialWombat.h"

// For a good explanation of 1st order FIR filter calculations, see: 
// https://www.monocilindro.com/2017/04/08/how-to-implement-a-1st-order-iir-filter-in-5-minutes/
// Some filter cut-off (3dB down) frequency values:
// 0.5  65417
// 1 65298
// 2 65062
// 5 64358
// 10  63202

class SerialWombat;

class SerialWombatAnalogInput
{
public:
	SerialWombatAnalogInput(SerialWombat& SerialWombat);
	void begin(uint8_t pin);
	void begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant);
	uint16_t readVoltage_mV();
	uint16_t readCounts();
	uint16_t readFiltered_mV();
	uint16_t readFilteredCounts();
	uint16_t readAveraged_mV();
	uint16_t readAveragedCounts();
	uint16_t readMinimum();
	uint16_t readMaximum();
	void resetMinMax();
	uint16_t updateSupplyVoltage_mV();
	~SerialWombatAnalogInput();

private:
	SerialWombat &_sw ;
	
	uint8_t _pin = 255;
	
	
};

