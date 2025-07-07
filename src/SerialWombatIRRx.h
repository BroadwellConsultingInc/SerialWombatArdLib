#pragma once

/*
Copyright 2025 Broadwell Consulting Inc.

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

#include "Stream.h"
#include "SerialWombat.h"
/*! @file SerialWombatIRRx.h
*/
/*! @brief A class for the Serial Wombat SW8B or SW18AB chips which received IR remote signals.  Currently only NEC protocol is supported.



A Tutorial video is avaialble:

COMING SOON


The class inherits from the Arduino Stream class, so queued button presses can be read like a
Serial port.


*/

class SerialWombatIRRx :
    public Stream, public SerialWombatPin
{
	public:
		enum publicDataOutput {
			COMMAND = 0, ///<  The last 8 bit command received (address filtered if enabled)
			ADDRESS = 1, ///< the last 16 bit address received.  Not Address filtered 
			DATACOUNT = 2, ///< The number of data that have been queued (Affected by address filtering and repeat settings)
		};


		/*!
		  @brief Constructor for the SerialWombatIRRx class.  
		  @param serialWombat The Serial Wombat Chip on which the SerialWombatUART instance will run.
		  */
		SerialWombatIRRx(SerialWombatChip &sw ):SerialWombatPin(sw)
	{
	}
		/*!
		  @brief Initalize the SerialWombatIRRx.  
		  @param controlPin Keypad scanning transitions will occur while this pin is being serviced by the Serial Wombat executive.  Typically this will be the same as the row0 pin
		  @param row0pin pin attached to the topmost keypad row.  On many marked keypads this row has 1,2,3 and A in it.  Enter 255 if this column is unused
		  @param row1pin pin attached to the topcenter keypad row.  On many marked keypads this row has 4,5,6 and B in it.  Enter 255 if this row is unused
		  @param row2pin pin attached to the topmost keypad row.  On many marked keypads this row has 7,8,9 and C in it.  Enter 255 if this row is unused
		  @param row3pin pin attached to the topmost keypad row.  On many marked keypads this row has *,0,# and D in it.  Enter 255 if this row is unused
		  @param column0pin pin attached to the leftmost keypad column.  On many marked keypads this column has 1,4,7 and * in it.  Enter 255 if this column is unused
		  @param column1pin pin attached to the leftcenter keypad column.  On many marked keypads this column has 1,5,8 and 0 in it. Enter 255 if this column is unused
		  @param column2pin pin attached to the rightcenter keypad column.  On many marked keypads this column has 3,5,9 and # in it. Enter 255 if this column is unused
		  @param column3pin pin attached to the rightmost keypad column.  On many marked keypads this column has A,B,C and D in it. Enter 255 if this column is unused
		  @param bufferMode 0: Public data is Binary of 16 keys (Default)  1:  Public data is last key index pressed  2:  Public data is last key pressed or 16 for no key index  3: Public data is Ascii of last key pressed 
		  @param queueMode 0: Button presses are queued as indexes 1: Button Presses are queued as ASCII
		  @param rowTiming  mS to delay after setting a pin row low before reading columns
		  */
		int16_t begin(
				uint8_t pin, uint8_t irMode = 0, bool useRepeat = true, SerialWombatPinState_t activeState = SW_LOW,
				uint16_t publicDataTimeoutPeriod_mS = 1000, uint16_t publicDataTimeoutValue = 0xFFFF, bool useAddressFilter = false, uint16_t addressFilterValue = 0x1234,
				SerialWombatIRRx::publicDataOutput dataOutput = SerialWombatIRRx::publicDataOutput::COMMAND )
		{
			_pinMode = (uint8_t)PIN_MODE_IRRX;
			_pin = pin;
			int16_t returnValue = 0;

			{
				uint8_t tx[] = { 200, _pin, _pinMode, irMode,
					useRepeat ? (uint8_t)1 : (uint8_t)0,
					(uint8_t)activeState, 
					(uint8_t)(addressFilterValue & 0xFF), (uint8_t)((addressFilterValue >> 8) & 0xFF), };
				returnValue = _sw.sendPacket(tx);
				if (returnValue < 0)
				{
					return returnValue;
				}
			}
			{
				uint8_t tx[] { 201, _pin, _pinMode, (uint8_t)(publicDataTimeoutPeriod_mS & 0xFF), (uint8_t)((publicDataTimeoutPeriod_mS >> 8) & 0xFF),
					(uint8_t)(publicDataTimeoutValue & 0xFF), (uint8_t)((publicDataTimeoutValue >> 8) & 0xFF),  useAddressFilter ? (uint8_t)1 : (uint8_t)0
				};

				returnValue = _sw.sendPacket(tx);
				if (returnValue < 0)
				{
					return returnValue;
				}
			}
			{
				uint8_t tx[] { 205, _pin, _pinMode, (uint8_t)dataOutput,
					0x55,0x55,0x55,0x55};

				returnValue = _sw.sendPacket(tx);
				//if (returnValue < 0)
				{
					return returnValue;
				}
			}

		}


		int available()
		{
			uint8_t tx[8] = { 201, _pin, _pinMode, 0,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			_sw.sendPacket(tx, rx);
			return (rx[4]);
		}

		/*!
		  @brief Reads a byte from the SerialWombatIRRx queue
		  @return A byte from 0-255, or -1 if no bytes were avaialble
		  */
		int read()
		{
			uint8_t tx[8] = { 202, _pin,_pinMode, 1,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			if (_sw.sendPacket(tx, rx) < 0)
			{
				return -1;
			}

			if (rx[3] != 0)
			{
				return (rx[4]);
			}
			else
			{
				return (-1);
			}
		}
		/// @brief  Discard all bytes from the SerialWombatIRRx queue
		void flush()
		{
			//TODO	
		}
		/*!
		  \brief Query the SerialWombatIRRx queue for the next avaialble byte, but don't remove it from the queue
		  \return A byte from 0-255, or -1 if no bytes were avaialble
		  */
		int peek()
		{
			uint8_t tx[8] = { 203, _pin,_pinMode,0x55,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			_sw.sendPacket(tx, rx);
			if (rx[4] > 0)
			{
				return (rx[5]);
			}
			else
			{
				return (-1);
			}
		}

		/*!
		  @brief Write a byte to the SerialWombatIRRx queue  (Does Nothing)
		  @param data  Byte to write
		  @return Number of bytes written

		  This function exists to fully implement the Stream class.  It throws away the byte.
		  */
		size_t write(uint8_t data)
		{
			(void)data; // Avoid compiler warning about unused parameter

			return (1);
		}

		/*!
		  @brief Write bytes to the SerialWombatIRRx queue (Does nothing)
		  @param buffer  An array of uint8_t bytes to send
		  @param size the number of bytes to send
		  @return the number of bytes sent

		  This function exists to fully implement the Stream class.  It throws away the bytes.
		  */
		size_t write(const uint8_t* buffer, size_t size)
		{
			(void)buffer; // Avoid compiler warning about unused parameter
			return(size);
		}

		/*!
		  @brief Number of bytes avaialble to write to SerialWombatIRRx queue.  Returns 0
		  @return Zero.  Writes are not suppored.
		  */
		int availableForWrite()
		{
			return(0);
		}

		/*!
		  @brief Reads a specified number of bytes from the SerialWombatIRRx queue queue
		  @param buffer  An array into which to put received bytes
		  @param length  The maximum number of bytes to be received
		  @return the number of bytes written to buffer

		  This function will read bytes from the SerialWombatIRRx queue into buffer.
		  If 'length' characters are not available to read then the value returned
		  will be less than length.
		  */
		size_t readBytes(char* buffer, size_t length)
		{
			int index = 0;
			int bytesAvailable = 0;
			uint32_t timeoutMillis = millis() + timeout;
			while (length > 0 && timeoutMillis > millis())
			{
				int bytecount = 4;
				if (length < 4)
				{
					bytecount = length;
				}
				{

					uint8_t tx[8] = { 202, _pin,_pinMode, (uint8_t)bytecount,0x55,0x55,0x55,0x55 };
					uint8_t rx[8];
					_sw.sendPacket(tx, rx);
					bytesAvailable = rx[3];

					if (bytesAvailable == 0)
					{
						continue;
					}
					else
					{
						timeoutMillis = millis() + timeout;
					}
					uint8_t bytesReturned = bytecount;
					if (rx[3] < bytecount)
					{
						bytesReturned = rx[3];
					}
					for (int i = 0; i < bytesReturned; ++i)
					{
						buffer[index] = rx[i + 4];
						++index;
						--bytesAvailable;
						--length;

					}
				}

			}
			return (index);
		}

		/*!
		  @brief implemented to fulfill Stream requirement.
		  */
		void setTimeout(long timeout_mS)
		{
			if (timeout_mS == 0)
			{
				timeout = 0x80000000;
			}
			else
			{
				timeout = timeout_mS;
			}
		}


		/*!
		  @brief used to allow reference copy.  Not for user use.
		  */
		SerialWombatIRRx operator=(SerialWombatIRRx& irrx)
		{
			return irrx;
		}


		uint16_t readAddress()
		{
			uint8_t tx[8] = { 204, _pin,_pinMode, 0x55,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			_sw.sendPacket(tx, rx);
			uint16_t returnVal = rx[4];
			returnVal <<= 8;
			returnVal|= rx[3];
			return returnVal;
		}

		uint16_t readDataCount()
		{
			uint8_t tx[8] = { 204, _pin,_pinMode, 0x55,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			_sw.sendPacket(tx, rx);
			uint16_t returnVal = rx[6];
			returnVal <<= 8;
			returnVal|= rx[5];
			return returnVal;
		}
	protected:
		uint32_t timeout = 1;
};



