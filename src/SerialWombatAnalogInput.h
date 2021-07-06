#pragma once
#include <stdint.h>
#include "SerialWombat.h"

/*! \file SerialWombatAnalogInput.h
*/

class SerialWombat;

/*!

\brief A class to make analog measurements with the Serial Wombat.

The SerialWombatAnalogInput class is used to make measurements on a given pin.

Any analog-capable pin may be used to make a measurement.

Averaging of samples and first order IIR filtering (1 kHz sampling) of input are available

For a good explanation of 1st order FIR filter calculations, see:
https://www.monocilindro.com/2017/04/08/how-to-implement-a-1st-order-iir-filter-in-5-minutes/

Some filter cut-off (3dB down) frequency and constant values:
      - 0.5 Hz  65417
	  - 1 Hz 65298   
	  - 2 Hz 65062
	  - 5 Hz 64358
	  - 10 Hz 63202

Filtering adds lag.  The higher the filter constant value, the longer it takes for the filter to settle 
when given a steady input.

Declare and initialize a SerialWombatAnalogInput instance for each pin being used as an analog input.

The SW4B_ard_analogInput example included with the Arduino library shows how to use this class.


A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/_EKlrEVaEhg" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/_EKlrEVaEhg

*/
class SerialWombatAnalogInput
{
public:
	/// \brief Constructor for the SerialWombatAnalogInput class.
	/// 
	/// \param SerialWombat a reference to the Serial Wombat on which the Analog Input will be measured
	SerialWombatAnalogInput(SerialWombat& SerialWombat);

	/// \brief Initialize an analog input on a given pin.
	/// 
	/// This intialization turns on sample averaging to 64 samples and
	/// turns on first order IIR filtering with a cutoff frequency of 0.5 Hz
	/// 
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3 
	void begin(uint8_t pin);

	/// \brief Initialize an analog input on a given pin.
	/// 
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3 
	/// \param averageSamples Number of samples to average.  Valid values for SW4A and SW4B are 0 or 64.
	/// All non-zero values will be treated as 64 on these platforms.
	/// \param filterConstant First Order IIR filter constant, expressed as 1/65536ths .
	/// Values closer to 65536 give heavier filtering.  Sample frequency is 1kHz.
	void begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant);

	/// \brief Retreive a raw A/D measurement and convert it to mV
	/// 
	/// Conversion is based on the most recent A/D conversion taken by the 
	/// Serial Wombat at the command time and the last reference measurement made on the 
	/// Serial Wombat using the SerialWombat.readSupplyVoltage_mV() method.  
	/// \return A 16 bit unsigned value indicating measurement in mV
	uint16_t readVoltage_mV();

	/// \brief Retreive a raw A/D measurement
	/// 
	/// Conversion is based on the most recent A/D conversion taken by the 
	/// Serial Wombat at the command time.
	/// 
	/// All Serial Wombat products will return a 16-bit value.  However
	/// the SW4A and SW4B products only have 10-bit A/D converters, so
	/// the returned value moves by 64 counts at a time, except for the topmost value.
	///  For all 
	/// Serial Wombat products, the highest possible reading (0xFFC0 for the SW4A/SW4B)
	/// is changed to 0xFFFF to indicate maximum possible hardware value.
	/// 
	/// \return A 16 bit unsigned value indicating the counts of the A/D conversion
	uint16_t readCounts();

	/// \brief Retreive a filtered A/D measurement and convert it to mV
	/// 
	/// Conversion is based on the most recent filtered A/D result taken by the 
	/// Serial Wombat at the command time and the last reference measurement made on the 
	/// Serial Wombat using the SerialWombat.readSupplyVoltage_mV() method.  
	/// \return A 16 bit unsigned value indicating measurement in mV
	uint16_t readFiltered_mV();

	/// \brief Retreive a filtered A/D measurement
	/// 
	/// Conversion is based on the most recent filtered A/D value taken by the 
	/// Serial Wombat at the command time.
	/// 
	/// 
	/// \return A 16 bit unsigned value indicating the filtered A/D result
	uint16_t readFilteredCounts();

	/// \brief Retreive an averaged A/D measurement and convert it to mV
	/// 
	/// Conversion is based on the most recent averaged A/D result taken by the 
	/// Serial Wombat at the command time and the last reference measurement made on the 
	/// Serial Wombat using the SerialWombat.readSupplyVoltage_mV() method.  
	/// \return A 16 bit unsigned value indicating measurement in mV
	uint16_t readAveraged_mV();

	/// \brief Retreive an averaged A/D measurement
	/// 
	/// Conversion is based on the most recent averaged A/D value taken by the 
	/// Serial Wombat at the command time.
	/// 
	/// All Serial Wombat products will return a 16-bit value.  However
	/// the SW4A and SW4B products only have 10-bit A/D converters.  Averaging will potentially
	/// increase the effective resolution slightly for signals that have a small amount of
	/// randomly distributed noise.  
	/// 
	/// \return A 16 bit unsigned value indicating the counts of the A/D conversion
	uint16_t readAveragedCounts();

	/// Provides a wrapper around the readSupplyVoltage_mV() method for the SerialWombat hosting this pin mode
	uint16_t updateSupplyVoltage_mV();

	~SerialWombatAnalogInput();

	/// \brief Retreive the maximum single sample A/D value in mV
	///  
	/// The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	/// \param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	/// 
	/// \return A 16 bit unsigned value indicating measurement in mV
	uint16_t readMaximum_mV(bool resetAfterRead);

	/// \brief Retreive the maximum single sample A/D value in counts
	///  
	/// The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	/// \param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	/// 
	/// \return A 16 bit unsigned value indicating maximum A/D Counts
	uint16_t readMaximumCounts(bool resetAfterRead);

	/// \brief Retreive the minimum single sample A/D value in mV
	///  
	/// The minimum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	/// \param resetAfterRead If True, minimum value is set to 0 after read so that subsequent values become minimum.  Also resets maximum to next sample.
	/// 
	/// \return A 16 bit unsigned value indicating measurement in mV
	uint16_t readMinimum_mV(bool resetAfterRead);

	/// \brief Retreive the maximum single sample A/D value in counts
	///  
	/// The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	/// \param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	/// 
	/// \return A 16 bit unsigned value indicating maximum A/D Counts
	uint16_t readMinimumCounts(bool resetAfterRead);

private:
	SerialWombat &_sw ;
	
	uint8_t _pin = 255;
	
	
};

