#pragma once
#include "SerialWombat.h"
/*! \file SerialWombatWatchdog.h
*/

/*! \brief A class to wrap around a Serial Wombat Watchdog Pin

The Serial Wombat Watchdog is designed to improve system reliability in case of communications loss with the
host device.  This may be because the communications lines are no longer functional (e.g. I2C bus locked up)
or the host ceases to communicate (Such as when an Arduino malfunctions due to issues allocating string memory).

Once enabled, the Serial Wombat Watchdog will change its output and optionally other Serial Wombat  outputs
to predefined states and optionally reset the Serial Wombat itself
if a new Watchdog feeding message isn't received within a period of time specified in the initialization.

The output can be used to reset the host, for instance when connected to an Arduino reset pin, or
used to shut off an output.  For instance, a motor controlled by a SerialWombatWatchdog pin 
could be configured to turn off if the host doesn't periodically feed the watchdog.

A video tutorial is available:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/fIObjmHmprY" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/fIObjmHmprY

*/
class SerialWombatWatchdog
{
public:
	/// \brief Constructor for SerialWombatWatchdog
	/// \param serialWombat SerialWombat on which the SerialWombatWatchdog will run
	SerialWombatWatchdog(SerialWombat& serialWombat);
	/// \brief Initialize a SerialWombatWatchdog
	/// 
	/// \param pin Pin on which the SerialWombat Watchdog will run.  Valid values for SW4A: 0-3  SW4B: 1-3.
	/// \param normalState  The pin state while the countdown has not expired.  Valid values are SW_LOW, SW_HIGH or SW_INPUT
	/// \param resetState The pin state after the countdown has expired.  Valid values are SW_LOW, SW_HIGH or SW_INPUT
	/// \param timeout_mS The number of milliseconds before the countdown will expire if not reset.  Maximum value of 65535.
	/// \param resetWombatAfterTimeout TRUE:  The Serial Wombat will set outputs to resetState, wait 10mS, then reset the Serial Wombat itself.  FALSE:  Pins stay in resetState, Wombat continues operating
	void begin(uint8_t pin, SerialWombatPinState_t normalState, SerialWombatPinState_t resetState, uint16_t timeout_mS, bool resetWombatAfterTimeout);
	
	/// \brief Reset the countdown timer to a specified number of mS
	/// \param time_mS number of mS until countdown timer expires.
	void updateResetCountdown(uint16_t time_mS);
private:
	SerialWombat& _sw;
	uint8_t _pin = 255;
	uint16_t _resetStateTime = 10;

};

