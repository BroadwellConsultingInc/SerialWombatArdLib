#include "SerialWombatPulseTimer.h"

SerialWombatPulseTimer::SerialWombatPulseTimer(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat)
{
}



void SerialWombatPulseTimer::begin(uint8_t pin, SerialWombatPulseTimerUnits units, bool pullUpsEnabled)
{
	_pin = pin;
	_pinMode = PIN_MODE_PULSETIMER;
	uint8_t tx[] = { 200,_pin,_pinMode,pullUpsEnabled,(uint8_t)units,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
}

void SerialWombatPulseTimer::refresh()
{
	refreshHighCountsLowCounts();
	{
		uint8_t tx[] = { 202,_pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);
		Pulses = rx[5] + 256 * rx[6];
		MeasurementOverflowOccurred = rx[7];
	}
}

void SerialWombatPulseTimer::refreshHighCountsLowCounts()
{
	uint8_t tx[] = { 201,_pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	HighCounts = rx[3] + 256 * rx[4];
	LowCounts = rx[5] + 256 * rx[6];
}

void SerialWombatPulseTimer::refreshHighCountsPulses()
{
	uint8_t tx[] = { 202,_pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	HighCounts = rx[3] + 256 * rx[4];
	Pulses = rx[5] + 256 * rx[6];
	MeasurementOverflowOccurred = rx[7];
}

uint16_t SerialWombatPulseTimer::readHighCounts()
{
	refreshHighCountsLowCounts();
	return (HighCounts);
}

uint16_t SerialWombatPulseTimer::readLowCounts()
{
	refreshHighCountsLowCounts();
	return (LowCounts);
}

uint16_t SerialWombatPulseTimer::readPulses()
{
	refreshHighCountsPulses();
	return(Pulses);
}


int16_t SerialWombatPulseTimer_18AB::configurePublicDataOutput(SerialWombatPulseTimer_18AB::publicDataOutput publicDataOutput)
{
	uint8_t tx[] = { 203,_pin,_pinMode,(uint8_t) publicDataOutput};
	return _sw.sendPacket(tx);
}

SerialWombatPulseTimer_18AB::SerialWombatPulseTimer_18AB(SerialWombatChip& serialWombat):SerialWombatPulseTimer(serialWombat), SerialWombatAbstractProcessedInput(serialWombat)
{
}
