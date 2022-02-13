#pragma once
/*
Copyright 2021 Broadwell Consulting Inc.

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

#include "SerialWombat.h"
#include "limits.h"

/*! \file SerialWombatAbstractProcessedInput.h
*/


/// \brief SerialWombatAnalogInput, SerialWombatPulseTimer, SerialWombatResistanceInput and others inherit from this class
///
class SerialWombatAbstractProcessedInput 
{
public:
	/// \brief Constructor for the SerialWombatAbstractScaledOutput Class
	/// 
	/// \param sw A reference to a previously declared SerialWombatPin to which the output is connected.
	SerialWombatAbstractProcessedInput(SerialWombatChip& sw);


	enum  Frequency {
		FREQ_1Hz = 0,
		FREQ_2Hz = 1,
		FREQ_4Hz = 2,
		FREQ_8Hz = 3,
		FREQ_16Hz = 4,
		FREQ_32Hz = 5,
		FREQ_64Hz = 6,
		FREQ_128Hz = 7,
		FREQ_256Hz = 8,
		FREQ_512Hz = 9,
		FREQ_1024Hz = 10

	};

	enum OutputValue {
		RAW = 0,
		AVERAGE = 1,
		FIRST_ORDER_FILTERED = 2,
	};

	enum Transform {
		NONE = 0,
		SCALE_RANGE = 1,
		LINEAR_MXB = 2
	};
	int16_t writeInverted(bool inverted);
	int16_t writeFirstOrderFilteringConstant(uint16_t constant);
	int16_t writeAveragingNumberOfSamples(uint16_t numberOfSamples);
	int16_t writeExcludeBelowAbove(uint16_t low, uint16_t high);
	int16_t configureQueue(SerialWombatQueue* queue, Frequency frequency, bool queueHighByte = true, bool queueLowByte = true);
	int16_t configureOutputValue(OutputValue outputValue);
	int16_t configureTransformScaleRange(uint16_t min, uint16_t max);
	int16_t configureTransformLinearMXB(int32_t m, int32_t b);
	int16_t writeProcessedInputEnable(bool enabled);
	uint16_t readMinimum(bool reset = false);
	uint16_t readMaximum(bool reset = false);
	uint16_t readAverage();
	uint16_t readFiltered();
	virtual uint8_t pin() = 0;
	virtual uint8_t swPinModeNumber() = 0;

private:
	SerialWombatChip& _pisw;
};


