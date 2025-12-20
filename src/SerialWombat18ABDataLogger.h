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


enum class DataLoggerPeriod {
PERIOD_1024mS = 10,
        PERIOD_512mS = 9,
        PERIOD_256mS = 8,
        PERIOD_128mS = 7,
        PERIOD_64mS = 6,
        PERIOD_32mS = 5,
        PERIOD_16mS = 4,
        PERIOD_8mS = 3,
        PERIOD_4mS = 2,
        PERIOD_2mS = 1,
        PERIOD_1mS = 0
};

/*!
 \brief A Class representing the Serial Wombat 18AB Data Logger module
*/


class SerialWombat18ABDataLogger 
{
public:
	/*!
	/// \brief Constructor for SerialWombat18ABDataLogger class
   /// \param serialWombat SerialWombat chip on which the driver will run
        */
	SerialWombat18ABDataLogger(SerialWombatChip& serialWombat):_sw(serialWombat)
	{
		_sw = serialWombat;
	}

	/*!
	/// \brief Initialize the Serial Wombat Data logger.  It will create a queue with the given parameters
    ///
    /// \param queueAddress  Index in User Ram area of the Queue to be created
    /// \param queueSizeBytes The length in bytes of avaialble queue space
    /// \param queueFrameIndex Whether or not to queue the 16 bit frame number before each queue entry
    /// \param queueOnChange  Whether the queue queues on data change (true) or based on time (false)
    /// \param period The time between data logger entries if queueOnChange is false
    /// \return A zero positive number on success or  a negative number indicating an error code.
    	*/
  	int16_t begin(uint16_t queueAddress, uint16_t queueSizeBytes, bool queueFrameIndex, bool queueOnChange = false, DataLoggerPeriod period = DataLoggerPeriod::PERIOD_1mS)
{
	SerialWombatQueue swq(_sw);
	int16_t result = swq.begin(queueAddress,queueSizeBytes);
	 if (result < 0) return (result);
            {
                uint8_t tx[] = {(uint8_t)SerialWombatCommands::COMMAND_BINARY_CONFIG_DATALOGGER,
                0, //Initial Config
                SW_LE16(queueAddress),
                (uint8_t)(period),
                queueFrameIndex?(uint8_t)1:(uint8_t)0,
                queueOnChange?(uint8_t)1:(uint8_t)0,
                };
                return(_sw.sendPacket(tx));
            }
}

/*!
@brief Enable or disable the Data Logger
        @param enable True to enable the Data Logger, False to disable it
        @return 0 on success or a negative number indicating an error code.

        This function enables or disables the Data Logger.  Enable should be called after
        begin() has been called to start logging data.
        */

 int16_t enable(bool enable = true)
        {
            uint8_t tx[] = {(uint8_t)SerialWombatCommands::COMMAND_BINARY_CONFIG_DATALOGGER,
                1, //Logger total Enable / Disable
                enable?(uint8_t)1:(uint8_t)0,
                };
            return (_sw.sendPacket(tx));
        }

        /*@
        @brief Configure an individual pin to be logged by the Data Logger
        @param pin The Serial Wombat pin number to be configured    
        @param queueLowByte True to queue the low byte of the pin's value, False to not queue it
        @param queueHighByte True to queue the high byte of the pin's value, False to not queue it
        @return 0 on success or a negative number indicating an error code.
        */
        int16_t configurePin(uint8_t pin, bool queueLowByte, bool queueHighByte)
        {
            uint8_t tx[] = {(uint8_t)SerialWombatCommands::COMMAND_BINARY_CONFIG_DATALOGGER,
                2, //Configure individual pins
                pin,
                queueLowByte?(uint8_t)1:(uint8_t)0,
                                queueHighByte?(uint8_t)1:(uint8_t)0,
            };
            return (_sw.sendPacket(tx));
        }
private:
    SerialWombatChip& _sw;
	
};
