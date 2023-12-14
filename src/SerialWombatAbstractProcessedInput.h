#pragma once
/*
Copyright 2021 Broadwell Consulting Inc.

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

#include "SerialWombat.h"
#include "limits.h"

/*! \file SerialWombatAbstractProcessedInput.h
*/


/*! \brief SerialWombatAnalogInput, SerialWombatPulseTimer, SerialWombatResistanceInput and others inherit from this class

This class allows a common set of services to be applied to Serial Wombat inputs which inherit from it.  These include
SerialWombatAnalogInput, SerialWombatPulseTimer,  SerialWombatResistanceInput and will include others in the future.

This class is only applicable to the Serial Wombat 18AB firmware.

This class allows various transformations and filters to be performed on incoming measurements within the Serial Wombat
firmware using the Serial Wombat chip's cpu cycles.   Since this class is processed every 1mS for each pin configured to
an input class, it can do tasks like filtering or averaging much more quickly and consistently than could be achieved by
sampling the value over I2C or UART and doing the computation on the host device.  Minimum and Maximum measured values 
are also tracked for retreival by the host

Additionally, this class is capable of limiting input (for example any value below 10000 is processed as 10000, and any
value above 62331 is processed as 62331), scaling input (e.g. an expected input range of 3000 to 7000 is scaled linearly to the full 
Serial Wombat Range of 0 to 65535), mx+b linear transformations,  exclusion of outlier data (e.g. any value over 50000 is ignored, and
the previous valid measurement is substituted in its place). 

Inputs can be inverted (scaled from 0-65535 to 65535-0 by substracting the raw value from 65535).  This is useful for reversing the
direction of things like analog measured potentiometers.

The final output of the SerialWombatAbstractProcessedInput operations can be queued in a User Memory Area queue on a periodic basis.
This allows synchronsous sampling and storage of input data for retreival and processing by the host.  This allows waveforms to be
stored and processed.  Sampling period is an enumerated type ranging from 1mS to 1024mS in power of 2 intervals

Data processing happens in the following order each 1mS for any enabled feature:

- The pin mode measures the physical input
- Any outlier values are excluded.  if a value is excluded the last valid measured raw input is substituted in its place
- Inversion of input (subtraction of value from 65535)
- Transformation of output value (Scale of smaller input range (e.g. 8000-12000 to 0-65535) or mx+b linear transformation
- Averaging and filtering of the result of prior steps and storage of averaged / filtered values for access by the host.  
- Selection of the result to be passed to the next steps.  The unfiltered value, the averaged value, or the filtered value
can be selected to be the pin's public data output
- Updating the minimum and maximum recorded value for retreival by the host
- Sampling the data into a queue in the user buffer
- Placement of the value into the pin's 16-bit public data buffer for access by the host or other pin modes that react to a pin's public data buffer.


To use this class first configure the pin to its mode using the normal begin() call for that pin mode (the derived class).
Then call any configuratioon commands ( writeInverted, writeTransformLinearMXB, etc)
then call writeProcessedInputEnable(true) to enable processing.


*/
class SerialWombatAbstractProcessedInput 
{
public:
	/*!
	\brief Constructor for the SerialWombatAbstractScaledOutput Class
	
	\param sw A reference to a previously declared SerialWombatPin to which the output is connected.
	*/
	SerialWombatAbstractProcessedInput(SerialWombatChip& sw):_pisw(sw){}


	enum  Period {
		PERIOD_1mS = 0,
		PERIOD_2mS = 1,
		PERIOD_4mS = 2,
		PERIOD_8mS = 3,
		PERIOD_16mS = 4,
		PERIOD_32mS = 5,
		PERIOD_64mS = 6,
		PERIOD_128mS = 7,
		PERIOD_256mS = 8,
		PERIOD_512mS = 9,
		PERIOD_1024mS = 10,

	};

	enum OutputValue {
		RAW = 0,  ///<   Use the unfiltered signal for the pin's public data 
		FIRST_ORDER_FILTERED = 1, ///< Use a first order filtered signal for the pin's public data 
		AVERAGE = 2, ///< Use an averaged signal for the pin's public data (updates less often)
	};

	enum Transform {
		NONE = 0,  ///< Don't transform the input signal
		SCALE_RANGE = 1,  ///< Scale the input signal to a 0-65535 value based on input high and low limits
		LINEAR_MXB = 2  ///< Scale the input signal based on a linear mx+b equation
	};

	/*!
	\brief if enabled subtract the input value from 65535 before doing any other processing.
	
	\param inverted False - input value isn't changed.  True- input value is subtracted from 65535
	
	\return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeInverted(bool inverted)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			3,
			(uint8_t) inverted,
			0x55,0x55,0x55
		};

		int16_t result = _pisw.sendPacket(tx);

		return(result);
	}

	/*!
	\brief Set a first order filtering constant to be applied to the signal  Higher is heavier filtering
	
	The filter samples at 1kHz.
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
	
	\param constant  The first order filter constant from 0 to 65535.  Larger constant is heavier filtering
	\return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeFirstOrderFilteringConstant(uint16_t constant)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			11,
			SW_LE16(constant),
			0x55,0x55
		};
		return (_pisw.sendPacket(tx));
	}

	/*!
	\brief Set a number of samples to average for each update of the downstream signal
	
	The average is a typical average, not a moving average so the more samples comprise the averaged value
	the less often the downstream value will update.
	
	\param A number of samples to include in each output average.  Samples are typically taken each mS, although
	some modes such as SerialWombatUltrasonicDistanceSensor sample at other periods.
	\return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeAveragingNumberOfSamples(uint16_t numberOfSamples)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			1,
			SW_LE16(numberOfSamples),
			0x55,0x55
		};

		int16_t result = _pisw.sendPacket(tx);

		return(result);
	}

	/*!
	\brief Sets input value ranges which are discarded rather than processed
	
	Input values that are below the low parameter  or above the high parameter are not processed.
	The last valid input value is repeated instead.  This feature is designed to reject 
	outlier values, not to act as a high or low limiting clamp
	
	\param low input values below this value will not be processed
	\param high input values above this value will not be processsed.
	\return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeExcludeBelowAbove(uint16_t low, uint16_t high)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			2,
			SW_LE16(low),
			SW_LE16(high)
		};

		int16_t result = _pisw.sendPacket(tx);

		return(result);
	}


	/*!
	\brief Sets up the queueing feature for this pin mode.  Queue must have been initialized prior to this queue
	
	Allows periodic storage of the public data value into a previously initialized queue in the user memory buffer.
	The high byte, the low byte, or both can be stored into the queue.  This is useful to increase the number of 
	Samples that can be stored and transferred if 8 bit resolution is sufficent.   
	
	Note that the sampling period is an enumerated type, not a numerical value
	
	\param queue The index into the User Memory Buffer where the queue is located
	\param period The sampling period.  See the enumerated type for values
	\param queueHighByte  Whether to put the high byte of the sample into the queue
	\param queueLowByte Whether to put the low byte of the sample into the queue
	\return returns 0 or higher if success, or a negative error code
	*/
	int16_t configureQueue(SerialWombatQueue* queue, Period period, bool queueHighByte = true, bool queueLowByte = true)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			5,
			SW_LE16(queue->startIndex)
			,(uint8_t)period,
			(uint8_t)((((uint8_t) queueHighByte) << (uint8_t)1) | (uint8_t)queueLowByte)
		};
		int16_t result = _pisw.sendPacket(tx);
		return(result);
	}

	/*!
	\brief Configures whether the pin's public data value is averaged, filtered, or neither
	
	\param outputValue An enumerated type for filtered, averaged, or raw
	*/
	int16_t configureOutputValue(OutputValue outputValue)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			4,
			(uint8_t)outputValue,
			0x55,0x55,0x55
		};

		int16_t result = _pisw.sendPacket(tx);

		return(result);
	}

	/*!
	\brief Scale incoming values to a range of 0 to 65535
	
	This function allows configuration of an input scaling range that maps to 0-65535.  
	For example, if a sensor returns a range from 2000 to 5000, setting the minimum to 2000 and
	maximum to 5000 will cause values below 2000 to be 0, values above 5000 to be 65535, and values
	in between will be scaled accordingly.  This allows a sensor or other input device to scale to the
	Serial Wombat philosophy of using a 16 bit resolution number to represent the the possible range of values
	
	Calling this feature disables writeTransformLinearMXB until the pin is reinitialized with begin().
	
	\param min The minimum value of the input range.  Input values less than or equal to that will be scaled to 0
	\param max The maximum value of the input range.  Input values greater or equal to that will be scaled to 65535
	\return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeTransformScaleRange(uint16_t min, uint16_t max)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			6,
			SW_LE16(min),
			SW_LE16(max)
		};

		int16_t result = _pisw.sendPacket(tx);

		return(result);
	}

	/*!
	\brief Scale incoming values based on an mx+b linear equation
	
	Allows scaling of an input by multiplying by m, dividing by 256, and adding b.
	The m value can be thought of as a fraction with a divisor of 256.  This allows the scaling
	value to make the input bigger or smaller.  After the multiplication, division and addition
	the result is limited to the range of 0 to 65535.
	Calling this feature disables writeTransformScaleRange until the pin is reinitialized with begin().
	
	\param m A value between -16777215 and +1677215 representing the number of 256th by which to multiply the input 
	\param b A value between -65535 and 65535 to add to the result of the multiplication
	\return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeTransformLinearMXB(int32_t m, int32_t b)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			7,
			SW_LE32(((uint32_t)m)),
		};

		int16_t result = _pisw.sendPacket(tx);
		if (result < 0)
		{
			return(result);
		}

		uint8_t tx2[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			8,
			SW_LE32(((uint32_t)b)),
		};
	    result = _pisw.sendPacket(tx2);
		return(result);
	}

	/*!
	\brief Enables or disables all input processing functions
	If disabled, the raw input value is placed directly in the pin's 16 bit public data buffer
	*/
	int16_t writeProcessedInputEnable(bool enabled)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			0,
			(uint8_t)enabled,
			0x55,0x55,0x55
		};

		int16_t result = _pisw.sendPacket(tx);

		return(result);
	}

	/*!
	\brief Retreive the maximum public data output  value since the last call with reset= true
	
	\param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  
	
	\return A 16 bit unsigned value indicating the maximum value
	*/
	uint16_t readMinimum(bool resetAfterRead = false)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			9,
			(uint8_t)resetAfterRead,
			0x55,0x55,0x55
		};
		uint8_t rx[8];
		int16_t result = _pisw.sendPacket(tx,rx);
		if (result < 0)
		{
			return (65535);
		}
		return(rx[4] + 256*rx[5]);
	}

	/*!
	\brief Retreive the minimum public data output  value since the last call with reset= true
	
	\param resetAfterRead If True, minimum value is set to 65535 after read so that subsequent values become minimum.  
	
	\return A 16 bit unsigned value indicating the minimum value
	*/
	uint16_t readMaximum(bool resetAfterRead = false)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			10,
			(uint8_t)resetAfterRead,
			0x55,0x55,0x55
		};
		uint8_t rx[8];
		int16_t result = _pisw.sendPacket(tx, rx);
		if (result < 0)
		{
			return (65535);
		}
		return(rx[4] + 256 * rx[5]);
	}

	/*!
	\brief Retreive the last completed averaged value
	
	\return The last completed result of the averaging.  Note that because the average is a normal average and not
	a moving average, this value is unlikely to include the most recent raw samples.
	*/
	uint16_t readAverage()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			11,
			0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];
		int16_t result = _pisw.sendPacket(tx, rx);
		if (result < 0)
		{
			return (0);
		}
		return(rx[4] + 256 * rx[5]);
	}
	/*!
	\brief Retreive the filtered value
	
	\brief A 16 bit value representing the First Order IIR filtered result of the input
	*/
	uint16_t readFiltered()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
			pin(),
			swPinModeNumber(),
			11,
			0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];
		int16_t result = _pisw.sendPacket(tx, rx);
		if (result < 0)
		{
			return (0);
		}
		return(rx[6] + 256 * rx[7]);
	}
	/*!
	\brief Used for inheritance
	*/
	virtual uint8_t pin() = 0;

	/*!
	\brief Used for ineheritance
	*/
	virtual uint8_t swPinModeNumber() = 0;

private:
	SerialWombatChip& _pisw;
};


