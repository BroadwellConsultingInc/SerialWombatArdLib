#pragma once
/*
Copyright 2020-2024 Broadwell Consulting Inc.

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

/*! \file SerialWombatAnalogInput.h
*/


enum class AnalogInputPublicDataOutput {
	AnalogInputPublicDataOutput_Raw = 0,  ///< The raw A/D reading is displayed as public data (default)
	AnalogInputPublicDataOutput_Filtered = 1,	///< The Filtered A/D reading is displayed as public data (default)
	AnalogInputPublicDataOutput_Averaged = 2,	///< The Averaged A/D reading is displayed as public data (default)
	AnalogInputPublicDataOutput_Minimum = 3,	///< The Minimum A/D reading is displayed as public data (default)
	AnalogInputPublicDataOutput_Maximum = 4,	///< The Maximum A/D reading is displayed as public data (default)
};

/*!

\brief A class to make analog measurements with the Serial Wombat.

The SerialWombatAnalogInput class is used to make measurements on a given pin.

Any analog-capable pin may be used to make a measurement.

Averaging of samples and first order IIR filtering (1 kHz sampling) of input are available.

For a good explanation of 1st order IIR filter calculations, see:
https://www.monocilindro.com/2017/04/08/how-to-implement-a-1st-order-iir-filter-in-5-minutes/

Some filter cut-off (3dB down) frequency and constant values:
      - 0.5 Hz  65417
	  - 1 Hz 65298   
	  - 2 Hz 65062
	  - 5 Hz 64358
	  - 10 Hz 63202

Filtering adds lag.  The higher the filter constant value, the longer it takes for the filter to settle 
when given a steady input.

Declare and initialize a SerialWombatAnalogInput instance for each pin being used as an analog input.

The SW4B_ard_analogInput example included with the Arduino library shows how to use this class.


A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/_EKlrEVaEhg" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/_EKlrEVaEhg

*/
class SerialWombatAnalogInput : public SerialWombatPin
{
public:
	/*!
	\brief Constructor for the SerialWombatAnalogInput class.
	
	\param SerialWombatChip a reference to the Serial Wombat chip on which the Analog Input will be measured
	*/
	SerialWombatAnalogInput(SerialWombatChip& SerialWombatChip):SerialWombatPin(SerialWombatChip){}

	

	

	/*!
	\brief Initialize an analog input on a given pin.
	
	\param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3 SW18AB: 0-4 and 16-19 
	\param averageSamples Number of samples to average.  Valid values for SW4A and SW4B are 0 or 64.
	All non-zero values will be treated as 64 on these platforms.  Default is 64.
	\param filterConstant First Order IIR filter constant, expressed as 1/65536ths .  Default is 65508
	Values closer to 65536 give heavier filtering.  Sample frequency is 1kHz.
	\param publicDataOutput What to output as pin public data.  Default is raw.
	\return Returns a negative error code if initialization failed.
	*/
	int16_t begin(uint8_t pin, uint16_t averageSamples = 64, uint16_t filterConstant = 0xFF80, AnalogInputPublicDataOutput output = AnalogInputPublicDataOutput::AnalogInputPublicDataOutput_Raw)
	{
		_pin = pin;
		_pinMode = PIN_MODE_ANALOGINPUT;
		int16_t result = initPacketNoResponse(0,0,0,0,0,0);
		if (result < 0) { return result; }
		//uint8_t tx1[] = { 201,_pin,PIN_MODE_ANALOGINPUT,SW_LE16(averageSamples) ,SW_LE16(filterConstant),(uint8_t)output };
		return initPacketNoResponse(1,averageSamples,filterConstant,(uint8_t)output);
	}


	/*!
	\brief Retreive a raw A/D measurement and convert it to mV
	
	Conversion is based on the most recent A/D conversion taken by the 
	Serial Wombat A/D at the command time and the last reference measurement made on the 
	Serial Wombat chip using the SerialWombatChip.readSupplyVoltage_mV() method.  
	\return A 16 bit unsigned value indicating measurement in mV
	*/
	uint16_t readVoltage_mV()
	{
		uint16_t reading = _sw.readPublicData(_pin);
		uint32_t x = ((uint32_t) reading ) * _sw._supplyVoltagemV;
		uint16_t returnval = x >> 16;
		return (returnval);
	}

	/*!
	\brief Retreive a raw A/D measurement
	
	Conversion is based on the most recent A/D conversion taken by the 
	Serial Wombat A/D at the command time.
	
	All Serial Wombat products will return a 16-bit value.  However
	the SW4A and SW4B products only have 10-bit A/D converters, so
	the returned value moves by 64 counts at a time, except for the topmost value.
	 For all 
	Serial Wombat products, the highest possible reading (0xFFC0 for the SW4A/SW4B, 0xFFF0 for the SW18AB)
	is changed to 0xFFFF to indicate maximum possible hardware value.
	
	\return A 16 bit unsigned value indicating the counts of the A/D conversion
	*/
	uint16_t readCounts()
	{
		return (_sw.readPublicData(_pin));
	}

	/*!
	\brief Retreive a filtered A/D measurement and convert it to mV
	
	Conversion is based on the most recent filtered A/D result taken by the 
	Serial Wombat A/D at the command time and the last reference measurement made on the 
	Serial Wombat chip using the SerialWombatChip.readSupplyVoltage_mV() method.  
	\return A 16 bit unsigned value indicating measurement in mV
	*/
	uint16_t readFiltered_mV() 
	{
		uint32_t x = readFilteredCounts(); // Counts ranging from 0 to 65535
		x *= _sw._supplyVoltagemV;
		return ((uint16_t)(x >> 16));
	}

	/*!
	\brief Retreive a filtered A/D measurement
	
	Conversion is based on the most recent filtered A/D value taken by the 
	Serial Wombat A/D at the command time.
	
	
	\return A 16 bit unsigned value indicating the filtered A/D result
	*/
	uint16_t readFilteredCounts()
	{
		uint8_t tx[] = { 204,_pin,PIN_MODE_ANALOGINPUT,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];

		_sw.sendPacket(tx, rx, true, 3,1);

		return(rx[5] + rx[6] * 256);
	}

	/*!
	\brief Retreive an averaged A/D measurement and convert it to mV
	
	Conversion is based on the most recent averaged A/D result taken by the 
	Serial Wombat A/D at the command time and the last reference measurement made on the 
	Serial Wombat chip using the SerialWombatChip.readSupplyVoltage_mV() method.  
	\return A 16 bit unsigned value indicating measurement in mV
	*/
	uint16_t readAveraged_mV()
	{
		uint32_t x = readAveragedCounts(); // Counts ranging from 0 to 65535
		x *= _sw._supplyVoltagemV;
		return ((uint16_t)(x >> 16));
	}

	/*!
	\brief Retreive an averaged A/D measurement
	
	Conversion is based on the most recent averaged A/D value taken by the 
	Serial Wombat A/D at the command time.
	
	All Serial Wombat products will return a 16-bit value.  However
	the SW4A and SW4B products only have 10-bit A/D converters.  Averaging will potentially
	increase the effective resolution slightly for signals that have a small amount of
	randomly distributed noise.  
	
	\return A 16 bit unsigned value indicating the counts of the A/D conversion
	*/
	uint16_t readAveragedCounts()
	{
		uint8_t tx[] = { 204,_pin,PIN_MODE_ANALOGINPUT,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx,true,3,1); 
		return(rx[3] + rx[4] * 256);
	}

	/// \brief Provides a wrapper around the readSupplyVoltage_mV() method for the SerialWombat chip hosting this pin mode
	uint16_t updateSupplyVoltage_mV()
	{
		return  _sw.readSupplyVoltage_mV();
	}


	/*!
	\brief Retreive the maximum single sample A/D value in mV
	 
	The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	
	\param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	
	\return A 16 bit unsigned value indicating measurement in mV
	*/
	uint16_t readMaximum_mV(bool resetAfterRead = false)
	{
		uint8_t tx[] = { 203,_pin,PIN_MODE_ANALOGINPUT,0,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		if (resetAfterRead)
		{
			tx[3] = 1;
		}
		_sw.sendPacket(tx, rx);
		return(rx[5] + rx[6] * 256);
	}


	/*!
	\brief Retreive the maximum single sample A/D value in counts
	 
	The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	\param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	
	\return A 16 bit unsigned value indicating maximum A/D Counts
	*/
	uint16_t readMaximumCounts(bool resetAfterRead = false)
	{
		uint8_t tx[] = { 203,_pin,PIN_MODE_ANALOGINPUT,0,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		if (resetAfterRead)
		{
			tx[3] = 1;
		}
		_sw.sendPacket(tx, rx);
		return(rx[5] + rx[6] * 256);
	}

	/*!
	\brief Retreive the minimum single sample A/D value in mV
	 
	The minimum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	\param resetAfterRead If True, minimum value is set to 0 after read so that subsequent values become minimum.  Also resets maximum to next sample.
	
	\return A 16 bit unsigned value indicating measurement in mV
	*/
	uint16_t readMinimum_mV(bool resetAfterRead = false)
	{
		uint32_t x = readMaximumCounts(resetAfterRead); // Counts ranging from 0 to 65535
		x *= _sw._supplyVoltagemV;
		return ((uint16_t)(x >> 16));
	}


	/*!
	\brief Retreive the maximum single sample A/D value in counts
	 
	The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	
	\param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	
	\return A 16 bit unsigned value indicating maximum A/D Counts
	*/
	uint16_t readMinimumCounts(bool resetAfterRead = false)
	{
		uint8_t tx[] = { 203,_pin,PIN_MODE_ANALOGINPUT,0,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		if (resetAfterRead)
		{
			tx[3] = 1;
		}
		_sw.sendPacket(tx, rx);
		return(rx[3] + rx[4] * 256);
	}

	
private:
	
};

/// \brief This class extends SerialWombatAnalogInput with SW18AB specific capabilities
class SerialWombatAnalogInput_18AB : public SerialWombatAnalogInput, public SerialWombatAbstractProcessedInput
{
public:

	SerialWombatAnalogInput_18AB(SerialWombatChip& serialWombat):SerialWombatAnalogInput(serialWombat),SerialWombatAbstractProcessedInput(serialWombat){}

	uint8_t pin() { return _pin; }
	uint8_t swPinModeNumber() { return _pinMode; }
};

