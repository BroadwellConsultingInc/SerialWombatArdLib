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
@file PCB0046_HSD.h
*/
#include "SerialWombat.h"


/*!

@brief A class representing a PCB0046 HSD Eight Channel HSD High Side Driver board

The PCB0046 class makes all functionality from the PCB0046 board available.  


A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/

class PCB0046_HSD_PWM
{
public:
	PCB0046_HSD_PWM() :  sw0(),sw1(),vinMeas(sw1),currentSenseSelector1(sw1),currentSenseSelector2(sw1),selH(sw1),selL(sw1),diagEn(sw1),fault0to3(sw1),fault4to7(sw1),
		output0(sw0),output1(sw0),output2(sw0),output3(sw0),output4(sw0),output5(sw0),output6(sw0),output7(sw0)
	{}

	SerialWombatChip sw0,sw1;	
	SerialWombatAnalogInput_18AB vinMeas,currentSenseSelector1,currentSenseSelector2;
	SerialWombatDigitalOutput_18AB selH,selL,diagEn;
	SerialWombatDigitalInput_18AB fault0to3,fault4to7;
	SerialWombatPWM_18AB output0,output1,output2,output3,output4,output5,output6,output7; 

	SerialWombatPWM_18AB* outputArray[8] = { &output0,&output1,&output2,&output3,&output4,&output5,&output6,&output7};


	/*!	
	@brief Initialize a PCB0046_HSD pcb on a given I2C address and the I2C address + 1
	
	Wire must be initialized before calling this function	
	
	@param i2cAddress The I2C address of the PCB
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/
	int16_t begin(uint8_t i2cAddress)
	{
		sw0.begin(Wire, i2cAddress, false);
		sw1.begin(Wire, i2cAddress + 1, false);
		vinMeas.begin(1);
		currentSenseSelector1.begin(4);
		currentSenseSelector2.begin(3);
		diagEn.begin(2,HIGH);
		selL.begin(0,LOW);
		selH.begin(6,LOW);
		fault0to3.begin(7,true);
		fault4to7.begin(5,true);


		for (int i = 0; i < 8; ++i)
		{
			outputArray[i]->begin((uint8_t)i);
			outputArray[i]->writeFrequency_Hz(800); // Make different than 1mS Analog sample time
		}
		return (0);
	}
	int16_t selectCurrentFeedbackChannel(uint8_t ch)
	{
		switch (ch)
		{
			case 0:
			case 4:
			{
				selH.high();
				selL.high();
			}
			break;
			case 1:
			case 5:
			{
				selH.high();
				selL.low();
			}
			break;
			case 2:
			case 6:
			{
				selH.low();
				selL.high();
			}
			break;
			case 3:
			case 7:
			{
				selH.low();
				selL.low();
			}
			break;
			default:
			return -1;

		}

		selectedFeedbackChannel = ch;
	}

	uint16_t readCurrentFeedbackAverage_mA()
	{
		uint32_t mV;

		if (selectedFeedbackChannel >= 4)
		{
			mV = currentSenseSelector1.readAveraged_mV();
		}
		else
		{
			mV = currentSenseSelector2.readAveraged_mV();
		}

		// mv = mA * 100 ohms
		// mA = mV / 100
		// Actual mA is 300 times this due to current mirror factor in chip 
		// ma = mV * 300 / 100
		// ma = mV *  3
		mV *= 3;
	
		return (uint16_t)mV;

	}

	uint16_t readCurrentFeedbackInstant_mA()
	{
			uint32_t mV;

		if (selectedFeedbackChannel >= 4)
		{
			mV = currentSenseSelector1.readVoltage_mV();
		}
		else
		{
			mV = currentSenseSelector2.readVoltage_mV();
		}

		// mv = mA * 100 ohms
		// mA = mV / 100
		// Actual mA is 300 times this due to current mirror factor in chip 
		// ma = mV * 300 / 100
		mV *= 3;

		return (uint16_t)mV;
	}
	bool readChip4to7IsFaulted()
	{
		return (!fault4to7.readPublicData());
	}


	bool readChip0to3IsFaulted()
	{
		return (!fault0to3.readPublicData());
	}

	uint16_t readVin_mV()
	{
		return((uint16_t) (vinMeas.readAveraged_mV() * 11));

	}
	
	private:
	uint8_t selectedFeedbackChannel = 0;


};

