#pragma once
#include "SerialWombat.h"



class SerialWombatWatchdog
{
public:
	SerialWombatWatchdog(SerialWombat& sw);
	void begin(uint8_t pin, SerialWombatPinState_t normalState, SerialWombatPinState_t resetState, uint16_t timeout_mS, bool ResetWombatAfterTimeout);
	void setResetStateTime_ms(uint16_t resetStateTime);
	void setSafePinStates(SerialWombatPinState_t pin0, SerialWombatPinState_t pin1, SerialWombatPinState_t pin2, SerialWombatPinState_t pin3);
	void updateResetCountdown(uint16_t time_mS);
private:
	SerialWombat& _sw;
	uint8_t _pin;
	uint16_t _resetStateTime = 10;

};

