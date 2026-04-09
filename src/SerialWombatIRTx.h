#pragma once
#include "Stream.h"
#include "SerialWombat.h"
/*! \file SerialWombatUART.h
*/

/*
Copyright 2026 Broadwell Consulting Inc.

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

/*! @brief A class for the Serial Wombat 8B or SW18AB chips which allows sending NEC IR Commands

This class sends NEC IR commands with a 16 bit address, and optional repeats.  It provides the high
level protocol, but not the 38kHz carrier.  The carrier needs to be combined with the high level
protocol in order to transmit IR signals.  This can be achieved with an AND logic chip, or by connecting
an an IR LED and resistor to two pins where current flows from the protocol pin into the carrier pin.

The Serial Wombat 18AB chip can generate the carrier frequency on any enchanced digital capability 
pin using the SerialWombatFrequencyOutput pin mode.  The Serial Wombat 8B chip can generate a 38kHz carrier
on WP6 by using an internal peripheral.  

This pin mode works very well witht the SerialWombat 8B REMCON PCB0041 which has the protocol pin on WP7 and
the carrier pin on WP6, connected to an AND chip which feeds a transistor driven high power LED.

See the Examples in the examples area for usage.

A Tutorial video is avaialble:

@htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
@endhtmlonly

https://youtu.be/TODO


The class inherits from the Arduino Sream class, so functions such as write 
can be used once the class is initialized.

*/

class SerialWombatIRTx :
    public Stream, public SerialWombatPin
{
public:
	/*!
    @brief Constructor for the SerialWombatIRTx class.  
    @param serialWombat The Serial Wombat chip on which the SerialWombatIRTx instance will run.
    */
    SerialWombatIRTx(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat){}
   

    /*!
    @brief Initalize the SerialWombatIRTx.  
@param pin  The pin that will host the state machine. For the REMCON board thsi should be 7 
@param irMode Reserved for possible future support of non NEC protocols
@return negative error code or positive success
*/
    int16_t begin( uint8_t pin, uint16_t address , uint8_t irMode = 0) 
	{
		(void) irMode;
		_pin = pin;
			_pinMode = PIN_MODE_IRTX;
		_address = address;
		uint8_t tx[8] = { 200, _pin,_pinMode, 0,0x55,0x55,0x55, 0x55 };
		uint8_t rx[8];
		return _sw.sendPacket(tx, rx);

	}

    /*!
    @brief Causes pin WP6 on the Serial Wombat 8B to generate a 38kHz carrier wave

	This can only be turned off by resetting the chip.  No effect on SW18AB.  Use the frequency output function on that chip.
	*/ 
	int16_t enableSW8b38KHzWP6()
{
		uint8_t tx[8] = { 220, _pin,_pinMode,0x55,0x55,0x55, 0x55,0x55 };
		uint8_t rx[8];
		return _sw.sendPacket(tx, rx);

}
/*!
    @brief Write a byte to the SerialWombatIRTx for Transmit as a command using the prior specified address
    @param data  Byte to write
    @return Number of bytes written
    */
    int sendMessage(uint8_t command, int32_t address = -1, uint8_t repeat = 0)
{
	uint8_t peektx[8] = { 203, _pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
	uint8_t peekrx[8];
	_sw.sendPacket(peektx, peekrx);
	if (address >= 0)
	{
	_address = (uint16_t)address;
	}

	if (peekrx[3] >= 4)
	{

		uint8_t tx[8] = { 201, _pin,_pinMode,4,(uint8_t)(_address &0xFF),(uint8_t)(_address >>8),command,repeat };  
		uint8_t rx[8];
		_sw.sendPacket(tx,rx);
		return (1);
	}
	else
	{
		return -1;
	}
}
   	/*!
    @brief This function exists to satisfy stream requirements, and always returns 0.
    @return -1
    */
    virtual int available()
{
	return(0);
}
	/*!
    @brief This function exists to satisfy stream requirements, and always returns  -1.
    @return -1
    */
    virtual int read()
{
		return -1;
}
/*!
    @brief  Discard all received bytes.  Does nothing, exists to satisfy stream requirements.
   */
    virtual void flush()
{
}
/*!
    @brief This function exists to satisfy stream requirements, and always returns  -1.
    @return -1
    */
    virtual int peek()
{
		return (-1);
}
/*!
    @brief Write a byte to the SerialWombatIRTx for Transmit as a command using the prior specified address
    @param data  Byte to write
    @return Number of bytes written
    */
    virtual size_t write(uint8_t data)
{
	if (sendMessage(data) > 0)
	{
		return (1);
	}
	else
	{
		return 0;
	}
}

/*!
    @brief Write bytes to the SerialWombatUART for Transmit
    @param buffer  An array of uint8_t bytes to send
    @param size the number of bytes to send
    @return the number of bytes sent
    
    This function queries the SerialWombatUART for avaialble TX
    buffer space, and sends bytes as buffer space is avaialble.
    If avaialable buffer space is not sufficient to send the entire
    array then the function will block and continue trying until the
    entire message has been sent to the SerialWombatUART transmit queue.
    */
    virtual size_t write(const uint8_t* buffer, size_t size)
{
	size_t bytesSent;
	
	for (bytesSent = 0; bytesSent  < size ;)
	{
		size_t result = write(buffer[bytesSent]);
		if (result < 1)
		{
			return (bytesSent);
		}

	}
	return (bytesSent);
}
	/*!
    @brief Queries the SerialWombatUART for the amount of free TX queue space
    @return A value between 0 and 64 for the SW4B
	*/
    virtual int availableForWrite()
{
	uint8_t peektx[8] = { 203, _pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
	uint8_t peekrx[8];
	_sw.sendPacket(peektx, peekrx);
	return peekrx[3];
}
/*!
    @brief This function exists to satisfy stream requirements, and always returns  0.
    @return 0 
    */
    virtual size_t readBytes(char* buffer, size_t length)
{
	(void) buffer;
	(void) length;
	return (0);
}


/*!
    @brief This function exists to satisfy stream requirements
    */
    virtual void setTimeout(long timeout_mS)
{
	(void) timeout_mS;
}
protected:
	uint16_t _address;
};


