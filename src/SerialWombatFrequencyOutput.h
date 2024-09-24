#pragma once
/*
Copyright 2024 Broadwell Consulting Inc.

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
/*! \file SerialWombatFrequencyOutput.h
*/


/*!
\brief A class representing a Serial Wombat Frequency Output 

An instance of this class should be declared for each pin
to be used as a Serial Wombat Frequency Output.  

Serial Wombat 18AB PWM outputs are driven either by hardware peripherals
or by a DMA based software PWM scheme.  Up to 6 hardware PWM outputs are avaialble
on Enhanced Digital Performance pins (0-4,7,9-19).  The first six Enhanced Digitial
Performance pins configured after reset will claim hardware resources.  Any additional
pins configured for Frequency output will use DMA based output.  Hardware capable pins can 
generate high resolution signals up to about 100kHz.  DMA based output is limited
to transitions every 17uS, so a 1kHz output will have about 6 bits of resolution and
a 100 Hz output will have about 9 bit resolution.  Since the DMA runs at only 57600 Hz, 
frequency accuarcy will suffer on DMA pins as frequency increases, unless the desired frequency
is an integer divisor of 57600.

For very slow signals (under 20 Hz) the low frequency option should be used.  Otherwise these signals
cannot be generated.  The low frequency option works on a mS rather than uS timer, so its resolution
becomes increasingly poor at higher frequencies, but may be useful for applications such as simulation of
speedometers or tachometers which operate at relatively low freqeuencies.

A single very high speed frequency output is available on the Serial Wombat 18AB through the
High Speed Clock Output function
*/

class SerialWombatFrequencyOutput : public SerialWombatPin
{
public:
	/*!
    \brief Constructor for SerialWombatFrequencyOutput class
    \param serialWombat SerialWombat  chip on which the FrequencyOutput will run
    */
    SerialWombatFrequencyOutput(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat) {}

	/*!
    \brief Initialize a pin that has been declared as FrequencyOutput. 
   \param pin Pin to use for Frequency Output.  Use an enhanced Digital Capability pin for high frequencies
   \param maxFrequency_Hz  Maximum frequency to be output (used to pick best timing hardware)
   \param lowFrequency  Set to true if signals slower than 20Hz need to be generated.  Has negative effect on higher frequencies
   \param dutyCycle  Duty cycle of frequency to be output
   */
    int16_t begin(uint8_t pin, uint16_t maxFrequency_Hz = 65535, bool lowFrequency = false,  uint16_t dutyCycle = 0x8000)
	{
		_pin = pin;
		_pinMode = (uint8_t)PIN_MODE_FREQUENCY_OUTPUT;
		return initPacketNoResponse(0, SW_LE16(dutyCycle), SW_LE16( maxFrequency_Hz),(uint8_t)lowFrequency);
	}


private:
};



/// \brief Extends the SerialWombatFrequencyOutput class with SW18AB specific functionality, including SerialWombatAbstractScaledOutput
class SerialWombatFrequencyOutput_18AB: public SerialWombatFrequencyOutput, public SerialWombatAbstractScaledOutput
{
public:
    SerialWombatFrequencyOutput_18AB(SerialWombatChip& serialWombat) :SerialWombatFrequencyOutput(serialWombat), SerialWombatAbstractScaledOutput(serialWombat)
    {}


    /*!
    \brief fulfills a virtual function requirement of SerialWombatAbstractScaledOutput
    \return current pin number
    */
    uint8_t pin()
{
	return SerialWombatPin::_pin;
}
    /*!
    \brief fulfills a virtual function requirement of SerialWombatAbstractScaledOutput
    \return current pin mode number
    */
    uint8_t swPinModeNumber()
	{
		return SerialWombatPin::_pinMode;
	}
};

