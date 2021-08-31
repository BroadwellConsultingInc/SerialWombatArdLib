#include "SerialWombatProtectedOutput.h"

SerialWombatProtectedOutput::SerialWombatProtectedOutput(SerialWombat& serialWombat):_sw(serialWombat)
{
	_sw = serialWombat;
}

void SerialWombatProtectedOutput::begin(uint8_t pin, uint8_t monitoredPin)
{
	_pin = pin;
	_monitoredPin = monitoredPin;
}

void SerialWombatProtectedOutput::configure(PO_COMPARE_t compareMode, uint16_t compareValue, uint8_t debounceTime, SerialWombatPinState_t activeState, SerialWombatPinState_t safeState)
{
	_debounceTime = debounceTime;
	_safeState = safeState;
	_compareMode = compareMode;
	_activeState = activeState;
	{
		uint8_t tx[] = { 200,_pin,PIN_MODE_PROTECTED_OUTPUT,SW_LE16(compareValue), _debounceTime,_monitoredPin,(uint8_t)_safeState };
		_sw.sendPacket(tx);
	}
	{
		uint8_t tx1[] = { 201,_pin,PIN_MODE_PROTECTED_OUTPUT,(uint8_t) _compareMode,(uint8_t)_activeState,0x55,0x55,0x55 };
		_sw.sendPacket(tx1);
	}

}

bool SerialWombatProtectedOutput::isInSafeState()
{

	return (_sw.readPublicData(_pin) >= _debounceTime);
}

void SerialWombatProtectedOutput::makeInput()
{
	configure(PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED, 0, 100, SW_INPUT, SW_INPUT);
}

void SerialWombatProtectedOutput::digitalWrite(uint8_t state)
{
	if (state == HIGH)
	{
		configure(PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED, 0, 100, SW_HIGH, SW_HIGH);
	}
	else if (state == LOW)
	{
		
			configure(PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED, 0, 100, SW_LOW, SW_LOW);
	}
}


