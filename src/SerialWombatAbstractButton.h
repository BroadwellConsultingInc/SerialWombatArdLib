#pragma once

#include "SerialWombat.h"
#include "limits.h"

/*! \file SerialWombatAbstractButton.h
*/

class SerialWombatAbstractButton
{
public:

	/// \brief Returns the state of the input
	/// 
	/// This function reads the status of an input
	/// \return TRUE or FALSE.  Meaning depends on inversion setting
	virtual bool digitalRead();

	/// \brief return the number of mS that the input has been in true state
	/// 
	/// Note that this value starts incrementing after the debounce period, not after the physical pin transition.
	/// 
	/// \return returns a value in mS which saturates at 65535.  Returns 0 if currently false.
	virtual	uint16_t readDurationInTrueState_mS();
	/// \brief return the number of mS that the input has been in false state
	/// 
	/// Note that this value starts incrementing after any debounce period, not after the physical pin transition.
	/// 
	/// \return returns a value in mS which saturates at 65535.  Returns 0 if currently true.
	virtual uint16_t readDurationInFalseState_mS();

	/// \brief Number of transitions returned by last call to readTransitionsState()
	uint16_t transitions = 0;

	/// \brief Queries the number of transistions that have occured on the debounced input
	/// 
	/// This function queries the debounced input for current state and transitions since last call.
	/// transition count is put in the global member transitions.  The debounced input in the Serial
	/// Wombat resets its count to zero after this call.
	/// 
	/// \return TRUE or FALSE, current status of debounced input
	virtual bool readTransitionsState();

};


