#pragma once
#include <stdint.h>
#include "SerialWombat.h"

/*! \file SerialWombatResistanceInput.h
*/


enum ResistanceInputPublicDataOutput {
	ResistanceInputPublicDataOutput_Raw = 0,  ///< The raw Resistance reading is displayed as public data (default)
	ResistanceInputPublicDataOutput_Filtered = 1,	///< The Filtered Resistance reading is displayed as public data (default)
	ResistanceInputPublicDataOutput_Averaged = 2,	///< The Averaged Resistance reading is displayed as public data (default)
	ResistanceInputPublicDataOutput_Minimum = 3,	///< The Minimum Resistance reading is displayed as public data (default)
	ResistanceInputPublicDataOutput_Maximum = 4,	///< The Maximum Resistance reading is displayed as public data (default)
};

/*!

\brief A class to make resistance measurements with the Serial Wombat.

The SerialWombatResistanceInput class is used to make resistance measurements on a given pin.

This pin mode is only available on the SerialWombat 18AB chip

Any analog-capable pin may be used to make a measurement.

Averaging of samples and first order IIR filtering (20 Hz sampling) of input are available

For a good explanation of 1st order FIR filter calculations, see:
https://www.monocilindro.com/2017/04/08/how-to-implement-a-1st-order-iir-filter-in-5-minutes/

Filtering adds lag.  The higher the filter constant value, the longer it takes for the filter to settle 
when given a steady input.

Declare and initialize a SerialWombatResistanceInput instance for each pin being used as a resistance input.


*/
class SerialWombatResistanceInput
{
public:
	/// \brief Constructor for the SerialWombatResistanceInput class.
	/// 
	/// \param SerialWombat a reference to the Serial Wombat chip on which the Resistance Input will be measured
	SerialWombatResistanceInput(SerialWombatChip& SerialWombat);

	/// \brief Initialize a resistance input on a given pin.
	/// 
	/// This intialization turns on sample averaging to 64 samples 
	/// 
	/// \param pin The Serial Wombat pin to set.  Must be an analog pin 
	/// 
	/// \return Returns a negative error code if initialization failed.
	int16_t begin(uint8_t pin);

	/// \brief Initialize a resistance input on a given pin.
	/// 
	/// \param pin The Serial Wombat pin to set.   Must be an analog pin
	/// \param averageSamples Number of samples to average.  
	/// \param filterConstant First Order IIR filter constant, expressed as 1/65536ths .
	/// Values closer to 65536 give heavier filtering.  Sample frequency is 20Hz.
	/// \return Returns a negative error code if initialization failed.
	int16_t begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant);

	/// \brief Initialize a resistance input on a given pin.
	/// 
	/// \param pin The Serial Wombat pin to set.  Must be an analog pin
	/// \param averageSamples Number of samples to average. 
	/// \param filterConstant First Order IIR filter constant, expressed as 1/65536ths .
	/// Values closer to 65536 give heavier filtering.  Sample frequency is 20 Hz.
	/// \param publicDataOutput What to output as pin public data
	/// \return Returns a negative error code if initialization failed.
	int16_t begin(uint8_t pin, uint16_t averageSamples, uint16_t filterConstant, ResistanceInputPublicDataOutput output);




	/// \brief Retreive a filtered Resistance measurement
	/// 
	/// Conversion is based on the most recent filtered Resistance value taken by the 
	/// Serial Wombat at the command time.
	/// 
	/// 
	/// \return A 16 bit unsigned value indicating the filtered Resistance result
	uint16_t readFilteredOhms();


	/// \brief Retreive an averaged Resistance measurement
	/// 
	/// Conversion is based on the most recent averaged Resistance value taken by the 
	/// Serial Wombat at the command time.
	/// 
	/// \return A 16 bit unsigned value indicating the ohms of the Resistance conversion
	uint16_t readAveragedOhms();


	/// \brief Retreive the maximum single sample Resistance value in Ohms
	///  
	/// The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	/// \param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	/// 
	/// \return A 16 bit unsigned value indicating maximum Resistance Ohms
	uint16_t readMaximumOhms(bool resetAfterRead);


	/// \brief Retreive the maximum single sample Resistance value in Ohms
	///  
	/// The maximum value the Serial Wombat chip has seen on that pin since last reset of Min/Max
	///
	/// \param resetAfterRead If True, maximum value is set to 0 after read so that subsequent values become maximum.  Also resets minimum to next sample.
	/// 
	/// \return A 16 bit unsigned value indicating maximum Resistance Ohms
	uint16_t readMinimumOhms(bool resetAfterRead);

private:
	SerialWombatChip &_sw ;
	
	uint8_t _pin = 255;
	
	
};

