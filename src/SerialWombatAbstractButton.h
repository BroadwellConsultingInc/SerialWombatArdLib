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

/*! \file SerialWombatAbstractButton.h
*/


/*!
\brief SerialWombat18CapTouch, SerialWombatDebouncedInput and SerialWombatMatrixButton inherit from this class

This class provides a basic set of interface for any pin mode class that has a button-like interface
It allows classes like SerialWombatButtonCounter to work uniformly with any button-like class.
*/
class SerialWombatAbstractButton
{
public:

	SerialWombatAbstractButton() {}
	/*!
	\brief Returns the state of the input
	
	This function reads the status of an input
	\return TRUE or FALSE.  Meaning depends on inversion setting
	*/
	virtual bool digitalRead() = 0;

	/*!
	\brief return the number of mS that the input has been in true state
	
	Note that this value starts incrementing after the debounce period, not after the physical pin transition.
	
	\return returns a value in mS which saturates at 65535.  Returns 0 if currently false.
	*/
	virtual	uint16_t readDurationInTrueState_mS() = 0;

	/*!
	\brief return the number of mS that the input has been in false state
	
	Note that this value starts incrementing after any debounce period, not after the physical pin transition.
	
	\return returns a value in mS which saturates at 65535.  Returns 0 if currently true.
	*/
	virtual uint16_t readDurationInFalseState_mS() = 0;

	/// \brief Number of transitions returned by last call to readTransitionsState()
	uint16_t transitions = 0;

	/*!
	\brief Queries the number of transistions that have occured on the debounced input
	
	This function queries the debounced input for current state and transitions since last call.
	transition count is put in the global member transitions.  The debounced input in the Serial
	Wombat resets its count to zero after this call.
	
	\return TRUE or FALSE, current status of debounced input
	*/
	virtual bool readTransitionsState() = 0;

};


