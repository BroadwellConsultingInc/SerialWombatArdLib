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
@file PCB0041_Remcon.h
*/
#include "SerialWombat.h"

/*!

@brief A class representing a PCB0041 Remcon PCB

The PCB0048_Mux class is a child class of SerialWombatChip.  It initializes IR TX and RX and optionally connects Blink to RX.
RX provides the current 

Pin 3 is IR RX from the 38kHz demodulator
Pin 6 and 7 go to an AND chip which drives an IR LED through a transistor.
Pin 1 is connected to anode of a green LED and current limiting resistor.
Pins 0,2, 4, and 5, are accessed like any othe Serial Wombat Pin.  The Default firmware image loaded on this board at manufacturing
supports the following
pin modes:
* Digital I/O
* Analog Input
* Servo Output
* PWM Output

Pin modes are enabled with default parameters.  If different parameters are desired, such as changing RX public data to button
code then begin can be called again on that pin, such as 
irrx.begin (3, SerialWombatIRRx::publicDataOutput::COMMAND);

See examples for details

Other images can be loaded via the bootloader, but no other prebuilt images include IR functionality


See full documentation for this board at 
https://serialwombat.com/p41

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/

class PCB0041_Remcon : public SerialWombatChip
{
public:
	PCB0041_Remcon() : SerialWombatChip(), irrx(*this), irtx(*this), blink(*this)
	{}
	
	SerialWombatIRRx irrx;
	SerialWombatIRTx irtx;
	SerialWombatBlink blink;

	


	/*!	
	@brief Initialize a PCB0048 Remcon pcb on a given I2C address
	
	Wire must be initialized before calling this function	
	
	@param i2cAddress The I2C address of the PCB
	@param irTxAddress The 16 bit NEC address used to transmit codes
	@param blink  Whether to enable the LED on pin 1 to blink when IR commands are received
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/
	int16_t begin(uint8_t i2cAddress, uint16_t irTxAddress = 0x1234, bool enableBlink = true )
	{
		SerialWombatChip::begin(Wire, i2cAddress, true);
		irrx.begin((uint8_t)3);
		irtx.begin((uint8_t)7, irTxAddress);
		irtx.enableSW8b38KHzWP6 ();
		if (enableBlink)
		{
			blink.begin((uint8_t)1,(uint8_t)3);
		}
		return (0);
	}

private:
};

