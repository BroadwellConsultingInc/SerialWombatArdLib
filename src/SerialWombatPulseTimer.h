#pragma once
#include <stdint.h>
#include "SerialWombat.h"

class SerialWombat;

typedef enum
{
	SW_PULSETIMER_uS = 0,
	SW_PULSETIMER_mS = 1
} SerialWombatPulseTimerUnits;
class SerialWombatPulseTimer
{
public:
	SerialWombatPulseTimer(SerialWombat& serialWombat);

	void begin(uint8_t pin);
	void begin(uint8_t pin, SerialWombatPulseTimerUnits units, bool pullUpsEnabled);
	void refresh();
	void refreshHighCountsLowCounts();
	void refreshHighCountsPulses();
	uint16_t readHighCounts();
	uint16_t readLowCounts();
	uint16_t readPulses();

	uint16_t HighCounts = 0;
	uint16_t LowCounts = 0;
	uint16_t Pulses = 0;
	bool MeasurementOverflowOccurred = false;

private:
	SerialWombat& _sw;
	uint8_t _pin;
};


