#pragma once

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
class SerialWombatDebouncedInput
{
public:
	/// \brief Constructor for the SerialWombatDebouncedInput class.
	/// 
	/// \param serialWombat a reference to the Serial Wombat on which the Debounced Input will exist
	SerialWombatDebouncedInput(SerialWombat& serialWombat);

	/// \brief Initialize a debounced input (simplified for typical switch to ground)
	/// 
	/// This simplified initialization assumes a typical switch between the Serial Wombat pin and ground.
	/// Pull ups are turned on, debounce time is set to 30 mS, and inverted reporting is turned on
	/// so that a closed switch results in a TRUE reading.
	/// \param pin  The Serial Wombat pin used for the debounced input
	void begin(uint8_t pin);

	/// \brief Initialize a debounced input 
	/// 
	/// \param pin  The Serial Wombat pin used for the debounced input
	/// \param debounce_mS number of mS the pin must be stable to cause a transition
	/// \param invert FALSE: pin reading is returned  TRUE: inverted pin reading is returned
	/// \param usePullUp Whether the pin's weak pull up is enabled
	void begin(uint8_t pin, uint16_t debounce_mS, bool invert, bool usePullUp);

	/// \brief Returns the debounced state of the input
	/// 
	/// This function reads from the public data of the pin which 
	/// indicates the debounced and invert adjusted state of the
	/// input
	/// \return TRUE or FALSE.  Meaning depends on inversion setting
	bool digitalRead();

	/// \brief return the number of mS that the debounced input has been in true state
	/// 
	/// Note that this value starts incrementing after the debounce period, not after the physical pin transition.
	/// 
	/// \return returns a value in mS which saturates at 65535.  Returns 0 if currently false.
	uint16_t readDurationInTrueState_mS();
	/// \brief return the number of mS that the debounced input has been in false state
	/// 
	/// Note that this value starts incrementing after the debounce period, not after the physical pin transition.
	/// 
	/// \return returns a value in mS which saturates at 65535.  Returns 0 if currently true.
	uint16_t readDurationInFalseState_mS();

	/// \brief Number of transitions returned by last call to readTransitionsState()
	uint16_t transitions = 0;

	/// \brief Queries the number of transistions that have occured on the debounced input
	/// 
	/// This function queries the debounced input for current state and transitions since last call.
	/// transition count is put in the global member transitions.  The debounced input in the Serial
	/// Wombat resets its count to zero after this call.
	/// 
	/// \return TRUE or FALSE, current status of debounced input
	bool readTransitionsState();



private:
	SerialWombat& _sw;
	uint8_t _pin = 255;
	

};


/*! \brief A class that runs on top of SerialWombatDebouncedInput to increment or decrement a variable based on a button

This class runs on top of a SerialWombatDebounced input.  It is passed a variable reference in its begin call.
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

	/// \brief Constructor for SerialWombatButtonCounter
	/// \param serialWombatDebouncedInput  A pointer to an already initialized SerialWombatDebouncedInput
	 SerialWombatButtonCounter( SerialWombatDebouncedInput* serialWombatDebouncedInput);

	 /// Initializes the SerialWombatButtonCounter
	 /// 
	 /// \param variableToIncrement  A pointer to a signed long integer
	 /// \param slowIncrement the amount that the variable should increment (or decrement if negative) per increment
	 /// \param slow_mS_betweenIncrements how often an increment should happen in slow mode
	 /// \param slowToMediumTransition_mS how long to stay in slow mode before switching to medium mode
	 /// \param mediumIncrement the amount that the variable should increment (or decrement if negative) per increment
	 /// \param medium_mS_betweenIncrements how often an increment should happen in medium mode
	 /// \param mediumToFastTransition_mS how long after the initail button press start until switching to Fast mode
	 /// \param fastIncrement the amount that the variable should increment (or decrement if negative) per increment
	 /// \param fast_mS_betweenIncrements how often an increment should happen in fast mode
	 void begin(long* variableToIncrement,
		 long slowIncrement, unsigned long slow_mS_betweenIncrements,
		 uint16_t slowToMediumTransition_mS, 
		 long mediumIncrement,	 unsigned long medium_mS_betweenIncrements, 
		 uint16_t mediumToFastTransition_mS, 
		 long fastIncrement, unsigned long fast_mS_betweenIncrements);
	 /// \brief  Called periodically to query the SerialWombatDebouncedInput and update the variable
	bool update();

	/// \brief The variable will not increment above this limit.
	long highLimit = LONG_MAX;
	///  \brief The variable will not decrement below this limit.
	long lowLimit = LONG_MIN;

private:
	SerialWombatDebouncedInput* _debouncedInput;
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

