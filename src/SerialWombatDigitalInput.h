#pragma once
/*
Copyright 2026 Broadwell Consulting Inc.

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

class SerialWombatDigitalInput_18AB : public SerialWombatPin, public SerialWombatAbstractProcessedInput
{
	public:
		SerialWombatDigitalInput_18AB(SerialWombatChip& serialWombat) : SerialWombatPin(serialWombat), SerialWombatAbstractProcessedInput(serialWombat){}
		int16_t begin(uint8_t pin,  bool pullUp = false, bool pullDown = false)
		{
			_pin = pin;
			_pinMode = (uint8_t)PIN_MODE_DIGITALIO;
			return initPacketNoResponse(0,(uint8_t)2,(uint8_t)(pullUp?1:0),(uint8_t)(pullDown?1:0),(uint8_t)0);
		}

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

