#pragma once
/*
Copyright 2020-2023 Broadwell Consulting Inc.

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
/*! \file SerialWombatProtectedOutput.h
*/


typedef enum
{
    PO_FAULT_IF_NOT_EQUAL = 0,
    PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED= 1,
    PO_FAULT_IF_FEEDBACK_GREATER_THAN_EXPECTED = 2,
} PO_COMPARE_t;


/*! \brief Combine and input and output pin for a protected output

The SerialWombtProtectedOutput class is assigned to a Serial Wombat
output pin.  It monitors another previously configured pin's public data,
such as a digital I/O value or an Analog input.  If the monitored value
does not meet expectations, then the protected pin changes values to
a configured state.   This allows the Serial Wombat chip to constantly
verify a condition without the need for constant polling from the 
host device.

\warning The SerialWombatProtectedOutput is intended to help prevent
accidental damage to hobby circuitry.  The Serial Wombat chip and its associated
libraries are not designed for use in Safety Critical applications.  The 
Serial Wombat chip should not be used in situations where a malfunction or design
defect could result in damage to property, economic loss,
or harm to living people or creatures.

The period of time that a mismatch must occur before going to the safeState
is configurable.

Video Tutorial

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/p8CO04C1q_Y" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/p8CO04C1q_Y
*/
class SerialWombatProtectedOutput : public SerialWombatPin
{
public:
    /// \brief Constructor for SerialWombatProtectedOutput
    /// \param serialWombat The serial Wombat Chip on which the Protected Output runs
    SerialWombatProtectedOutput(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat){}
    ///  Initialize the Protected Output.  No pin state changes happen with this function
    /// \param pin The output pin of the Protected output   Valid values for SW4A: 0-3  SW4B: 1-3 
    /// \param monitoredPin The input pin for the Protected Output.  Must be initialized before this call
    void begin(uint8_t pin, uint8_t monitoredPin)
	{
		_pin = pin;
		_monitoredPin = monitoredPin;
	}
    /// Configure a protected output.  This function will change the pin output to activeState unless fault conditions are met
    /// \param compareMode  Enumerated type of PO_COMPARE_t .
    /// \param compareValue  A 16 bit unsigned value that will be compared with the public data from the monitored pin and compared based on compareMode
    /// \param debounceTime How long, in mS the compareMode fault condition must exist before transitioning to safeState
    /// \param activeState The initial, pre-fault state for the output (SW_LOW, SW_HIGH, SW_INPUT).  Do not use Arduino HIGH or LOW or INPUT constants.
    /// \param safeState The safe, post-fault state for the output (SW_LOW, SW_HIGH, SW_INPUT).  Do not use Arduino HIGH or LOW or INPUT constants.
    void configure(PO_COMPARE_t compareMode, uint16_t compareValue, uint8_t debounceTime, SerialWombatPinState_t activeState, SerialWombatPinState_t safeState)
    {
	_debounceTime = debounceTime;
	_safeState = safeState;
	_compareMode = compareMode;
	_activeState = activeState;
	{
		uint8_t tx[] = { 200,_pin,PIN_MODE_PROTECTED_OUTPUT,SW_LE16(compareValue), _debounceTime,_monitoredPin,(uint8_t)_safeState };
		_sw.sendPacket(tx);
	}
	{
		uint8_t tx1[] = { 201,_pin,PIN_MODE_PROTECTED_OUTPUT,(uint8_t) _compareMode,(uint8_t)_activeState,0x55,0x55,0x55 };
		_sw.sendPacket(tx1);
	}

    }
    ///  \brief Queries the Serial Wombat to see if the protected output has entered safe mode due to a fault
    /// \return True if in safe state, false if in openState
    bool isInSafeState()
	{

		return (_sw.readPublicData(_pin) >= _debounceTime);
	}
    /// \brief Turn off protection features and make the protected pin an input
    void makeInput()
	{
		configure(PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED, 0, 100, SW_INPUT, SW_INPUT);
	}
    /// \brief Turn off the protection features and make the protected pin an unprotected output 
    /// 
    /// This function is compatible with the Arduino digital write, and therefore uses HIGH and LOW
    /// as parameters rather than SW_HIGH or SW_LOW
    /// /param state HIGH or LOW
    void digitalWrite(uint8_t state)
	{
		if (state == HIGH)
		{
			configure(PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED, 0, 100, SW_HIGH, SW_HIGH);
		}
		else if (state == LOW)
		{
			
				configure(PO_FAULT_IF_FEEDBACK_LESS_THAN_EXPECTED, 0, 100, SW_LOW, SW_LOW);
		}
	}

private:

    uint8_t _monitoredPin = 255;
    PO_COMPARE_t _compareMode = PO_FAULT_IF_NOT_EQUAL;
    uint8_t _debounceTime = 0;
    SerialWombatPinState_t _safeState = SW_INPUT;
    SerialWombatPinState_t _activeState = SW_INPUT;
    uint16_t _compareValue;
};

