#pragma once
/*
Copyright 2025 Broadwell Consulting Inc.

"Serial Wombat" is a registered trademark of Broadwell Consulting Inc. in
the United States.  See SerialWombat.com for usage guidance.

Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
*/
/*! 
@file PCB0030_Bridge.h
*/
#include "SerialWombat.h"

/*!

@brief A class representing a PCB0030 Bridge PCB

The PCB0030_Bridge class is a child class of SerialWombatChip.  It wraps two H Bridge pin modes and 
provides methods for using the board feature for measuring source voltage.

Pins 0-3 are accessed like any othe Serial Wombat Pin.  The Brushed Motor image loaded on this board at manufacturing
supports the following
pin modes:
* Digital I/O
* Analog Input
* Quadrature Encoder Input
* H Bridge Output
* Debounced Input
* PWM Output
* Pulse Timer



A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/

class PCB0030_Bridge : public SerialWombatChip
{
public:
	PCB0030_Bridge() : SerialWombatChip(), hBridge45(*this), hBridge67(*this), powerVoltage(*this)
	{}
	
	SerialWombatHBridge hBridge45, hBridge67;
	SerialWombatAnalogInput_18AB powerVoltage;

	


	/*!	
	@brief Initialize a PCB0030_Bridge pcb on a given I2C address
	
	Wire must be initialized before calling this function	
	
	@param i2cAddress The I2C address of the PCB
	@param pin3IsVoltage Set this to true if the voltage measurement solder jumper has been soldered closed
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/
	int16_t begin(uint8_t i2cAddress,bool pin3IsVoltage = false, uint16_t pwmFrequency = 1000)
	{
		SerialWombatChip::begin(Wire, i2cAddress, true);
		
		hBridge45.begin(4, 5, pwmFrequency,HBRIDGE_OFF_BOTH_HIGH); // Short Brake mode
		hBridge67.begin(6, 7, pwmFrequency,HBRIDGE_OFF_BOTH_HIGH); // Short Brake mode
		if (pin3IsVoltage)
		{
			powerVoltage.begin(3);
			pin3VoltageEnable = true;
		}
		return (0);
	}


	/*!	
	@brief Returns the voltage provided to the Bridge in mV
	
	begin must be called with pin3VoltageEnable = true  before using this method

	this function calculates the value based on the 8200/2000 ohm voltage divider on the board

	The solder jumper must be closed to connect the voltage divider to pin 3
	
	@return  Returns the bridge source voltage in millivolts
	*/
	uint16_t readPowerVoltage_mv()
	{
		if (pin3VoltageEnable)
		{
			uint32_t v = powerVoltage.readAveraged_mV();
			v *= (8200 + 2000);  // Board has an 8200 / 2000 ohm voltage divider
			v /= 2000;
			return ((uint16_t)v);
		}
		return (0);
			
	}

private:
	bool pin3VoltageEnable = false;
};

