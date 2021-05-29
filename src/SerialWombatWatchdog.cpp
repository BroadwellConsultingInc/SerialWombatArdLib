#include "SerialWombatWatchdog.h"

SerialWombatWatchdog::SerialWombatWatchdog(SerialWombat& sw):_sw(sw)
{
	_sw = sw;
}

void SerialWombatWatchdog::begin(uint8_t pin, SerialWombatPinState_t normalState, SerialWombatPinState_t resetState, uint16_t timeout_mS, bool ResetWombatAfterTimeout)
{
	_pin = pin;
	uint8_t tx[8] = {200,pin,7, normalState,resetState,SW_LE16(timeout_mS),ResetWombatAfterTimeout };

	_sw.sendPacket(tx);

	uint8_t tx1[8] = { 201,pin,7,0,0,0,0,0x55 };
	if (! ResetWombatAfterTimeout)
	{
		tx1[5] = 0xFF; // No return to normal
		tx1[6] = 0xFF;
	}

	_sw.sendPacket(tx1);
}

void SerialWombatWatchdog::updateResetCountdown(uint16_t time_mS)
{
	_sw.writePublicData(_pin, time_mS);
}
