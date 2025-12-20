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

#include <stdint.h>
#include "SerialWombat.h"


enum class SerialWombatQueueType {
	QUEUE_TYPE_RAM_BYTE = 0,  ///< A queue that queues byte-sized data in a queue in the User RAM area
    QUEUE_TYPE_RAM_BYTE_SHIFT = 1,  ///< A queue that queues byte-sized data in a queue in the User RAM area
};

/*!
 \brief A Class representing a Queue in the User Ram area on the Serial Wombat Chip
*/


class SerialWombatQueue : public Stream
{
public:
	/*!
	/// \brief Constructor for SerialWombatWS2812 class
   /// \param serialWombat SerialWombat chip on which the driver will run
        */
	SerialWombatQueue(SerialWombatChip& serialWombat):_sw(serialWombat)
	{
		_sw = serialWombat;
	}

	/*!
	/// \brief Initialize a Serial Wombat Queue (RAM Bytes) in User Memory Area on Serial Wombat Chip
    ///
    /// \param index  An index in bytes for the beginning of the Queue location in Serial Wombat User Memory Area
    /// \param length The length in bytes of avaialble queue space
    /// \return A positive number indicating the number of bytes used in User Memory Area (Will be more than
    /// length due to queue management variables) or a negative number indicating an error code.
    	*/
  	int16_t begin(uint16_t index, uint16_t length, SerialWombatQueueType qtype = SerialWombatQueueType::QUEUE_TYPE_RAM_BYTE)
{
	startIndex = index;
	length = length;
	uint8_t tx[] = {(uint8_t) SerialWombatCommands::COMMAND_BINARY_QUEUE_INITIALIZE ,SW_LE16(index),SW_LE16(length),(uint8_t)qtype, 0x55,0x55 };
	uint8_t rx[8];
	int16_t result =  _sw.sendPacket(tx,rx);
	if (result < 0)
	{
		return result;
	}
	else
	{
		return ((int16_t)(rx[3] + 256 * rx[4]));
	}
}

	/*!
    /// \brief Queries the Serial Wombat for number bytes available to read
    /// \return Number of bytes available to read.
    	*/
    int available()
{
	uint8_t tx[] = {(uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult =  _sw.sendPacket(tx,rx);
	if (sendResult >= 0)
	{
		return (rx[4] + 256 * rx[5]);
	}
	return (0);
}
    /*!
    /// \brief Reads a byte from the Serial Wombat
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    */
    int read()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_READ_BYTES ,SW_LE16(startIndex),1,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		if (rx[1] == 1)
		return (rx[2]);
	}
	return (-1);
}
    /*!
    /// \brief  Discard all received bytes
    */
    void flush()
{
	begin(startIndex, length);
}
    /*!
    /// \brief Query the Serial Wombat for the next avaialble byte, but don't remove it from the queue
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    */
    int peek()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		if ((rx[4] + 256 * rx[5]) > 0)
		{
			return(rx[3]);
		}
	}
	return (-1);
}
    /*!
    /// \brief Write a byte to the Serial Wombat Queue
    /// \param data  Byte to write
    /// \return Number of bytes written
    */
   
    size_t write(uint8_t data)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_BYTES ,SW_LE16(startIndex),1,data,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		
			return(rx[3]);
	}
	return (0);
}

	/*!
	/// \brief Write an unsigned word to the Serial Wombat Queue
	/// \param data  Word to write
	/// \return Number of bytes written
	*/

	size_t write(uint16_t data)
	{
		if (availableForWrite() < 2) return 0;
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_BYTES ,SW_LE16(startIndex),2,SW_LE16(data),0x55,0x55};
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult >= 0)
		{

			return(rx[3]);
		}
		return (0);
	}
	/*!
	/// \brief Write unsigned words to the Serial Wombat Queue
	/// \param buffer  An array of uint16_t words to send
	/// \param size the number of words (not bytes) to send
	/// \return the number of words sent
	///
	/// This function queries the Serial Wombat Queue
	/// buffer space, and sends words as buffer space is avaialble.
	/// If avaialable buffer space is not sufficient to send the entire
	/// array then the function will block and continue trying until the
	/// entire message has been sent to the Serial Wombat  queue.
	*/

	size_t write(uint16_t buffer[], size_t size)
	{
		return (write((const uint8_t*)buffer, size * 2));
	}
	/*!
	/// \brief Write unsigned words to the Serial Wombat Queue
	/// \param buffer  An array of uint16_t words to send
	/// \param size the number of words (not bytes) to send
	/// \return the number of words sent
	///
	/// This function queries the Serial Wombat Queue
	/// buffer space, and sends words as buffer space is avaialble.
	/// If avaialable buffer space is not sufficient to send the entire
	/// array then the function will block and continue trying until the
	/// entire message has been sent to the Serial Wombat  queue.
	*/

	size_t write(const uint16_t* buffer, size_t size)
	{
		return (write((const uint8_t*)buffer,size*2));
	}

    /*!
    /// \brief Write bytes to the Serial Wombat Queue
    /// \param buffer  An array of uint8_t bytes to send
    /// \param size the number of bytes to send
    /// \return the number of bytes sent
    /// 
    /// This function queries the Serial Wombat Queue
    /// buffer space, and sends bytes as buffer space is avaialble.
    /// If avaialable buffer space is not sufficient to send the entire
    /// array then the function will block and continue trying until the
    /// entire message has been sent to the Serial Wombat  queue.
    */
    size_t write(const uint8_t* buffer, size_t size)
{
	uint8_t nextWriteSize;
	uint16_t bytesWritten = 0;
	uint32_t startTime = millis();

	//Write up to the first 4 bytes
	if (size >= 4)
	{
		nextWriteSize = 4;
	}
	else
	{
		nextWriteSize = (uint8_t)size;
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_BYTES ,SW_LE16(startIndex),0,0x55,0x55,0x55,0x55 };
		uint8_t i;
		for (i = 0; i < nextWriteSize; ++i)
		{
			tx[4 + i] = buffer[i];
		}
		tx[3] = nextWriteSize;
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult < 0)
		{
			return(bytesWritten);
		}
		bytesWritten += rx[3];
	}
	while ((size - bytesWritten) >= 7)
	{
		yield();
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_7BYTES ,
			buffer[bytesWritten],
			buffer[bytesWritten + 1],
			buffer[bytesWritten + 2],
			buffer[bytesWritten + 3],
			buffer[bytesWritten + 4],
			buffer[bytesWritten + 5],
			buffer[bytesWritten + 6] };

		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult < 0)
		{
			return(bytesWritten);
		}
		bytesWritten += rx[3];
		delay(0);
		if ((rx[3] == 0) && millis() >= startTime + _timeout) // Time out if couldn't write bytes and over timeout
		{
			return(bytesWritten);
		}
	}

	while (size - bytesWritten > 0)
	{
		yield();
		if (size - bytesWritten >= 4)
		{
			nextWriteSize = 4;
		}
		else
		{
			nextWriteSize = (uint8_t)(size - bytesWritten);
		}
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_BYTES ,SW_LE16(startIndex),0,0x55,0x55,0x55,0x55 };
		uint8_t i;
		for (i = 0; i < nextWriteSize; ++i)
		{
			tx[4 + i] = buffer[i + bytesWritten];
		}
		tx[3] = nextWriteSize;
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult < 0)
		{
			return(bytesWritten);
		}
		bytesWritten += rx[3];
		if ( (rx[3] == 0) && millis() >= startTime + _timeout)
		{
			return(bytesWritten);
		}
	}
	return (bytesWritten);
}

    /*!
    /// \brief Queries the Serial Wombat for the amount of free queue space
    /// \return Number of bytes avaialable
    */
    int availableForWrite()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		return (rx[6] + 256 * rx[7]);
	}
	return (0);
}

    /*!
    /// \brief Reads a specified number of bytes from the Serial Wombat Queue
    /// \param buffer  An array into which to put received bytes
    /// \param length  The maximum number of bytes to be received
    /// \return the number of bytes written to buffer
    /// 
    /// This function will read bytes from the Serial Wombat Queue into buffer.
    /// If 'length' characters are not available to read then the value returned
    /// will be less than length.
    */
    size_t readBytes(char* buffer, size_t length)
{
	uint16_t bytesAvailable = 0;
	uint32_t startTime = millis();

	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult >= 0)
		{
			bytesAvailable = (rx[4] + 256 * rx[5]);
		}

		if (bytesAvailable < length)
		{
			length = bytesAvailable;
		}
	}
	uint16_t bytesRead = 0;
	while (bytesRead < length)
	{
		yield();
		uint8_t bytesToRead =(uint8_t)( length - bytesRead);
		if (bytesToRead > 6)
		{
			bytesToRead = 6;
		}
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_READ_BYTES ,SW_LE16(startIndex),bytesToRead,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult >= 0)
		{
			uint8_t i;
				for (i = 0; i < rx[1] && bytesRead < length; ++i)
				{
					buffer[bytesRead] = rx[2 + i];
					++bytesRead;
				}
			
		}
		else
		{
			return (bytesRead);
		}
		if (millis() >= startTime + _timeout)
		{
			return(bytesRead);
		}

	}
	return bytesRead;
}


    uint16_t startIndex = 0xFFFF;
    uint16_t length = 0;

    void setTimeout(long timeout_mS)
{
	_timeout = timeout_mS;
}

	/*!
   /// \brief Reads a specified number of unsigned 16 bit words from the Serial Wombat Queue
   /// \param buffer  An array into which to put received words
   /// \param length  The maximum number of words (not bytes) to be received
   /// \return the number of words written to buffer
   ///
   /// This function will read bytes from the Serial Wombat Queue into buffer.
   /// If 'length' characters are not available to read then the value returned
   /// will be less than length.
   */
	size_t readUInt16(uint16_t* buffer, size_t length)
	{
		uint16_t bytesAvailable = 0;
	
			uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			int16_t sendResult = _sw.sendPacket(tx, rx);
			if (sendResult >= 0)
			{
				bytesAvailable = (rx[4] + 256 * rx[5]);
			}
			uint16_t wordsAvailable = bytesAvailable /2;
			if (wordsAvailable < length)
			{
				length = wordsAvailable ;
			}
			return readBytes((char*)buffer, length * 2) / 2;
	}

    //TODO add copy interface
private:
    SerialWombatChip& _sw;

	
    uint32_t _timeout = 500;
};
