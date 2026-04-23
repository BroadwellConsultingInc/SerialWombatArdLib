#pragma once
/*
Copyright 2026 Broadwell Consulting Inc.

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
@file PCB0048_Mux.h
*/
#include "SerialWombat.h"

/*!

@brief A class representing a PCB0048 Mux PCB

The PCB0048_Mux class is a child class of SerialWombatChip.  It initializes all I2C bridge digital I/Os to disabled and provides
methods for them.

Pins 1,2,3 and 7 are used to control the 
Pins 0, 4, 5, and 6 are accessed like any othe Serial Wombat Pin.  The Default firmware image loaded on this board at manufacturing
supports the following
pin modes:
* Digital I/O
* Analog Input
* Quadrature Encoder Input
* PWM Output
* H Bridge
* Frequency Output

Other images can be loaded via the bootloader.  The pins selected for Mux control were chosen so that the hardware UART pins remain
free for use with the communications firmware build.

Each bus segment is enabled by writing 0xFFFF to the appropriate bus pin, and disabled by writing 0x0000
For example:
bus3.writePublicData(0x0000); //Disable bus segment 3
bus2.writePublicData(0xFFFF);  // Enable bus segment bus2

See full documentation for this board at 
https://serialwombat.com/p48

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/

class PCB0048_Mux : public SerialWombatChip
{
public:
	PCB0048_Mux() : SerialWombatChip(), bus1(*this), bus2(*this), bus3(*this), bus7(*this)
	{}
	
	SerialWombatDigitalOutput_18AB bus1, bus2,bus3,bus7;

	


	/*!	
	@brief Initialize a PCB0048_Mux pcb on a given I2C address
	
	Wire must be initialized before calling this function	
	
	@param i2cAddress The I2C address of the PCB
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/
	int16_t begin(uint8_t i2cAddress)
	{
		SerialWombatChip::begin(Wire, i2cAddress, true);
		bus1.begin(1);	
		bus2.begin(2);	
		bus3.begin(3);	
		bus7.begin(7);	
		return (0);
	}



private:
};

