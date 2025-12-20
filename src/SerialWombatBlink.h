#pragma once
/*
Copyright 2025 Broadwell Consulting Inc.

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
/*! \file SerialWombatBlink.h
*/


/*!
@brief A class representing a Serial Wombat Blink Output

An instance of this class should be declared for each pin
to be used as a Serial Wombat Blink Output.  

This is a very simple pin mode that sets an output high for at least 30mS each time
a public data value changes.  Otherwise the pin is digital low.  
A constantly changing output can cause this pin to remain high (the 30mS  timer is
reset to 30 each time the public data input changes.  

The public data of this pin indicates how many mS of on time remain.

This pin mode exists to provide a blink option using very little microcontroller flash 
space, so it can be included with all SW8B builds.  See the SerialWombatPulseOnChange
pin mode for a much more powerful and configurable version.
*/

class SerialWombatBlink : public SerialWombatPin
{
public:
	/*!
    @brief Constructor for SerialWombatBlink class
    @param serialWombat SerialWombat  chip on which the Blink output will run
    */
    SerialWombatBlink(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat) {}

	/*!
    @brief Initialize a pin that has been declared as Blink. 
    @param pin  Pin to initialize to Blink mode.
   @param publicDataSource Pin that will cause a blink if it changes.
      @returns 0 if successful, non-zero otherwise.
   */
    int16_t begin(uint8_t pin, uint8_t publicDataSource)
	{
		_pin = pin;
		_pinMode = (uint8_t)PIN_MODE_BLINK;
		return initPacketNoResponse(0, publicDataSource);
	}

    /*!
    @brief Initialize a pin that has been declared as Blink. 
    @param pin  Pin to initialize to Blink mode.
   @param publicDataSource Pin that will cause a blink if it changes.
   @returns 0 if successful, non-zero otherwise.
   */
    int16_t begin(uint8_t pin, SerialWombatDataSource publicDataSource)
	{
		_pin = pin;
		_pinMode = (uint8_t)PIN_MODE_BLINK;
		return initPacketNoResponse(0, (uint8_t)publicDataSource);
	}


private:
};


