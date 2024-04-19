#pragma once
/*
Copyright 2024 Broadwell Consulting Inc.

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


/*! \file SerialWombatQueuedPulseOutput.h
*/

/*! \brief A Class which outputs pulses of a duration stored in a queue. 

This class is useful for outputting a number of queued pulses of the same or
different lengths.  For instance, this mode can be used to output arbitrary 
square waveforms such as the demodulated signal used for an IR remote control.

It is also useful for precise pulsing of motors, LEDs, heaters pumps, relays
or solenoids.

It could potentially also be used for UART style communication at non standard
baud rates.

A video Tutorial on this pin mode is available:

@htmlonly
//TODO  - Video coming soon
@endhtmlonly

//TODO https://youtu.be/


*/

class SerialWombatQueuedPulseOutput :  public SerialWombatPin 
{ 
	public: 
		/*!
		  @brief Class constructor for SerialWombatQueuedPulseOutput pin mode 
		  @param serialWombat The Serial Wombat chip on which the SerialWombatQueuedPulseOutput pin mode will run 
		 */
		SerialWombatQueuedPulseOutput(SerialWombatChip &sw):SerialWombatPin(sw){}

		/*!
		/// @brief Initialize a queued pulse output mode
		/// @param  pin The pin used to output the queued output mode 
		/// @param initialState 0 to initialize low, or 1 to initialize high
		/// @param idleState 0 to go low when queue empty, 1 to go high when queue empty
		/// @param unitsMs 0 to use uS as pulse time, 1 to use mS as pulse time
		/// @param QueueIndex if set to a value other than 0xFFFF use a previously created queue in User RAM rather than the small buffer in the pin's state machine memory
		/// @return a negative number if an error occured.
		 */
		int16_t begin(uint8_t pin, bool initialState = 0, bool idleState = 0, bool unitsMs = 0,  uint16_t QueueIndex = 0xFFFF) 
		{
			_pin = pin;
			_pinMode = PIN_MODE_QUEUED_PULSE_OUTPUT;

			uint8_t tx[] =
			{
				(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
				_pin,
				_pinMode,
				initialState,
				idleState,
				unitsMs, 
				SW_LE16(QueueIndex)
			};
			uint8_t rx[8];
			int16_t result = _sw.sendPacket(tx, rx);
			return (result);
		}

		/*!
		/// @brief Add a pulse or two pulses to the queue
		/// @param  firstPulse A number from 1 to 32767 logical ored with 0x8000 or 0x0000 to indicate a high or low pulse of a given duration (controlled by units in begin command).  0 indicates do nothing.  
		/// @param  secondPulse A number from 1 to 32767 logical ored with 0x8000 or 0x0000 to indicate a high or low pulse of a given duration (controlled by units in begin command).  0 indicates do nothing.
		/// @return number of pulses successfully queued or a negative number if an error occured.
		 */
		int16_t queuePulses(uint16_t firstPulse, uint16_t secondPulse = 0)
		{
			uint8_t tx[] =
			{
				(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
				_pin,
				_pinMode,
				SW_LE16(firstPulse),
				SW_LE16(secondPulse),
				0x55
			};
			uint8_t rx[8];
			int16_t result = _sw.sendPacket(tx, rx);
			if (result < 0)
			{
				return (result);
			}
			else
			{
				return (rx[3]);
			}
		}

		/*!
		/// @brief Pause or unpause the generation of new pulses
		/// @param  paused Whether the generation of pulses is paused or unpaused
		/// @return negative number if an error occured.
		 */
		int16_t pause(bool paused)
		{
			uint8_t tx[] =
			{
				(uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE2,
				_pin,
				_pinMode,
				paused,
				0x55,0x55,0x55, 0x55
			};
			int16_t result = _sw.sendPacket(tx);
				return (result);
		}
};
