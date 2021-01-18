#pragma once
#include <stdint.h>
#include "SerialWombat.h"

class SerialWombat;

/*! \file serialWombatPulseTimer.h
*/
typedef enum
{
	SW_PULSETIMER_uS = 0,
	SW_PULSETIMER_mS = 1
} SerialWombatPulseTimerUnits;

/*! \brief A Class which uses a Serial Wombat pin to measure the length of a pulse high and low time

This class is used to measure the length of pulses in mS or in uS.  On the Serial Wombat 4A/B the 
precision is about 8uS in uS mode, with an accuracy of +/- 2% because of variation in the 
Serial Wombat's internal oscillator.

One SerialWombatPulseTimer should be declared per Serial Wombat pin in this mode.

A video Tutorial on this pin mode is available:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/YtQWUub9gYw" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/YtQWUub9gYw

This pinmode is useful for measuring RC Servo outputs, PWM frequency and duty cycle, or any other signal where the pulse width
is relevant.  It is designed to measure repetitive signals.  It is not intended to read a number of differing independent pulse
widths that come one after another, such as WS2812 or similar output.

\warning Care must be taken when using this pinmode with high frequency ( > 5 kHz) signals or pins that may be left floating on the Serial Wombat 4A / 4B because
the Serial Wombat uses an interrupt internally to capture transition timestamps.  Excessively frequent pin transitions may cause the interrupt
handler to starve the main processing loop, impacting function of all pin modes and communications.

The Serial Wombat can be queried for overflow frames.  If overflow frames are occuring, then the system is overloaded.

The Serial Wombat 4A and 4B can measure a maxium of 8 transitions per mS across all pulse input pins.  More frequent transitions than
this may result in incorrect measurements.  The MeasurementOverflowOccurred flag is set if at any time more than 8 transitions occured
without processing.

The number of pulses over time can be used for rough frequency measurements.

Consecutive High and low periods can be used to calculate duty cycle, although there is no indication whether high or low was
most recent.

*/

class SerialWombatPulseTimer
{
public:
	/// \brief Class constructor for SerialWombatPulseTimer
	/// \param serialWombat The Serial Wombat on which the SerialWombatPulseTimer pinmode will be run
	SerialWombatPulseTimer(SerialWombat& serialWombat);

	/// \brief Simple initialization
	/// 
	/// This initialization takes a Serial Wombat pin as a parameter, sets units to uS and disables pull-ups
	/// \param pin The Serial Wombat pin used for pulse measurments.  All 4 pins on the SW4A/SW4B may be used.
	void begin(uint8_t pin);

	/// \brief Initialization routine for SerialWombatPulseTimer
	/// 
	/// \param pin The Serial Wombat pin used for pulse measurments.  All 4 pins on the SW4A/SW4B may be used.
	/// \param units SW_PULSETIMER_uS or SW_PULSETIMER_mS
	/// \param pullUpEnabled TRUE = Pull Up Enabled, FALSE = Pull Up Disabled
	void begin(uint8_t pin, SerialWombatPulseTimerUnits units, bool pullUpEnabled);

	/// \brief Retreive the latest values for HighCounts, LowCounts, Pulses, and MeasurementOverflowOccured
	/// 
	/// This happens in two packets to the Serial Wombat, so data may not be coherent
	void refresh();

	/// \brief Retreive the High and Low counts from the Serial Wombat in a single transaction
	/// 
	/// This command will retreive consecutive high and low periods from the serial Wombat.
	/// It it not guaranteed which is the most recent.
	void refreshHighCountsLowCounts();

	/// \brief Retreive the High counts and number of pulses Serial Wombat in a single transaction
	/// 
	/// This command will retreive consecutive high and low periods from the serial Wombat.
	/// It it not guaranteed which is the most recent.
	void refreshHighCountsPulses();

	/// \brief Retreives the most recent Counts in the configured units for the most recent high pulse
	/// 
	/// \return Counts in mS or uS depending on configuration.
	uint16_t readHighCounts();


	/// \brief Retreives the most recent Counts in the configured units for the most recent low pulse
	/// 
	/// \return Counts in mS or uS depending on configuration.
	uint16_t readLowCounts();

	/// \brief Retreives the number of pulses
	/// 
	/// This value overflows at 65535.  Reading this value does not clear it.
	/// This function has a side effect of reading MeasurementOverflowOccured from the Serial Wombat,
	/// which clears it for future readings.
	/// \return Number of pulses counted (1 count per high/low cycle).  Rolls over at 65535 to 0
	uint16_t readPulses();

	/// \brief Count in selected units of last retreived high pulse
	///  
	/// This value is updated by refresh, refreshHighCountsLowCounts, refreshHighCountsPulses, readHighCounts, readLowCounts
	uint16_t HighCounts = 0;
	/// \brief Count in selected units of last retreived low pulse
	///  
	/// This value is updated by refresh, refreshHighCountsLowCounts, readHighCounts, readLowCounts
	uint16_t LowCounts = 0;

	/// \brief Count of last retreived pulses
	/// 
	/// This value is updated by refresh, and refreshHighCountsPulses
	uint16_t Pulses = 0;
	bool MeasurementOverflowOccurred = false;

private:
	SerialWombat& _sw;
	uint8_t _pin = 255;
};


