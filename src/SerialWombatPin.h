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

/*!
\brief Describes a Serial Wombat Pin.  Is base class for other pin modes

This class describes a Serial Wombat Pin on a Serial Wombat Chip.  This class
can be used to control a pin as a simple Digital I/O pin.
This class is used as a base class for many other pin mode classes.

Digital I/O related members such as digitalWrite or pinMode should not be
called against derived classes as this may have unpredicable effects on
the state machines run by those derived classes.
*/
class SerialWombatPin
{
public:
/*!
	\brief Instantiates a Serial Wombat Pin 
	\param serialWombatChip The chip on which the Serial Wombat Pin exists
*/
	SerialWombatPin(SerialWombatChip& serialWombatChip): _sw(serialWombatChip)
	{
	}

/*!
	\brief Instantiates a Serial Wombat Pin 
	\param serialWombatChip The chip on which the Serial Wombat Pin exists
	\param pin The pin number of the pin (WP number, not package pin number) of the pin
*/
	SerialWombatPin(SerialWombatChip& serialWombatChip, uint8_t pin): _sw(serialWombatChip)
	{
		_pin = pin;
	}

/*!
	\brief Read the 16 Bit public data associated with this pin 
	
	Reads and returns the 16 bit value associated with this pin.
	\return 16 bit public data for this pin.
*/
	uint16_t readPublicData() 
	{
		return _sw.readPublicData(_pin);
	};


/*!
	\brief Set pin to INPUT or OUTPUT, with options for pull Ups and open Drain settings
	
	\param mode Valid values are INPUT, OUTPUT or INPUT_PULLUP as defined by arduino.  Do 
	not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	\param pullDown  If True, a weak pull down will be enabled on this pin (No effect on SW4A/SW4B)
	\param openDrain If True, output becomes openDrain output rather than push / pull
*/
	void pinMode(uint8_t mode, bool pullDown = false, bool openDrain = false)
	{
		_sw.pinMode(_pin, mode, pullDown, openDrain);
	}

/*!
	\brief Set output pin High or Low
	
	Before calling this function, the pin should be configured as an input or output with pinMode()
	\param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	\param val  Valid values are HIGH or LOW
	not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
*/
	void digitalWrite(uint8_t val)
	{
		_sw.digitalWrite(_pin, val);
	}

/*!
	\brief Reads the state of the Pin
	
	This function is based on the pin's public data, not a raw reading.
	
	\return Returns LOW if pin is low or public data is 0.  Returns HIGH if pin is high or public data is > 0
*/
	int digitalRead()
	{
		return (_sw.digitalRead(_pin));
	}


/*!
	\brief Write a 16 bit value to this pin
	\param value The 16 bit value to write
*/
	uint16_t writePublicData(uint16_t value) 
	{ return _sw.writePublicData(_pin, value); }

/*!
	\brief Returns the current SW pin number.  Used primarily for virtual calls by derived classes
	\return Returns the current SW pin number.
*/
	uint8_t pin() {return _pin;}

/*!
	\brief Returns the Mode number.  Used primarily by derived classes to populate packet data
	\return Returns the pin mode number
*/
	uint8_t swPinModeNumber() { return _pinMode; }




protected:
	uint8_t _pin = 255;
	SerialWombatChip& _sw;
	uint8_t _pinMode = 0;

};

