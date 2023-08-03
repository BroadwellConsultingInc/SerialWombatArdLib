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

#include "SerialWombat.h"


/*! \file SerialWombatServo.h
*/

/*!

\brief A class to control Servos attached to the Serial Wombat chip.

The SerialWombatServo class is used to control hobby servos attached to the Serial Wombat chip.

Any output-capable pin may be attached to an instance of the SerialWombatServo class.
A separate instance should be declared for each Servo.

On the Serial Wombat 4A and Serial Wombat 4B chips servo output pulses are timed to 1uS precision 
using a timer interrupt.

On the Serial Wombat 18AB chip all Enhanced Digital Capability output pins (0-4,7,9-19)
can be timed to sub-microsecond precision by sharing hardware timing resources (assuming
resources are avaialble and have not be permanently claimed by PWM pin modes or other
pin modes that claim and hold timing resources).

For SW18AB pins that are not enhanced, or if timing resources are not available the
pin mode will degrade to using DMA based output with 17uS resolution.  This equates
to slightly better than 7 bit resolution for a 500-2500uS pulse range servo.

The SW4B_Ard_Servo example included with the Arduino library shows how to use this class.

This class uses "attach" rather than "begin" to initialize servos to be consistent with the
Arduino Servo native API.

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/WiciAtS1ng0" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/WiciAtS1ng0

*/
class SerialWombatServo : public SerialWombatPin
{
public:
	/// \brief Constructor for the SerialWombatServo Class
	/// 
	/// \param sw A reference to a previously declared SerialWombatChip to which the Servo is connected.
	SerialWombatServo(SerialWombatChip& sw);
	~SerialWombatServo();

	
	
	/// \brief Initialize a servo on the specified pin
	/// 
	/// This function must be called after the SerialWombatChip instance specified in the constructor
	/// has been initialized with a begin call.
	/// 
	/// This function initializes a servo with a minimum pulse of 544 uS and a maximum pulse of 2400uS
	/// to be consistent with the native Arduino API attach function.
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param reverse  If true, the pulse widths will be reversed, with the longest pulse width corresponding to 0 position.
	void attach(uint8_t pin, bool reverse);
	
	/// \brief Initialize a servo on the specified pin
	/// 
	/// This function must be called after the SerialWombatChip instance specified in the constructor
	/// has been initialized with a begin call.
	/// 
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param min Minimum pulse width in uS.  Minimum functional value is 100.
	/// \param max Maximum pulse width in uS
 	/// \param reverse  If true, the pulse widths will be reversed, with the longest pulse width corresponding to 0 position.
	void attach(uint8_t pin, uint16_t min = 544, uint16_t max = 2400, bool reverse = false);

	/// \brief Writes a value to the servo
	/// 
	/// This funciton allows setting the "angle" of a servo, although actual travel may vary.
	/// Consider using the SerialWombatServo function write16bit() for higher resolution
	/// 
	/// \param angle  A value from 0 to 180
	void write(uint8_t angle);

	/// \brief Writes a 16 bit value to the servo
	/// 
	/// This funciton scales the pulse width of the signal being sent to the  servo.
	/// 
	/// \param position Servo position.  0 sends the minimum pulse width.  65535 sends the maximum pulse width (unless Reverse is enabled)
	void write16bit(uint16_t position);

	/// \brief returns the last position of the servo scaled to a number from 0 to 180.
	/// 
	/// \return A position value from 0 to 180.
	uint8_t read(void);

protected:
	
	uint16_t _position = 0;
	uint16_t _min = 544;  // Default for Arduino Servo library
	uint16_t _max = 2400; // Default for Arduino Servo Library
	bool _reverse = false;
	void initializeServo();
};

/*!

\brief A derived class that adds additional Servo Functionality for the Serial Wombat 18AB Chip.

The SerialWombatServo_18AB class is used to control hobby servos attached to the Serial Wombat chip.

This class extends SerialWombatServo by adding support for SerialWombatAbstractScaledOutput interfaces

Any output-capable pin may be attached to an instance of the SerialWombatServo_18AB class.
A separate instance should be declared for each Servo.

On the Serial Wombat 18AB chip all Enhanced Digital Capability output pins (0-4,7,9-19)
can be timed to sub-microsecond precision by sharing hardware timing resources (assuming
resources are avaialble and have not be permanently claimed by PWM pin modes or other
pin modes that claim and hold timing resources).

For SW18AB pins that are not enhanced, or if timing resources are not available the
pin mode will degrade to using DMA based output with 17uS resolution.  This equates
to slightly better than 7 bit resolution for a 500-2500uS pulse range servo.


This class uses "attach" rather than "begin" to initialize servos to be consistent with the
Arduino Servo native API.
*/
class SerialWombatServo_18AB : public SerialWombatServo, public SerialWombatAbstractScaledOutput
{
public:
	SerialWombatServo_18AB(SerialWombatChip& serialWombat);
	uint8_t pin();
	uint8_t swPinModeNumber();

};

/*!

\brief A class for the Serial Wombat 18AB Chip that adds support for high frequency hobby Servos(200 Hz, 333Hz, 560Hz, etc)

The SerialWombatHighFrequencyServo class is used to control hobby high frequency servos attached to the Serial Wombat chip.

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/sCQGRyau40g" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/sCQGRyau40g


This class SerialWombatServo_18AB SerialWombatServo by adding support for a frequency/period setting and by
changing the on-chip pin mode to one optimized for high speed servos.

Only  Enhanced Digital Capability output pins (0-4,7,9-19) pins can be used for this pin mode.

Up to six High Frequency Servo pins may be assigned per Serial Wombat 18AB Chip.  This pin mode
claims and holds one of the 6 timing resources also used by PWM output, standard servo output,
etc.  

This mode creates rapid pulse outputs by using a PWM rather than pulse generation mode.  This makes it
well suited for fast updates, (200 Hz or better) but a poor choice for driving standard 50Hz servos as
it will have worse resolution at low speeds than the standard mode.

The SerialWombatServo and SerialWombatServo_18AB interface void attach(uint8_t pin, bool reverse)
is not available in this mode, as it must be explicitly configured for minimum and maximum pulse
width.  

The pulse update rate can be set with writeFrequency() or writePeriod().

This class uses "attach" rather than "begin" to initialize servos to be consistent with the
Arduino Servo native API.
*/
class SerialWombatHighFrequencyServo : public SerialWombatServo_18AB
{

public:
	SerialWombatHighFrequencyServo(SerialWombatChip& serialWombat);
	/// \brief Do not use this interface for High Frequency Servos
	/// 
	/// \param pin The Serial Wombat pin to set.  Do not use this interface.  Use the one below.
	/// \param reverse  Do not use this interface.  Use the one below.
	void attach(uint8_t pin, bool reverse);

	/// \brief Initialize a high Speed servo on the specified pin
	/// 
	/// This function must be called after the SerialWombatChip instance specified in the constructor
	/// has been initialized with a begin call.  This function must be followed by a call to writeFrequency() or
	/// writePeriod()
	/// 
	/// \param pin The Serial Wombat pin to set. The Serial Wombat pin to set.  This must be an enhanced capability pin ( WP0-4, WP7, or WP9-19)
	/// \param min Minimum pulse width in uS.  
	/// \param max Maximum pulse width in uS.  Needs to be less than the value specified after in writePeriod / write Frequency
	/// \param reverse  If true, the pulse widths will be reversed, with the longest pulse width corresponding to 0 position.
	void attach(uint8_t pin, uint16_t min , uint16_t max , bool reverse = false);


	int16_t writeFrequency_Hz(uint16_t frequency_hZ);
	int16_t writePeriod_uS(uint16_t period_uS);
};

