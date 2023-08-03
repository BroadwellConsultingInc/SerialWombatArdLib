#pragma once
/*
Copyright 2023 Broadwell Consulting Inc.

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

/*! \file serialWombatHSCounter.h
*/


/*! \brief

This class is used to measure the frequency or cycles of a high speed input.  On the Serial Wombat 18AB
chip this class can be used two times, as two clock inputs are avaialble.  An enhanced digital capability
pin must be used.

This pin mode has been tested on inputs up to 4MHz on the 18AB.

For frequency measurements a number of counts is divided by a time.  The time in mS can be specified.
The frequency is updated every X ms.  In order to get a good value, X should be an even divisor of 1000.

The counter can be retreived and optionally be reset on reading.

The public data buffer for this pin mode can be based either on the count of cycles or the frequency.

Since the public data buffer is limited to 16 bits a divisor is available that's applied to the
counter or frequency before it's copied to the public data buffer.  That way a varying high speed
frequency can still create a varying public data buffer rather than saturating at 65535.

A video Tutorial on this pin mode may be available in the future:

\htmlonly
TODO - Video coming Soon
\endhtmlonly
//TODO - Video coming soon
*/

class SerialWombatHSCounter : public SerialWombatPin, public SerialWombatAbstractProcessedInput
{
public:
	enum publicDataOutput {
		PULSE_COUNT = 2, ///< The number of pulses that have occured since initialization. 
		FREQUENCY_ON_LTH_TRANSITION = 5, ///< The frequency of the pulse in Hz
	};

	/// \brief Class constructor for SerialWombatHSCounter
	/// \param serialWombat The Serial Wombat chip on which the SerialWombatHSCounter pinmode will be run
	SerialWombatHSCounter(SerialWombatChip& serialWombat);

	
	/// \brief Initialization routine for SerialWombatHSCounter
	/// 
	/// \param pin 
	/// \param 
	/// \param 
	int16_t begin(uint8_t pin, SerialWombatHSCounter::publicDataOutput publicDataOutput = publicDataOutput::FREQUENCY_ON_LTH_TRANSITION ,uint16_t framesBetweenUpdates = 100, uint16_t publicOutputDivisor = 1);

	uint32_t readCounts(bool resetCounts = false);

	int32_t readFrequency();
	/// \brief Disables the high speed clock output
	int16_t disable ();

	uint8_t pin() { return _pin; }
	uint8_t swPinModeNumber() { return _pinMode; }

private:

};
