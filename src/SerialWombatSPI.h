#pragma once
#include "SerialWombat.h"
/*! \file SerialWombatSPI.h
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

// Define values the same as in Arduino SPI.

#ifndef MSBFIRST
#define MSBFIRST 1
#endif
// Don't define LSB first because it's not supported.  


#ifndef SPI_MODE0
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE3 0x0C
#endif


/*! @brief A class for storing SPI settings.  This isn't actually used, but included for congruency with Arduino */
class SerialWombatSPISettings {
public:
  SerialWombatSPISettings(uint32_t clock = 0, uint8_t bitOrder = MSBFIRST, uint8_t dataMode = SPI_MODE0) {
	(void)clock; // Clock is not configurable, so ignore this parameter.
	(void)bitOrder; // Bit order is not configurable, so ignore this parameter.		
	(void)dataMode;
  }
};

/*! @brief A pin mode for the Serial Wombat 8B or SW18AB chips which allows SPI communication over I2C

This pin mode allows the Serial Wombat 8B or SW18AB to send and receive SPI data over I2C.  This pin mode is good for low
to moderate bandwidth SPI devices due to the overhead of the I2C bus.  Suggested applications include shfit registers,
low speed ADC, and LED current drivers.  This pin mode is intended as a much more efficient alternative to bit-banging
SPI using pin high and low I2C commands.

This pin mode is assigned to the clock pin.  Additonal pins can be assigned as MOSI, MISO and CS pins.  
The clock speed is not configurable, and is about 250kHz on the SW8B.  

Unlike other pin modes, all functionality of this pin mode occurs during the I2C communcation handling instead of the
1mS pin update.   This is not a problem for the SW8B because it only has to service 6 or less other pins every 1mS, but
for the 18AB can be problematic if many other pins are in use as the time to bit-bang the SPI transaction may cause
frame overflows.  For the 8B up to 5 SPI bytes can be transferred per I2C transaction, but for the 18AB only 1 byte
can be transferred by default to minimize the chance of frame overflows.  

SPI modes 0, 1, and 3 are supported.  Mode 2 is not a commonly used mode, and is left out to reduce the flash space
required by the pin mode.

If a chip select pin is specified the chip select is set low before the SPI transaction.  The chip select can be
configured to stay low or go high after completion of the SPI transaction allowing for long transactions.

This pin mode is designed to function similarly to the Arduino SPIClass with similar interfaces.  Unlike Arduino
there are no interupt based options, the clock speed is fixed, and the bitOrder is ignored (always MSB first).

See the Examples in the examples area for usage.

A Tutorial video is avaialble:

@htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
@endhtmlonly

https://youtu.be/TODO


The class inherits from the Arduino Sream class, so functions such as write 
can be used once the class is initialized.

*/

class SerialWombatSPI :  public SerialWombatPin
{
public:
	/*!
    @brief Constructor for the SerialWombatSPI class.  
    @param serialWombat The Serial Wombat chip on which the SerialWombatSPI instance will run.
    */
    SerialWombatSPI(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat){}
   

    /*!
    @brief Initalize the SerialWombatSPI.  
@param pin  The pin that will host the state machine.  
@param SPIMode 0, 1 or 3.  Mode 2 is not supported to save flash space.
@param MOSIpin Optional pin number for MOSI.  Set to 255 to not use a MOSI pin.  
@param MISOpin Optional pin number for MISO.  Set to 255 to not use a MISO pin.  
@param CSpin Optional pin number for Chip Select.  Set to 255 to not use a chip select pin.  
@return negative error code or positive success
*/
    int16_t begin( uint8_t pin,uint8_t SPIMode, uint8_t MOSIpin = 255, uint8_t MISOpin = 255, uint8_t CSpin = 255) 
	{
		
		_pin = pin;
			_pinMode = PIN_MODE_SPI;
		if (SPIMode == SPI_MODE1)
		{
			SPIMode = 1;
		}
		else if (SPIMode == SPI_MODE3)
		{
			SPIMode = 3;
		}

		if (SPIMode != 0 && SPIMode != 1 && SPIMode != 3)
		{
			return -1 * SW_ERROR_INVALID_PARAMETER_3;
		}
		uint8_t tx[8] = { 200, _pin,_pinMode, SPIMode,MOSIpin,MISOpin, CSpin, 0x55 };
		uint8_t rx[8];
		return _sw.sendPacket(tx, rx);

	}

	/* @brief Not needed for this class, included for congruency with Arduino SPIClass.  Does nothing.
	
	*/
	static void beginTransaction(SerialWombatSPISettings settings) {
    (void)settings; // Settings are not used because clock speed and bit order are fixed.
    }
   
	/* @brief Not needed for this class, included for congruency with Arduino SPIClass.  Does nothing.
	
	*/
	static void end(){}
	/*
	 @brief Write to the SPI bus (MOSI pin) and also receive (MISO pin)
	@param data The byte to send out the MOSI pin if configured.  The byte received on the MISO pin during the transaction is returned.
	@param csSaysStayLow If true, the chip select pin (if configured) will remain low after the transaction.
	@return The byte received on the MISO pin during the transaction if configured.
	*/

	uint8_t transfer(uint8_t data, bool csSaysStayLow = false) 
	{
    uint8_t tx[8] = { 201, _pin, _pinMode, 8, data,0x55, 0x55, 0x55 };
	uint8_t rx[8];

	if (csSaysStayLow)
	{
		tx[0] = 202; // Use a different command to indicate that CS should stay low after the transaction.
	}
	int16_t result = _sw.sendPacket(tx, rx);
	if (result < 0) {
	  // Handle error if needed. For now, just return 0 on error.
	  return 0;
	}
	return rx[4]; // The received byte is expected to be in rx[2].
  }

  /*
	 @brief Write to the SPI bus (MOSI pin) and also receive (MISO pin)
	@param data The word to send out the MOSI pin if configured.  The word will be send Least Significant Byte, most signficant bit first.
	@param csSaysStayLow If true, the chip select pin (if configured) will remain low after the transaction.
	@return The byte received on the MISO pin during the transaction if configured.
	*/

	uint16_t transfer(uint16_t data, bool csSaysStayLow = false) {
    uint8_t tx[8] = { 201, _pin, _pinMode, 16, SW_LE16(data), 0x55 };
	uint8_t rx[8];

	if (csSaysStayLow)
	{
		tx[0] = 202; // Use a different command to indicate that CS should stay low after the transaction.
	}
	int16_t result = _sw.sendPacket(tx, rx);
	if (result < 0) {
	  // Handle error if needed. For now, just return 0 on error.
	  return 0;
	}
	return ((((uint16_t)rx[5]) << 8) + (uint16_t)rx[4]); // The received word is expected to be in rx[4] and rx[5].
  }

  /*
	 @brief Write to the SPI bus (MOSI pin) and also receive (MISO pin)
	@param buf The array to send and receive data.  The length of the array is determined by the count parameter. 
	@param csSaysStayLow If true, the chip select pin (if configured) will remain low after the transaction.
	@return The byte received on the MISO pin during the transaction if configured.
	*/

	void transfer(void* buf, size_t count, bool csSaysStayLow = false) {

		if (_sw.isSW08())
	{
		while (count >= 5)
		{
			uint8_t tx[8] = { 204, _pin, _pinMode, ((uint8_t*)buf)[0], ((uint8_t*)buf)[1], ((uint8_t*)buf)[2], ((uint8_t*)buf)[3], ((uint8_t*)buf)[4] };
			uint8_t rx[8];

			
			count -= 5;
			if (count == 0  && 	!csSaysStayLow)  
			{
				--tx[0]; // Send a zero byte to indicate the end of the transaction and allow the chip select to go high again.
			}
			_sw.sendPacket(tx, rx);
			((uint8_t*)buf)[0] = rx[3];
			((uint8_t*)buf)[1] = rx[4];
			((uint8_t*)buf)[2] = rx[5];
			((uint8_t*)buf)[3] = rx[6];
			((uint8_t*)buf)[4] = rx[7];
			buf = (void*)((uint8_t*)buf + 5);
		}
		if (count > 0)
		{
			uint8_t tx[8] = { 201, _pin, _pinMode,(uint8_t)(count * 8), 0x55, 0x55, 0x55, 0x55 };
			for (size_t i = 0; i < count; ++i)
			{
				tx[4 + i] = ((uint8_t*)buf)[i];
			}
			uint8_t rx[8];

			if (csSaysStayLow)
			{
				tx[0] = 202; // Use a different command to indicate that CS should stay low after the transaction.
			}
			int16_t result = _sw.sendPacket(tx, rx);
			if (result < 0) {
			  // Handle error if needed. For now, just return 0 on error.
			  return;
			}
			for (size_t i = 0; i < count; ++i)
			{
				((uint8_t*)buf)[i] = rx[4 + i];
			}
		}

	}
	else // SW18AB can only transfer 1 byte at a time to minimize the chance of frame overflows.
	{
		while (count > 0)
		{
			uint8_t tx[8] = { 202, _pin, _pinMode, 8, ((uint8_t*)buf)[0],0x55, 0x55, 0x55 };
			uint8_t rx[8];

			-- count;
			if(count == 0 && !csSaysStayLow)
			{
				tx[0] = 201; 
			}
			int16_t result = _sw.sendPacket(tx, rx);
			if (result < 0) {
			  // Handle error if needed. For now, just return 0 on error.
			  return;
			}
			*(uint8_t*)buf = rx[4]; // The received byte is expected to be in rx[4].
			buf = (void*)((uint8_t*)buf + 1);
		}   
  }

};

	int16_t transferPacketUpTo32Bits(uint8_t* outBuf,uint8_t* inBuf, size_t bitCount, bool csSaysStayLow = false) {
	if (bitCount > 32) {
	  // Handle error: bitCount exceeds maximum packet size.
	  return -1; // Return an error code or handle as appropriate.
	}
	uint8_t tx[8] = { 201, _pin, _pinMode, (uint8_t)(bitCount), 0x55, 0x55, 0x55, 0x55 };
	if (csSaysStayLow) {
	  tx[0] = 202; // Use a different command to indicate that CS should stay low after the transaction.
	}
	uint8_t byteCount = (bitCount + 7) / 8; // Calculate the number of bytes needed to represent the bits.
	if (outBuf != nullptr) {
	for (size_t i = 0; i < byteCount; ++i) {
	  tx[4 + i] = outBuf[i];
	}
	}
	uint8_t rx[8];

	if (csSaysStayLow) {
	  tx[0] = 202; // Use a different command to indicate that CS should stay low after the transaction.
	}
	int16_t result = _sw.sendPacket(tx, rx);
	if (result < 0) {
	  // Handle error if needed. For now, just return on error.
	  return result;
	}
	if (inBuf != nullptr) {
	for (size_t i = 0; i < byteCount; ++i) {
	  inBuf[i] = rx[4 + i];
	}
	}
	return bitCount;

  };

  int16_t transferPacket40Bits(uint8_t* outBuf,uint8_t* inBuf, bool csSaysStayLow = false) {
	
	uint8_t tx[8] = { 203, _pin, _pinMode, 0x55, 0x55, 0x55, 0x55, 0x55 };
	if (outBuf != nullptr) {
	for (size_t i = 0; i < 5; ++i) {
	  tx[3 + i] = outBuf[i];
	}
	}
	uint8_t rx[8];

	if (csSaysStayLow) {
	  tx[0] = 204; // Use a different command to indicate that CS should stay low after the transaction.
	}
	int16_t result = _sw.sendPacket(tx, rx);
	if (result < 0) {
	  // Handle error if needed. For now, just return on error.
	  return result;
	}
	if (inBuf != nullptr) {
	for (size_t i = 0; i < 5; ++i) {
	  inBuf[i] = rx[3 + i];
	}
	}
	return 40;
};
};


