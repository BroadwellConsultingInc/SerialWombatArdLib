#pragma once
/*
Copyright 2020-2024 Broadwell Consulting Inc.

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


#include <stdint.h>
#include "SerialWombat.h"
/*! \file SerialWombatPWM.h
*/

typedef enum 
{
    SW4AB_PWMFrequency_1_Hz = 0x76,
    SW4AB_PWMFrequency_2_Hz = 0x66,
    SW4AB_PWMFrequency_4_Hz = 0x56,
    SW4AB_PWMFrequency_8_Hz = 0x46,
    SW4AB_PWMFrequency_16_Hz = 0x75,
    SW4AB_PWMFrequency_32_Hz = 0x65,
    SW4AB_PWMFrequency_63_Hz = 0x55,
    SW4AB_PWMFrequency_125_Hz = 0x45,
    SW4AB_PWMFrequency_244_Hz = 0x71,
    SW4AB_PWMFrequency_488_Hz = 0x61,
    SW4AB_PWMFrequency_976_Hz = 0x51,
    SW4AB_PWMFrequency_1952_Hz = 0x41,
    SW4AB_PWMFrequency_3900_Hz = 0x31,
    SW4AB_PWMFrequency_7800_Hz = 0x21,
    SW4AB_PWMFrequency_15625_Hz = 0x11,
    SW4AB_PWMFrequency_31250_Hz = 0x01,
}Wombat4A_B_PWMFrequencyValues_t;

/*!
\brief A class representing a Serial Wombat PWM output

An instance of this class should be declared for each pin
to be used as a Serial Wombat PWM.  

SW4A / SW4B PWMs are initialized to a frequency of 31250 Hz at startup.
This frequency can be changed using the setFrequency_SW4AB method.
All PWM outputs use the same clock divider, so a change in frequency
to one PWM output will affect other outputs.

SW4A/4B PWM inputs are either 8 or 10 bit resolution, depending on frequency
selection.  The duty cycle parameter of methods that set duty cycle
take a 16 bit value ranging from 0 to 65535 as an input regardless of
resolution, with 0 being
always low, and 65535 being always high.

Serial Wombat 18AB PWM outputs are driven either by hardware peripherals
or by a DMA based software PWM scheme.  Up to 6 hardware PWM outputs are avaialble
on Enhanced Digital Performance pins (0-4,7,9-19).  The first six Enhanced Digitial
Performance pins configured after reset will claim hardware resources.  Any additional
pins configured for PWM will use DMA based output.  Hardware capable pins can 
generate high resolution signals up to about 100kHz.  DMA based output is limited
to transitions every 17uS, so a 1kHz output will have about 6 bits of resolution and
a 100 Hz output will have about 9 bit resolution.
*/

class SerialWombatPWM : public SerialWombatPin
{
public:
	/*!
    \brief Constructor for SerialWombatPWM class
    \param serialWombat SerialWombat  chip on which the PWM will run
    */
    SerialWombatPWM(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat) {}

	/*!
    \brief Initialize a pin that has been declared as PWM. 
   \param pin The pin to become a PWM.  Valid values for SW4A: 0-3  SW4B: 1-3 
   \param dutyCycle A value from 0 to 65535 representing duty cycle
   \param invert if true, internally adjust duty cycle to 65535-duty cycle
   */
    int16_t begin(uint8_t pin, uint16_t dutyCycle = 0,bool invert = false)
	{
		_pin = pin;
		_pinMode = (uint8_t)PIN_MODE_PWM;
		/*
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,_pin,PIN_MODE_PWM,_pin,(uint8_t)(dutyCycle & 0xFF),(uint8_t)(dutyCycle >> 8),invert,0x55 };
		_sw.sendPacket(tx);
		*/
		return initPacketNoResponse(0,_pin,SW_LE16(dutyCycle),(uint8_t) invert);
	}

    /*
    \brief Set PWM duty cycle
    \param dutyCycle A value from 0 to 65535 representing duty cycle
    */
	void writeDutyCycle(uint16_t dutyCycle)
	{
		writePublicData(dutyCycle);
	}

private:
};


/// \brief Extends the SerialWombatPWM class with SW4A/SW4B specific functionality
class SerialWombatPWM_4AB : public SerialWombatPWM
{
public:
    SerialWombatPWM_4AB(SerialWombatChip& serialWombat):SerialWombatPWM(serialWombat){}
    /*
    \brief Set PWM Frequency (Adjusts all PWM outputs' frequency on a SerialWombat 4A/B chip)
    \param frequency  A value of the #Wombat4A_B_PWMFrequencyValues_t enumeration
    
    This function changes the Serial Wombat 4A and 4B PWM output frequncy by adjusting
    the clock divisor for the PWM generation hardware.  By default the value is 31250Hz.
    Changing the frequency may reduce PWM resolution from 10 bits to 8 bits for some
    frequencies.  However, the input value for duty cycle for methods of this class
    continue to be 0 to 65535 and are scaled accordingly.
    
    \warning This function will likely not be compatible with other models in the Serial Wombat
    family based on other hardware that are released in the future because it is tightly coupled to the
    PIC16F15214 hardware.
    */
    int16_t setFrequency_SW4AB(Wombat4A_B_PWMFrequencyValues_t frequency)
	{
		/*
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE_HW_0,_pin,PIN_MODE_PWM,(uint8_t)(frequency),0x55,0x55,0x55,0x55 };
		_sw.sendPacket(tx);*/
		return initPacketNoResponse(20,(uint8_t) frequency);
	}
};

/// \brief Extends the SerialWombatPWM class with SW18AB specific functionality, including SerialWombatAbstractScaledOutput
class SerialWombatPWM_18AB: public SerialWombatPWM, public SerialWombatAbstractScaledOutput
{
public:
    SerialWombatPWM_18AB(SerialWombatChip& serialWombat) :SerialWombatPWM(serialWombat), SerialWombatAbstractScaledOutput(serialWombat)
    {}

    /*!
    \brief Set the PWM frequency on a Serial Wombat 18AB chip's PWM
   
   \param frequency_Hz  Frequency in Hz.  Note that actual frequency may vary based on hardware capabilities of the pin.
   */
    void writeFrequency_Hz(uint32_t frequency_Hz)
	{
		uint8_t tx[] = { 220,_pin,PIN_MODE_PWM,SW_LE32(1000000 / frequency_Hz),0x55 };
		_sw.sendPacket(tx);

	}

    /*!
    \brief Set the PWM period on a Serial Wombat 18AB chip's PWM
 
 \param period_uS  Period in microseconds.  Note that actual period may vary based on hardware capabilities of the pin.
	 */
    void writePeriod_uS(uint32_t period_uS)
	{
		uint8_t tx[] = { 220,_pin,PIN_MODE_PWM,SW_LE32(period_uS),0x55 };
		_sw.sendPacket(tx);
	}

    /*!
    \brief fulfills a virtual function requirement of SerialWombatAbstractScaledOutput
    \return current pin number
    */
    uint8_t pin()
{
	return SerialWombatPin::_pin;
}
    /*!
    \brief fulfills a virtual function requirement of SerialWombatAbstractScaledOutput
    \return current pin mode number
    */
    uint8_t swPinModeNumber()
	{
		return SerialWombatPin::_pinMode;
	}
};

