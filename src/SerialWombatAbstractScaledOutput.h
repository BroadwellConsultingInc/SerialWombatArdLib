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

/*! \file SerialWombatAbstractScaledOutput.h
*/


/// \brief SerialWombatServo and SerialWombatPWM inherit from this class
///
class SerialWombatAbstractScaledOutput 
{
public:
	/// \brief Constructor for the SerialWombatAbstractScaledOutput Class
	/// 
	/// \param sw A reference to a previously declared SerialWombatPin to which the output is connected.
	SerialWombatAbstractScaledOutput(SerialWombatChip& sw);

	int16_t writeTimeout(uint16_t timeout_mS, uint16_t timeoutOutputValue);
	int16_t writeScalingEnabled(bool enabled, uint8_t sourcePin);
	int16_t writeInputScaling(uint16_t inputMin, uint16_t inputMax);
	int16_t writeOutputScaling(uint16_t outputMin, uint16_t outputMax);
	int16_t writeScalingInvertedInput(bool inverted);
	int16_t writeScalingTargetValue(uint16_t target);
	int16_t configureRateControl(uint8_t sampleRate, uint16_t filterConstant);
	int16_t configure1stOrderFiltering(uint8_t sampleRate, uint16_t filterConstant);
	int16_t configureHysteresis(uint16_t lowLimit, uint16_t lowOutputValue, uint16_t highLimit, uint16_t highOutputValue, uint16_t initialOutputValue);
	int16_t configurePID(uint16_t kp, uint16_t ki, uint16_t kd,uint16_t target,uint8_t sampleRate);
	virtual uint8_t pin() = 0;
	virtual uint8_t swPinModeNumber() = 0;

private:
	SerialWombatChip& _asosw;

};


