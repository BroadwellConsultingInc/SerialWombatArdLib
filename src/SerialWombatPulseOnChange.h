#pragma once
/*
Copyright 2022-2024 Broadwell Consulting Inc.

"Serial Wombat" is a registered trademark of Broadwell Consulting Inc. in
the United States.  See SerialWombat.com for usage guidance.

Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdint.h>
#include "SerialWombat.h"

class SerialWombat;

/*! \file SerialWombatPulseOnChange.h
*/

/*! @brief Monitors other pin(s) or public data in the Serial Wombat chip and generates a pin pulse on change

This class controls the Pulse on Change pin mode of the Serial Wombat Chip.  Pulse on Change is useful to generate
a pulse that can drive an interrupt on the host Arduino, or for creating user alerts such as LED pulses or buzzer tones
that acknowledge data reception or human input.

For instance, a pulse could be generated when the output value of a rotary encoder state machine on other pins changes value.
Or an LED could be pulsed to command traffic on the Serial Wombat chip by making it pulse when SW_DATA_SOURCE_PACKETS_RECEIVED 
changes.

Reference the Public Data and Pin Modes section on the main page to learn more about public data and how public data on the 
Serial Wombat chip works.

The each pin can monitor up to 8 other pins or data sources for changes.  A pulse will be generated when any of the criteria (orNotAnd == 1) or
all (simultaneously) of the criteria (orNotAnd == 0)  are met.  Criteria can be any of the following:

- Pin data or public data changed 
- Pin data or public data increased 
- Pin data or public data decreased 
- Pin data or public data equals a fixed value 
- Pin data or public data below a fixed value 
- Pin data or public data above a fixed value 
- Pin data or public data not equal to a fixed value 
- Pin data or public data crosses a fixed value
- Pin data or public data crosses (ascending) a fixed value
- Pin data or public data crosses (descending) a fixed value
- Pin data or public data equals another pin
- Pin data or public data below another pin
- Pin data or public data above another pin
- Pin data or public data not equal to another pin
- Pin data or public data is within a range
- Pin data or public data is outside a range


A video Tutorial on this pin mode is available:

@htmlonly
TODO
@endhtmlonly


TODO https://youtu.be/


*/

class SerialWombatPulseOnChange : public SerialWombatPin
{
public:
	/*!
	@brief Class constructor for SerialWombatPulseOnChange
	@param serialWombat The Serial Wombat chip on which the SerialWombatPulseTimer pinmode will be run
	*/
	SerialWombatPulseOnChange(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat)
	{
		_pinMode = PIN_MODE_PULSE_ON_CHANGE;
	}


	/*!
	@brief Initialization routine for SerialWombatPulseOnChange
	
	@param pin The Serial Wombat pin that will pulse on change
	@param pulseOnTime The Number of mS that the pulse will last when the true condition occurs.  Set to 65535 to stay on constantly until condition is false
	@param pulseOffTime the number of mS that the pulse will stay off until another pulse can be triggered.
	@param orNotAnd 0 = all initialized entries must be true to generate pulse.  1 = Any intitialized entry will generate a pulse if true
	@param PWMperiod the period in uS of ouput pulse.  Set to 0 for constant Active.
	@param PWMdutyCycle for output pulse.  Only valid if PWM Period is not 0.
	*/
	int16_t begin(uint8_t pin, SerialWombatPinState_t activeMode = SW_HIGH, SerialWombatPinState_t inactiveMode = SW_LOW, uint16_t pulseOnTime = 50, uint16_t pulseOffTime = 50, uint8_t orNotAnd = 1, uint16_t PWMperiod = 0, uint16_t PWMdutyCycle = 0x8000)
{
	int16_t result;
	_pin = pin;

	result = initPacketNoResponse(0,(uint8_t)activeMode,(uint8_t)inactiveMode, orNotAnd); if (result < 0) { return result; }
	result = initPacketNoResponse(1,pulseOnTime,pulseOffTime); if (result < 0) { return result; }
	return initPacketNoResponse(2,PWMperiod,PWMdutyCycle); 
}

	/*!
	@brief Configure a change entry to pulse when a pin or public data changes
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnChange(uint8_t entryID, uint8_t sourcePin)
{
	return setEntryMode(entryID, sourcePin, 0);
}
	/*!
	@brief Configure a change entry to pulse when a pin or public data increases
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnIncrease(uint8_t entryID, uint8_t sourcePin)
{
	return setEntryMode(entryID, sourcePin, 1);
}
	/*!
	@brief Configure a change entry to pulse when a pin or public data Decreases
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnDecrease(uint8_t entryID, uint8_t sourcePin)
{
	return setEntryMode(entryID, sourcePin, 2);
}
	/*!
	@brief Configure a change entry to pulse when a pin or public data equals a specified value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param value A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnEqualValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
{
	int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
	return setEntryMode(entryID, sourcePin, 3);
}
	/*!
	@brief Configure a change entry to pulse when a pin or public data is below a specified value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param value A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnLessThanValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
	{
		int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
		return setEntryMode(entryID, sourcePin, 4);
	}
	/*!
	@brief Configure a change entry to pulse when a pin or public data is above a specified value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param value A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnGreaterThanValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
	{
		int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
		return setEntryMode(entryID, sourcePin, 5);
	}


	/*!
	@brief Configure a change entry to pulse when a pin or public data is not equal to a specified value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param value A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnNotEqualValue(uint8_t entryID, uint8_t sourcePin, uint16_t value)
	{
		int16_t result = setEntryParams(entryID, value, 0);  if (result < 0) return result;
		return setEntryMode(entryID, sourcePin, 6);
	}
	/*!
	@brief Configure a change entry to pulse when a pin or public data equals a second pin or public data's value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param secondPin Pin or public data whose data will be compared to the public data from sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinsEqual(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin )
	{
		int16_t result = setEntryParams(entryID, secondPin, 0);  if (result < 0) return result;
		return setEntryMode(entryID, sourcePin, 7);
	}

	/*!
	@brief Configure a change entry to pulse when a pin or public data is not equal to a second pin or public data's value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param secondPin Pin or public data whose data will be compared to the public data from sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinsNotEqual(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin )
	{
		int16_t result = setEntryParams(entryID, secondPin, 0);  if (result < 0) return result;
		return setEntryMode(entryID, sourcePin, 10);
	}

	/*!
	@brief Configure a change entry to pulse when a pin or public data is greater than a second pin or public data's value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param secondPin Pin or public data whose data will be compared to the public data from sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinGTPin(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin );

	/*!
	@brief Configure a change entry to pulse when a pin or public data is less than a second pin or public data's value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param secondPin Pin or public data whose data will be compared to the public data from sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinLTPin(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin );
	/*!
	@brief Configure a change entry to pulse when a pin or public data crosses a specified value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param value A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinCrossValue(uint8_t entryID, uint8_t sourcePin, uint16_t value); 

	/*!
	@brief Configure a change entry to pulse when a pin or public data rises above a specified value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param value A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinCrossAscending(uint8_t entryID, uint8_t sourcePin, uint16_t value); 

	/*!
	@brief Configure a change entry to pulse when a pin or public data falls below a specified value
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param value A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinCrossDescending(uint8_t entryID, uint8_t sourcePin, uint16_t value);

	/*!
	@brief Configure a change entry to pulse when a pin or public data is within a specified range
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param lowValue A constant value to be compared against the public data specified by sourcePin
	@param highValue A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinWithinRange(uint8_t entryID, uint8_t sourcePin, uint16_t lowValue, uint16_t highValue); 

	/*!
	@brief Configure a change entry to pulse when a pin or public data is within a specified range
	
	@param entryID to set one entry in this pin's change entry table to OnChange.  Valid values are 0-7
	@param sourcePin Pin or data source to monitor for change.  Enumerated types should be casted to uint8_t
	@param lowValue A constant value to be compared against the public data specified by sourcePin
	@param highValue A constant value to be compared against the public data specified by sourcePin
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setEntryOnPinOutsideRange(uint8_t entryID, uint8_t sourcePin, uint16_t lowValue, uint16_t highValue);
private:
	int16_t setEntryParams(uint8_t entryID, uint16_t firstParam, uint16_t secondParam)
	{
		return initPacketNoResponse(3, entryID, SW_LE16(firstParam), SW_LE16(secondParam));
	}
	int16_t setEntryMode(uint8_t entryID, uint8_t pin, uint8_t mode)
	{
		return initPacketNoResponse(4, entryID, mode,pin);
	}
};

