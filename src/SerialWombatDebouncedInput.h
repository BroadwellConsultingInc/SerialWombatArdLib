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
#include "limits.h"

/*! \file SerialWombatDebouncedInput.h
*/

/*! \brief A pin mode class that debounces inputs

The SerialWombatDebouncedInput class is used to debounce inputs such as switches.

See the example sw4b_ard_Debounce1 distributed with the Serial Wombat Arduino Library for an example.

Video Tutorial

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/R1KM0J2Ug-M" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/R1KM0J2Ug-M

One SerialWombatDebouncedInput instance should be declared per debounced input.

The class sends commands and reads data from the Serial Wombat.  The actual switch
debouncing algorithm runs on the Serial Wombat.

The Serial Wombat's firmware algorithm polls the input every 1mS.  After it has 
been different from the reported value for X consecutive mS, the reported value 
changes to the new polled value.  The number of mS required for a change
can be configured.  

The Debounced Input mode keeps track of how long the debounced result has been
in the current state (up to 65535 mS) and can report this to the host.  This
is useful for implementing user interfaces that react based on how long a 
button has been held.

The Debounced Input mode keeps track of how many transitions have occured.
This can be used to poll the Debounced Input infrequently for status, but
still process all button presses/releases that occured since the last poll.

The pin mode has weak pull-up circuitry availble, and the ability to report 
inverted values.  These are both enabled when the simpliest begin() call is
used, allowing buttons that switch the Serial Wombat pin to ground to be used without
additional hardware.

See also the SerialWombatButtonCounter class which can run on top of this one.

*/
class SerialWombatDebouncedInput:public SerialWombatAbstractButton , public SerialWombatPin
{
public:
	/*!
	\brief Constructor for the SerialWombatDebouncedInput class.
	
	\param serialWombat a reference to the Serial Wombat on which the Debounced Input will exist
	*/
	SerialWombatDebouncedInput(SerialWombatChip& serialWombatChip):SerialWombatAbstractButton(),SerialWombatPin(serialWombatChip){}


	/*!
	\brief Initialize a debounced input 
	
	\param pin  The Serial Wombat pin used for the debounced input
	\param debounce_mS number of mS the pin must be stable to cause a transition
	\param invert FALSE: pin reading is returned  TRUE: inverted pin reading is returned
	\param usePullUp Whether the pin's weak pull up is enabled
	*/
	void begin(uint8_t pin, uint16_t debounce_mS = 30, bool invert = true, bool usePullUp = true)
	{
		_pin = pin;
		uint8_t tx[8] = { 200,_pin,10,SW_LE16(debounce_mS),invert,0,usePullUp };
		_sw.sendPacket(tx);
	}

	/*!
	\brief Returns the debounced state of the input
	
	This function reads from the public data of the pin which 
	indicates the debounced and invert adjusted state of the
	input
	\return TRUE or FALSE.  Meaning depends on inversion setting
	*/
	bool digitalRead()
	{
		return (_sw.readPublicData(_pin) > 0);
	}

	/*
	\brief return the number of mS that the debounced input has been in true state
	
	Note that this value starts incrementing after the debounce period, not after the physical pin transition.
	
	\return returns a value in mS which saturates at 65535.  Returns 0 if currently false.
	*/
	uint16_t readDurationInTrueState_mS()
	{
		
		uint8_t tx[8] = { 201,_pin,10,1,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx,rx);

		transitions +=  (256 * rx[5] + rx[4]);
		if (rx[3] == 0)
		{
			return (0);
		}
		else
		{
			return(256 * rx[7] + rx[6]);
		}
	}

	/*
	\brief return the number of mS that the debounced input has been in false state
	
	Note that this value starts incrementing after the debounce period, not after the physical pin transition.
	
	\return returns a value in mS which saturates at 65535.  Returns 0 if currently true.
	*/
	uint16_t readDurationInFalseState_mS()
	{

		uint8_t tx[8] = { 201,_pin,10,1,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);

		transitions += (256 * rx[5] + rx[4]);

		if (rx[3] == 1)
		{
			return (0);
		}
		else
		{
			return(256 * rx[7] + rx[6]);
		}
	}

	/*
	\brief Queries the number of transistions that have occured on the debounced input
	
	This function queries the debounced input for current state and transitions since last call.
	transition count is put in the global member transitions.  The debounced input in the Serial
	Wombat resets its count to zero after this call.
	
	\return TRUE or FALSE, current status of debounced input
	*/
	bool readTransitionsState()
	{
		uint8_t tx[8] = { 201,_pin,10,1,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);
		transitions = (256 * rx[5] + rx[4]);
		return (rx[3] > 0);
	}

private:
	uint8_t _pin = 255;
};


/*! \brief A class that runs on top of SerialWombaAbstractButton to increment or decrement a variable based on a button

This class runs on top of a SerialWombaAbstractButton input.  It is passed a variable reference in its begin call.
The update() method is then called periodically.  This method will look at how many times the debounced input has
transitioned since the last call, and also if the input is currently pressed and for how long.

A high limit and low limit can be set which keeps the variable from exceeing those bounds.

Times can be specified for button hold that allows the variable to be incremented at varying rates for short, medium,
and long holds.

See the example sw4b_ard_Debounce2 distributed with the Serial Wombat Arduino Library and in the video 

https://youtu.be/_EKlrEVaEhg

for an example.


*/
class SerialWombatButtonCounter
{
public:

	/*!
	\brief Constructor for SerialWombatButtonCounter
	\param serialWombatDebouncedInput  A pointer to an already initialized SerialWombatDebouncedInput, SerialWombatMatrixButton or digitally configured SerialWombat18CapTouch
	*/
	 SerialWombatButtonCounter( SerialWombatAbstractButton& serialWombatDebouncedInput):_debouncedInput(serialWombatDebouncedInput)
	{
		_debouncedInput = serialWombatDebouncedInput;
	}

	 /*!
	 Initializes the SerialWombatButtonCounter
	 
	 \param variableToIncrement  A pointer to a signed long integer
	 \param slowIncrement the amount that the variable should increment (or decrement if negative) per increment
	 \param slow_mS_betweenIncrements how often an increment should happen in slow mode
	 \param slowToMediumTransition_mS how long to stay in slow mode before switching to medium mode
	 \param mediumIncrement the amount that the variable should increment (or decrement if negative) per increment
	 \param medium_mS_betweenIncrements how often an increment should happen in medium mode
	 \param mediumToFastTransition_mS how long after the initail button press start until switching to Fast mode
	 \param fastIncrement the amount that the variable should increment (or decrement if negative) per increment
	 \param fast_mS_betweenIncrements how often an increment should happen in fast mode
	 */
	 void begin(long* variableToIncrement,
		 long slowIncrement = 1, unsigned long slow_mS_betweenIncrements = 250,
		 uint16_t slowToMediumTransition_mS = 1000, 
		 long mediumIncrement = 1,	 unsigned long medium_mS_betweenIncrements = 100, 
		 uint16_t mediumToFastTransition_mS = 1000 , 
		 long fastIncrement = 1, unsigned long fast_mS_betweenIncrements = 50)
	{
		_variableToIncrement = variableToIncrement;
		
		_slowIncrement = slowIncrement;
		_slow_mS_betweenIncrements = slow_mS_betweenIncrements;

		_slowToMediumTransition_mS = slowToMediumTransition_mS;
		
		_mediumIncrement = mediumIncrement;
		_medium_mS_betweenIncrements = medium_mS_betweenIncrements;

		_mediumToFastTransistion_mS = mediumToFastTransition_mS;

		_fastIncrement = fastIncrement;
		_fast_mS_betweenIncrements = fast_mS_betweenIncrements;

		_lastPressDuration = 0;

	}
	 /// \brief  Called periodically to query the SerialWombatDebouncedInput and update the variable
	bool update()
	{
		uint16_t pressDuration = _debouncedInput.readDurationInTrueState_mS();
		int increments = 0;
		bool incremented = false;
		bool pressed = false;
		if (pressDuration > 0)
		{
			if (_lastPressDuration >= pressDuration)
			{
				_lastPressDuration = 0;
			}

			if (pressDuration > _mediumToFastTransistion_mS)
			{
				// Increment fast
				increments = (pressDuration - _lastPressDuration) / _fast_mS_betweenIncrements;
				*_variableToIncrement += _fastIncrement * increments;
				_lastPressDuration += _fast_mS_betweenIncrements * increments;
			}
			else if (pressDuration > _slowToMediumTransition_mS)
			{
				// Increment medium
				increments = (pressDuration - _lastPressDuration) / _medium_mS_betweenIncrements;
				*_variableToIncrement += _mediumIncrement * increments;
				_lastPressDuration += _medium_mS_betweenIncrements * increments;
			}
			else
			{
				//Increment slow
				increments = (pressDuration - _lastPressDuration) / _slow_mS_betweenIncrements;
				*_variableToIncrement += _slowIncrement * increments;
				_lastPressDuration += _slow_mS_betweenIncrements * increments;
				incremented = increments > 0;  // An increment happened
			}
			if (incremented)
			{
				_debouncedInput.transitions = 0;  // Get rid of false->true transition so that final release doesn't cause and increment
			}
			pressed = true;
		}
		else 
		{
			// Button isn't currently pressed.  if there were other transitions, add them
			_lastPressDuration = 0;
			int presses = _debouncedInput.transitions / 2;
			 *_variableToIncrement += _slowIncrement * presses;
			 _debouncedInput.transitions -= presses * 2;
		}

		if (*_variableToIncrement > highLimit)
		{
			*_variableToIncrement = highLimit;
		}
		if (*_variableToIncrement < lowLimit)
		{
			*_variableToIncrement = lowLimit;
		}

		return (pressed);
	}

	/// \brief The variable will not increment above this limit.
	long highLimit = LONG_MAX;
	///  \brief The variable will not decrement below this limit.
	long lowLimit = LONG_MIN;

private:
	SerialWombatAbstractButton& _debouncedInput;
	long* _variableToIncrement;

	long _slowIncrement;
	unsigned long _slow_mS_betweenIncrements;
	
	uint16_t _slowToMediumTransition_mS;

	long _mediumIncrement; 
	unsigned long _medium_mS_betweenIncrements;
	
	uint16_t _mediumToFastTransistion_mS;

	long _fastIncrement;
	unsigned long _fast_mS_betweenIncrements;

	unsigned long _lastPressDuration;
	
};

