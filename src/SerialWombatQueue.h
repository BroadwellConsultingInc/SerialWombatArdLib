#pragma once
/*
Copyright 2020-2021 Broadwell Consulting Inc.

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
};

/// \brief A Class representing a Queue in the User Ram area on the Serial Wombat Chip
///

class SerialWombatQueue : public Stream
{
public:
	/// \brief Constructor for SerialWombatWS2812 class
   /// \param serialWombat SerialWombat chip on which the driver will run
	SerialWombatQueue(SerialWombatChip& serialWombat);

	/// \brief Initialize a Serial Wombat Queue (RAM Bytes) in User Memory Area on Serial Wombat Chip
    ///
    /// \param index  An index in bytes for the beginning of the Queue location in Serial Wombat User Memory Area
    /// \param length The length in bytes of avaialble queue space
    /// \return A positive number indicating the number of bytes used in User Memory Area (Will be more than
    /// length due to queue management variables) or a negative number indicating an error code.
  	int16_t begin(uint16_t index, uint16_t length);

    /// \brief Queries the Serial Wombat for number bytes available to read
    /// \return Number of bytes available to read.
    int available();
    /// \brief Reads a byte from the Serial Wombat
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    int read();
    /// \brief  Discard all received bytes
    void flush();
    /// \brief Query the Serial Wombat for the next avaialble byte, but don't remove it from the queue
    /// \return A byte from 0-255, or -1 if no bytes were avaialble
    int peek();
    /// \brief Write a byte to the Serial Wombat Queue
    /// \param data  Byte to write
    /// \return Number of bytes written
   
    size_t write(uint8_t data);

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
    size_t write(const uint8_t* buffer, size_t size);

    /// \brief Queries the Serial Wombat for the amount of free queue space
    /// \return Number of bytes avaialable
    int availableForWrite();

    /// \brief Reads a specified number of bytes from the Serial Wombat Queue
    /// \param buffer  An array into which to put received bytes
    /// \param length  The maximum number of bytes to be received
    /// \return the number of bytes written to buffer
    /// 
    /// This function will read bytes from the Serial Wombat Queue into buffer.
    /// If 'length' characters are not available to read then the value returned
    /// will be less than length.
    size_t readBytes(char* buffer, size_t length);

    uint16_t startIndex = 0xFFFF;
    uint16_t length = 0;

    void setTimeout(long timeout_mS);
private:
    SerialWombatChip& _sw;

	
    uint32_t _timeout = 500;
};
