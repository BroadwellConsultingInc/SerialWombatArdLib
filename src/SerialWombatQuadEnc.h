#pragma once

/*
Copyright 2020-2021 Broadwell Consulting Inc.

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


/*! \file SerialWombatQuadEnc.h
*/

typedef enum
{
	QE_ONLOW_INT = 0,  ///< Interrupt driven, process on high to low transition
	QE_ONHIGH_INT = 1, ///< Interrupt driven, process on low to high transition
	QE_ONBOTH_INT = 2, ///< Interrupt driven, process on low to high and high to low transition
	QE_ONLOW_POLL = 4,   ///< 1mS Polling, process on high to low transition
	QE_ONHIGH_POLL = 5,	 ///< 1mS Polling, process on low to high transition
	QE_ONBOTH_POLL = 6	 ///< 1mS Polling, process on low to high and high to low transition
} QE_READ_MODE_t;



/*! \brief A class that uses two Serial Wombat input pins to read quadrature encoder input

The SerialWombatQuadEnc class configures two pins on the Serial Wombat chip to work together to 
read quadrature encoder inputs.  

By offloading the reading of an encoder to the Serial Wombat chip, it makes it easy for the host
to track multiple encoders at once.  The host need only periodically retreive the net change
in rotary encoder position from the Serial Wombat chip rather than monitoring for every signal
change.

Video Tutorial:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/_wO8cOada3w" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/_wO8cOada3w

An instance of the SerialWombatQuadEnc class should be declared for each encoder (2 pins) 
connected to the Serial Wombat chip.

The quadrature encoder is capable of running in either polled or interrupt/DMA driven modes.  

Polled mode is recommended for manual inputs such as rotary encoder knobs.  It polls at 1 kHz
which is fast enough for most applications.

Interrupt driven mode on the Serial Wombat 4A/4B is capable of correctly decoding very fast signals.  However,
the signals need to be properly filtered in order to eliminate any bouncing.

DMA driven mode on the Serial Wombat 18AB samples at 57600 Hz rather than 1 kHz and can decode
pulse inputs from rapidly spinning encoders, but using DMA mode consumes much more CPU throughtput
on the Serial Wombat 18AB chip.

The SerialWombatQuadEnc can make use of the Serial Wombat chip's built in pull-up resistors
to make connecting a rotary encoder knob very simple.  Debouncing is available which prevents
additional transitions from being measured for a specified number of mS after a transition.

Rotational direction measurement can be changed by switching the "pin" and "second pin" parameters
in the begin call.

The reported position can be changed on low to high transitions of "pin", high to low transitions,
or both transitions.  This allows knobs that make and break connection on each click/detent and knobs
that either make or break connection on each detent to report one change per detent to the host.  

The default mode for simple initialization is to measure both, which will result in 2 increments per
detent for encoders that make and break connection on each detent.

\warning Care must be taken in interrupt mode on Serial Wombat 4A/4B chips  when using this pinmode with high frequency ( > 5 kHz) signals or pins that may be left floating on the Serial Wombat 4A / 4B because
the Serial Wombat uses an interrupt internally to capture transitions between state machine updates.  Excessively frequent pin transitions may cause the interrupt
handler to starve the main processing loop, impacting function of all pin modes and communications.

The Serial Wombat chip can be queried for overflow frames.  If overflow frames are occuring, then the system is overloaded.

The Serial Wombat 4A and 4B chips can measure a maxium of 8 transitions per mS across all pulse input pins.  More frequent transitions than
this may result pin mode malfunction.
*/

class SerialWombatQuadEnc:public SerialWombatPin
{
public:

	/// \brief Constructor for the SerialWombatQuadEnc class
	/// \param serialWombatChip The Serial Wombat chip running the quadrature encoder
	SerialWombatQuadEnc(SerialWombatChip& serialWombatChip);

	/// \brief Simple initialization for SerialWombatQuadEnc
	/// \param pin The first pin to be used as a Quadrature Encoder input. All 4 pins on the SW4A/SW4B may be used.  All 20 pins on the SW18AB chip may be used
	/// \param secondPin The second pin to be used as a Quadrature Encoder input. All I/O pins on Serial Wombat chip may be used, except the first specified pin.
	/// 
	/// This initialization assumes a simple Rotary Encoder knob that is connected to ground.  Polling mode is used, and both transitions generate increments.
	/// Pull ups are enabled, and 10ms of debouncing is used.
	/// Position is initialized to 0.
	void begin(uint8_t pin, uint8_t secondPin);

	/// \brief Initialization for SerialWombatQuadEnc that allows configuration of debounce time and pullUps
/// \param pin The first pin to be used as a Quadrature Encoder input. All 4 pins on the SW4A/SW4B may be used.  All 20 pins on the SW18AB chip may be used
	/// \param secondPin The second pin to be used as a Quadrature Encoder input. All I/O pins on Serial Wombat chip may be used, except the first specified pin.
		/// \param debounce_mS The number of mS after a transition is detected and increment occurs before another increment is allowed.
	/// \param pullUpsEnabled TRUE: both pins have weak pull ups enabled.  FALSE: Neither pin has weak pull up enabled.
	/// 
	/// Polling mode is used, and both transitions generate increments.
	/// Position is initialized to 0.
	void begin(uint8_t pin, uint8_t secondPin, uint16_t debounce_mS, bool pullUpsEnabled);

	/// \brief Initialization for SerialWombatQuadEnc that allows configuration of debounce time, pullUps and transition detection mode
/// \param pin The first pin to be used as a Quadrature Encoder input. All 4 pins on the SW4A/SW4B may be used.  All 20 pins on the SW18AB chip may be used
	/// \param secondPin The second pin to be used as a Quadrature Encoder input. All I/O pins on Serial Wombat chip may be used, except the first specified pin.
		/// \param debounce_mS The number of mS after a transition is detected and increment occurs before another increment is allowed.
	/// \param pullUpsEnabled TRUE: both pins have weak pull ups enabled.  FALSE: Neither pin has weak pull up enabled.
	/// \param readState #QE_READ_MODE_t determine if Polling or Interrupt/DMA mode is used, and which transitions cause increments.
	/// 
	/// Position is initialized to 0.
	void begin(uint8_t pin, uint8_t secondPin, uint16_t debounce_mS, bool pullUpsEnabled, QE_READ_MODE_t readState);

	/// \brief Read the quadrature encoder position from the Serial Wombat chip
	/// \return a 16 bit unsigned value indicating position.  Rolls from 0 to 65535 when decrementing below 0
	uint16_t read();

	/// \brief Read the quadrature encoder position from the Serial Wombat chip then set the position value
	/// \return a 16 bit unsigned value indicating position.  Rolls from 0 to 65535 when decrementing below 0
	/// \param replacementValue  The 16 bit value to set the position to after read.
	///
	/// This method is useful when looking for relative change since last call.  By setting the position to center (32768)
	/// after each read, the net postion change can easily be measured.  This is conceptually simplier than using read() 
	/// and adjusting for rollover between 65535 and 0.

	uint16_t read(uint16_t replacementValue);

	/// \brief This function initializes the position of the encoder
	/// \param value A starting value between 0 and 65535 for the encoder
	void write(uint16_t value);
private:

	uint8_t _secondPin;
};

