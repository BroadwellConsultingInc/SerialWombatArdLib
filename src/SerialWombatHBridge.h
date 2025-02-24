#pragma once
/*
Copyright 2024 Broadwell Consulting Inc.

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
/*! \file SerialWombatHBridge.h
*/

typedef enum 
{
        HBRIDGE_OFF_BOTH_LOW = 0,
        HBRIDGE_OFF_BOTH_HIGH = 1,
        HBRIDGE_RELAY_AND_PWM = 2,
}SerialWombatHBridgeDriverMode;

/*!
\brief A class representing a Serial Wombat H Bridge Output

An instance of this class should be declared for each pair of pins
to be used as a Serial Wombat H Bridge.  

*/

class SerialWombatHBridge : public SerialWombatPin
{
public:
	/*!
    \brief Constructor for SerialWombatHBridge class
    \param serialWombat SerialWombat  chip on which the PWM will run
    */
    SerialWombatHBridge(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat) {}

	/*!
    \brief Initialize a pin that has been declared as HBridge. 
   \param pin The pin to become the first pin of the HBridge control.  
   \param secondPin The 2nd pin to become the first pin of the HBridge control.  
   \param PWMPeriod_uS A value  representing the period of the  PWM duty cycle in uS
   \param chip   The Driver chip being driven.  
   */
    int16_t begin(uint8_t pin, uint8_t secondPin, uint16_t PWMPeriod_uS = 1000,SerialWombatHBridgeDriverMode driverMode = HBRIDGE_OFF_BOTH_LOW)
	{
		_pin = pin;
		_pinMode = (uint8_t)PIN_MODE_HBRIDGE;
	    int16_t result = initPacketNoResponse(0,secondPin,(uint8_t) driverMode);
	    if (result < 0) return result;
            uint8_t tx2[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE_HW_0,_pin,PIN_MODE_HBRIDGE,SW_LE16(PWMPeriod_uS),0x55,0x55,0x55 };
            _sw.sendPacket(tx2);
	    return initPacketNoResponse(20,PWMPeriod_uS);
	}


private:
};


/// \brief Extends the SerialWombatHBridge class with SW18AB specific functionality, including SerialWombatAbstractScaledOutput
class SerialWombatHBridge_18AB: public SerialWombatHBridge, public SerialWombatAbstractScaledOutput
{
public:
    SerialWombatHBridge_18AB(SerialWombatChip& serialWombat) :SerialWombatHBridge(serialWombat), SerialWombatAbstractScaledOutput(serialWombat)
    {}


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

