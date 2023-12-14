#pragma once
/*
Copyright 2020-2023 Broadwell Consulting Inc.

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

class SerialWombat;

/*! \file serialWombatPulseTimer.h
*/
typedef enum
{
	SW_PULSETIMER_uS = 0,
	SW_PULSETIMER_mS = 1
} SerialWombatPulseTimerUnits;

/*! \brief A Class which uses a Serial Wombat pin to measure the length of a pulse high and low time

This class is used to measure the length of pulses in mS or in uS.  On the Serial Wombat 4A/B the 
precision is about 8uS in uS mode, with an accuracy of +/- 2% because of variation in the 
Serial Wombat chip's internal oscillator.

One SerialWombatPulseTimer should be declared per Serial Wombat pin in this mode.

A video Tutorial on this pin mode is available:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/YtQWUub9gYw" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/YtQWUub9gYw

This pinmode is useful for measuring RC Servo outputs, PWM frequency and duty cycle, or any other signal where the pulse width
is relevant.  It is designed to measure repetitive signals.  It is not intended to read a number of differing independent pulse
widths that come one after another, such as WS2812 or similar output.

\warning Care must be taken when using this pinmode with high frequency ( > 5 kHz) signals or pins that may be left floating on the Serial Wombat 4A / 4B because
the Serial Wombat 4A and 4B chip uses an interrupt internally to capture transition timestamps.  Excessively frequent pin transitions may cause the interrupt
handler to starve the main processing loop, impacting function of all pin modes and communications.

The Serial Wombat chip can be queried for overflow frames.  If overflow frames are occuring, then the system is overloaded.

The Serial Wombat 4A and 4B chips can measure a maxium of 8 transitions per mS across all pulse input pins.  More frequent transitions than
this may result in incorrect measurements.  The MeasurementOverflowOccurred flag is set if at any time more than 8 transitions occured
without processing.

The Serial Wombat 18AB chip uses a DMA based sampling scheme with samples taken at 57600 Hz.  It has lower
resolution than the Serial Wombat 4A/B (17 uS resolution), but can measure up to 18 simultaneous pulse trains and
is not subject to being overloaded by faster signals.

The number of pulses over time can be used for rough frequency measurements.

Consecutive High and low periods can be used to calculate duty cycle, although there is no indication whether high or low was
most recent.

*/

class SerialWombatPulseTimer : public SerialWombatPin
{
public:
	/*!
	\brief Class constructor for SerialWombatPulseTimer
	\param serialWombat The Serial Wombat chip on which the SerialWombatPulseTimer pinmode will be run
	*/
	SerialWombatPulseTimer(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat)
	{
	}

	/*!	
	\brief Initialization routine for SerialWombatPulseTimer
	
	\param pin The Serial Wombat pin used for pulse measurments.  All 4 pins on the SW4A/SW4B and all 20 pins on the SW18AB  may be used.
	\param units SW_PULSETIMER_uS or SW_PULSETIMER_mS.  Default uS
	\param pullUpEnabled TRUE = Pull Up Enabled, FALSE = Pull Up Disabled.  Default disabled.
	*/
	void begin(uint8_t pin, SerialWombatPulseTimerUnits units = SW_PULSETIMER_uS, bool pullUpEnabled = false)
	{
		_pin = pin;
		_pinMode = PIN_MODE_PULSETIMER;
		uint8_t tx[] = { 200,_pin,_pinMode,pullUpEnabled,(uint8_t)units,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);
	}

	/*!
	\brief Retreive the latest values for HighCounts, LowCounts, Pulses, and MeasurementOverflowOccured
	
	This happens in two packets to the Serial Wombat chip, so data may not be coherent
	*/
	void refresh()
	{
		refreshHighCountsLowCounts();
		{
			uint8_t tx[] = { 202,_pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			_sw.sendPacket(tx, rx);
			Pulses = rx[5] + 256 * rx[6];
			MeasurementOverflowOccurred = rx[7];
		}
	}

	/*!
	\brief Retreive the High and Low counts from the Serial Wombat chip in a single transaction
	
	This command will retreive consecutive high and low periods from the Serial Wombat chip.
	It it not guaranteed which is the most recent.
	*/
	void refreshHighCountsLowCounts()
	{
		uint8_t tx[] = { 201,_pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);
		HighCounts = rx[3] + 256 * rx[4];
		LowCounts = rx[5] + 256 * rx[6];
	}

	/*
	\brief Retreive the High counts and number of pulses Serial Wombat chip in a single transaction
	
	This command will retreive consecutive high and low periods from the Serial Wombat chip.
	It it not guaranteed which is the most recent.
	*/
	void refreshHighCountsPulses()
	{
		uint8_t tx[] = { 202,_pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);
		HighCounts = rx[3] + 256 * rx[4];
		Pulses = rx[5] + 256 * rx[6];
		MeasurementOverflowOccurred = rx[7];
	}

	/*
	\brief Retreives the most recent Counts in the configured units for the most recent high pulse
	
	\return Counts in mS or uS depending on configuration.
	*/
	uint16_t readHighCounts()
	{
		refreshHighCountsLowCounts();
		return (HighCounts);
	}


	/*
	\brief Retreives the most recent Counts in the configured units for the most recent low pulse
	
	\return Counts in mS or uS depending on configuration.
	*/
	uint16_t readLowCounts()
	{
		refreshHighCountsLowCounts();
		return (LowCounts);
	}

	/*
	\brief Retreives the number of pulses
	
	This value overflows at 65535.  Reading this value does not clear it.
	This function has a side effect of reading MeasurementOverflowOccured from the Serial Wombat chip,
	which clears it for future readings.
	\return Number of pulses counted (1 count per high/low cycle).  Rolls over at 65535 to 0
	*/
	uint16_t readPulses()
	{
		refreshHighCountsPulses();
		return(Pulses);
	}

	/*
	\brief Count in selected units of last retreived high pulse
	 
	This value is updated by refresh, refreshHighCountsLowCounts, refreshHighCountsPulses, readHighCounts, readLowCounts
	*/
	uint16_t HighCounts = 0;
	/*
	\brief Count in selected units of last retreived low pulse
	 
	This value is updated by refresh, refreshHighCountsLowCounts, readHighCounts, readLowCounts
	*/
	uint16_t LowCounts = 0;

	/*
	\brief Count of last retreived pulses
	
	This value is updated by refresh, and refreshHighCountsPulses
	*/
	uint16_t Pulses = 0;
	bool MeasurementOverflowOccurred = false;

private:

};

/*!
\brief extends the SerialWombatPulseTimer class with SW18AB specific functionality 
///
This class adds functionality that is specific to the SW18AB firmware in addition
to generic SerialWombatPulseTimer functionality avaialble on all Serial Wombat chips
*/

class SerialWombatPulseTimer_18AB : public SerialWombatPulseTimer, public SerialWombatAbstractProcessedInput
{
public:
	enum publicDataOutput {
		HIGH_TIME = 0, ///<  The pulse high time in uS.  Updated on each high to low transition.
		LOW_TIME = 1, ///< the pulse low time in uS.  Update on each low to high transition.
		PULSE_COUNT = 2, ///< The number of pulses that have occured since initialization.  Updated on each high to low transition
		PERIOD_ON_LTH_TRANSITION = 3,  ///< The period of the pulse in uS, based on the previous high and low times, updated on low to high transition
		PERIOD_ON_HTL_TRANSITION = 4,  ///< The period of the pulse in uS, based on the previous high and low times, updated on high to low transition
		FREQUENCY_ON_LTH_TRANSITION = 5, ///< The frequency of the pulse in Hz, based on the previous high and low times, updated on low to high transition
		FREQUENCY_ON_HTL_TRANSITION = 6, ///< The frequency of the pulse in Hz, based on the previous high and low times, updated on high to low transition
		DUTYCYCLE_ON_LTH_TRANSITION = 7, ///< Duty cycle of the pulse as a ratio from 0 to 65535, updated on low to high transition
		DUTYCYCLE_ON_HTL_TRANSITION = 8, ///< Duty cycle of the pulse as a ratio from 0 to 65535, updated on high to low transition
	};

	/*!
	\brief constructor for SerialWombatPulseTimer_18AB
	\param serialWombat reference to the SerialWombat chip on which the SerialWombatPulseTimer_18AB will run
	*/
	SerialWombatPulseTimer_18AB(SerialWombatChip& serialWombat):SerialWombatPulseTimer(serialWombat), SerialWombatAbstractProcessedInput(serialWombat)
	{
	}

	/*
	\brief configures which measurement is the Public Data Output of this pin mode
	 
	 This function sets what data is avaialble through the public data 
	 This function is only avaialble on the Serial Wombat 18AB chip
	
	\param publicDataOutput An enumerated type indicating what data to output
	\return returns 0 or higher for success or a negative error code.
	*/
	int16_t configurePublicDataOutput(SerialWombatPulseTimer_18AB::publicDataOutput publicDataOutput)
	{
		uint8_t tx[] = { 203,_pin,_pinMode,(uint8_t) publicDataOutput};
		return _sw.sendPacket(tx);
	}
	
	/// \brief Facilitates multi-inheritance
	uint8_t pin() { return _pin; }
	/// \brief Facilitates multi-inheritance
	uint8_t swPinModeNumber() { return _pinMode; }
};
