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
@file PCB0042_LSD.h
*/
#include "SerialWombat.h"


/*!

@brief A class representing a PCB0042 HSD Eight Channel LSD Low Side Driver board

The PCB0042 class makes all functionality from the PCB0042 board available.  

See documentation at https://serwom.com/p42

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/

class PCB0042_LSD_PWM: public SerialWombatChip
{
public:
	PCB0042_LSD_PWM() :  SerialWombatChip(),vinMeas(*this),
		output0(*this),output1(*this),output2(*this),output3(*this),output4(*this),output5(*this),output6(*this),output7(*this)
	{}

	SerialWombatChip sw;	
	SerialWombatAnalogInput_18AB vinMeas;
	SerialWombatPWM_18AB output0,output1,output2,output3,output4,output5,output6,output7; 

	SerialWombatPWM_18AB* outputArray[8] = { &output0,&output1,&output2,&output3,&output4,&output5,&output6,&output7};


	/*!	
	@brief Initialize a PCB0042_LSD pcb on a given I2C address 
	
	Wire must be initialized before calling this function	
	
	@param i2cAddress The I2C address of the PCB
	@param measureVin Whether or not to enable an analog input on pin 1 (requires closing solder jumper)
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/
	int16_t begin(uint8_t i2cAddress, bool measureVin = false)
	{
		sw.begin(Wire, i2cAddress, false);
		

		for (int i = 0; i < 8; ++i)
		{
			outputArray[i]->begin((uint8_t)i);

		}

		if (measureVin)
		{
			vinMeas.begin(1);
		}
		return (0);
	}
	
	

	

	uint16_t readVin_mV()
	{
		return((uint16_t) (vinMeas.readAveraged_mV() * 11));

	}

};

