#pragma once

#include <stdint.h>
#include "SerialWombat.h"
#include "limits.h"

/// \brief A class that controls a Cap Touch Pin on the Serial Wombat 18AB chip
///
/// This class allows a metalic plate with a thin insulating layer to be used
/// as a capacitive touch surface.  Items like a coin, PCB board, metal plate, etc
/// can be connected directly to the pin and covered by a thin insulating layer.
/// A finger touch can be detected by the change in capacitance caused by its presence.
/// 
/// The mode can output either analog or digital values back to the host and as public
/// data to other pins.  In analog mode the A/D reading at the end of a charge cycle is
/// presented.  This value gets smaller when a finger or item causes the capacitance of
/// the sensor to increase.  (Smaller A/D values when finger present, higher values when
/// absent).
/// 
/// In digital mode the class is configured with a high and low limit which cause a
/// digital change in hysteresis manner.  This is useful when treating the touch
/// sensor like a button.   In digital mode the class implements the same interfaces
/// as the SerialWombatDebouncedInput class so that physical buttons and cap touch
/// inputs can be treated equivalently.    Settign the high and low limits further apart
/// will decrease the chance of false transitions but will also typically decrease the
/// responsiveness of the sensor.
/// 
/// 
/// 
/// Output public data values for touched and not touched are configurable.  This allows
/// other pin modes to react based on touch.  For instance, the touch and not touched
/// values might be set to 0x4000 and 0xC000 so that a servo set to monitor that public
/// data would move back and forth between 25% and 75% of its range depending on
/// whether or not a touch is present.
/// 
/// The final touch value is the result of 8 averaged samples in firmware remove noise. 
/// 
/// For particularly noisy signals a debounce option is also avaialble.  This requires
/// the specified number of samples to match before a transition is detected.  This
/// can help eliminate false transitions but makes the system less reponsive in terms of time
/// to transition after a touch is made or removed.
/// 
/// The Cap touch pin mode in the firmware takes exclusive access to the Microcontroller's
/// A/D hardware for a few milliseconds at a time.  This isn't an issue for most users
/// if the default 5ms delay between samples is used.  However, it should be considered
/// if multiple Cap Touch pins are being used simultaneously or if the delay is
/// decreased as they may combine to
/// starve other analog chanels and make conversions sporadic.  This may also 
/// impact performance of real-time control pin modes run on the Serial Wombat chip
/// such as PID control.
/// 
/// 
class SerialWombat18CapTouch
{
public:
	SerialWombat18CapTouch(SerialWombat& serialWombat);

	int16_t begin(uint8_t pin, uint16_t chargeTime);
	int16_t begin(uint8_t pin, uint16_t chargeTime, uint16_t delay);
	int16_t makeDigital(uint16_t touchLimit, uint16_t noTouchLimit);

	int16_t makeDigital(uint16_t touchLimit, uint16_t noTouchLimit, uint16_t touchValue, uint16_t noTouchValue, bool invert,uint16_t debounceCount);
	int16_t makeAnalog();

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
	uint16_t _trueOutput = 1;
	uint16_t _falseOutput = 1;
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
class SerialWombat18CapTouchCounter
{
public:

	/// \brief Constructor for SerialWombat18CapTouchCounter
	/// \param serialWombatCapTouch  A pointer to an already initialized SerialWombat18CapTouch
	SerialWombat18CapTouchCounter(SerialWombat18CapTouch* serialWombatCapTouch);

	/// Initializes the SerialWombat18TouchCounter
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
		long mediumIncrement, unsigned long medium_mS_betweenIncrements,
		uint16_t mediumToFastTransition_mS,
		long fastIncrement, unsigned long fast_mS_betweenIncrements);
	/// \brief  Called periodically to query the SerialWombatDebouncedInput and update the variable
	bool update();

	/// \brief The variable will not increment above this limit.
	long highLimit = LONG_MAX;
	///  \brief The variable will not decrement below this limit.
	long lowLimit = LONG_MIN;

private:
	SerialWombat18CapTouch* _capTouch;
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


