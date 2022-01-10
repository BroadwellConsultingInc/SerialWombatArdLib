#pragma once
/*
Copyright 2021 Broadwell Consulting Inc.

Serial Wombat is a registered trademark of Broadwell Consulting Inc. in the United States.

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


/// \brief A Class that consumes CPU time in order to facilitate testing 
///
/// This class is only supported on the Serial Wombat SW18AB chip.  It is not supported on the
/// Serial Wombat 4X line.
/// 
/// This class provides a means to consume throughput inside of a pin in order to test different CPU loading secenarios.  
/// 
/// This class provides an array of 16 delays measured in uS.  When each
/// frame runs the cumulative frame counter is taken mod 16 to determine
/// which delay to use.  During this delay the pin goes high and waits in a 
/// loop for approximately the specified number of uS. 
/// 
class SerialWombatThroughputConsumer
{
public:
	/// \brief Constructor for SerialWombatThroughputConsumer class
   /// \param serialWombat SerialWombatChip on which the ThroughputConsumer will run
	SerialWombatThroughputConsumer(SerialWombatChip& serialWombat);

	/// \brief Initialize an instance of the Throughput Conumer class.  All delays are set to 0.
	///
	/// \return Returns a negative error code if initialization failed.
	int16_t begin(uint8_t pin);


	/// \brief Set all delay times to a specified number of uS
	///
	/// \param delay  The number of uS to delay in each frame
	/// \return Returns a negative error code if errors occur during configuration 
	/// 
	int16_t writeAll(uint16_t delay);

	/// \brief Set a frame delay time to a specified number of uS
	///
	/// \param frame The frame number (0-15) to set
	/// \param delay  The number of uS to delay in each frame
	/// \return Returns a negative error code if errors occur during configuration 
	/// 
	int16_t write(uint8_t frame, uint16_t delay);


	/// \brief Delay a specified number of uS within the packet processing routine
	///
	/// \param delay  The number of uS to delay in each frame
	/// \return Returns a negative error code if errors occur during configuration 
	/// 
	int16_t delayInCommProcessing(uint16_t delay);
private:
	SerialWombatChip& _sw;

	uint8_t _pin = 255;
};

