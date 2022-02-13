#pragma once
/*
Copyright 2020-2021 Broadwell Consulting Inc.

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
typedef enum
{
	SW_PULSETIMER_uS = 0,
	SW_PULSETIMER_mS = 1
} SerialWombatPulseTimerUnits;

/*! \brief TODO 

This class is used to measure the length of pulses in mS or in uS.  On the Serial Wombat 4A/B the 
precision is about 8uS in uS mode, with an accuracy of +/- 2% because of variation in the 
Serial Wombat chip's internal oscillator.

One SerialWombatPulseTimer should be declared per Serial Wombat pin in this mode.

A video Tutorial on this pin mode is available:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/YtQWUub9gYw" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/YtQWUub9gYw

This pinmode is useful for measuring RC Servo outputs, PWM frequency and duty cycle, or any other signal where the pulse width
is relevant.  It is designed to measure repetitive signals.  It is not intended to read a number of differing independent pulse
widths that come one after another, such as WS2812 or similar output.

\warning Care must be taken when using this pinmode with high frequency ( > 5 kHz) signals or pins that may be left floating on the Serial Wombat 4A / 4B because
the Serial Wombat 4A and 4B chip uses an interrupt internally to capture transition timestamps.  Excessively frequent pin transitions may cause the interrupt
handler to starve the main processing loop, impacting function of all pin modes and communications.

The Serial Wombat chip can be queried for overflow frames.  If overflow frames are occuring, then the system is overloaded.

The Serial Wombat 4A and 4B chips can measure a maxium of 8 transitions per mS across all pulse input pins.  More frequent transitions than
this may result in incorrect measurements.  The MeasurementOverflowOccurred flag is set if at any time more than 8 transitions occured
without processing.

The Serial Wombat 18AB chip uses a DMA based sampling scheme with samples taken at 57600 Hz.  It has lower
resolution than the Serial Wombat 4A/B (17 uS resolution), but can measure up to 18 simultaneous pulse trains and
is not subject to being overloaded by faster signals.

The number of pulses over time can be used for rough frequency measurements.

Consecutive High and low periods can be used to calculate duty cycle, although there is no indication whether high or low was
most recent.

*/

class SerialWombatPulseOnChange : public SerialWombatPin
{
public:
	/// \brief Class constructor for SerialWombatPulseOnChange
	/// \param serialWombat The Serial Wombat chip on which the SerialWombatPulseTimer pinmode will be run
	SerialWombatPulseOnChange(SerialWombatChip& serialWombat);

	
	/// \brief Initialization routine for SerialWombatPulseOnChange
	/// 
	/// \param pin The Serial Wombat pin that will pulse on change
	/// \param pulseOnTime The Number of mS that the pulse will last when the true condition occurs.  Set to 65535 to stay on constantly until condition is false
	/// \param pulseOffTime the number of mS that the pulse will stay off until another pulse can be triggered.
	/// \param orNotAnd 0 = all initialized entries must be true to generate pulse.  1 = Any intitialized entry will generate a pulse if true
	/// \param PWMperiod the period in uS of ouput pulse.  Set to 0 for constant on.
	/// \param PWMdutyCycle for output pulse.  Only valid if PWM Period is not 0.
	void begin(uint8_t pin, uint16_t pulseOnTime = 50, uint16_t pulseOffTime = 50, uint8_t orNotAnd = 1, uint16_t PWMperiod = 0, uint16_t PWMdutyCycle = 0x8000);

	void setEntryOnChange(uint8_t entryID, uint8_t sourcePin);
	void setEntryOnIncrease(uint8_t entryID, uint8_t sourcePin);
	void setEntryOnDecrease(uint8_t entryID, uint8_t sourcePin);
	void setEntryOnEqualValue(uint8_t entryID, uint8_t sourcePin, uint16_t value);
	void setEntryOnBelowValue(uint8_t entryID, uint8_t sourcePin, uint16_t value);
	void setEntryOnAboveValue(uint8_t entryID, uint8_t sourcePin, uint16_t value);
	void setEntryOnNotEqualValue(uint8_t entryID, uint8_t sourcePin, uint16_t value);
	void setEntryOnPinsEqual(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin );
	void setEntryOnPinsNotEqual(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin );
	void setEntryOnPinGTPin(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin );
	void setEntryOnPinLTPin(uint8_t entryID, uint8_t sourcePin, uint8_t secondPin );
	void setEntryOnPinCrossValue(uint8_t entryID, uint8_t sourcePin, uint16_t value); 
	void setEntryOnPinCrossAscending(uint8_t entryID, uint8_t sourcePin, uint16_t value); 
	void setEntryOnPinCrossDescending(uint8_t entryID, uint8_t sourcePin uint16_t value);

private:

};

