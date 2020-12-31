#include "SerialWombatWatchdog.h"

SerialWombatWatchdog::SerialWombatWatchdog(SerialWombat& sw):_sw(sw)
{
	_sw = sw;
}

void SerialWombatWatchdog::begin(uint8_t pin, SerialWombatPinState_t normalState, SerialWombatPinState_t resetState, uint16_t timeout_mS, bool ResetWombatAfterTimeout)
{
	_pin = pin;
	uint8_t tx[8] = {200,pin,7, normalState,resetState,SW_LE16(timeout_mS),ResetWombatAfterTimeout };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
}

void SerialWombatWatchdog::updateResetCountdown(uint16_t time_mS)
{
	_sw.writePublicData(_pin, time_mS);
}
