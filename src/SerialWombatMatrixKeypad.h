#pragma once

/*
Copyright 2021-2025 Broadwell Consulting Inc.

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
/*! @file SerialWombatMatrixKeypad.h
*/
/*! @brief A class for the Serial Wombat SW18AB chips which scans matrix keypads up to 4x4



A Tutorial video is avaialble:

https://youtu.be/hxLda6lBWNg
@htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/hxLda6lBWNg" 
title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; 
clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
@endhtmlonly



The class inherits from the Arduino Stream class, so queued kepad presses can be read like a
Serial port.

This class allows the user to declare up to 4 row and 4 column pins which are strobed continuously
to read up to 16 buttons.  The Serial Wombat chip's internal pull-up resistors are used so no additional
hardware is necesary.  Standard matrix keypads can be attached directly to the Serial Wombat chip pins.
All Serial Wombat 18AB chip pins can be used in any combination or order.

Results can be returned to the host as a binary 16 bit number indicating the state of 16 buttons, 
as an index indicating which button is currently pressed (0 for Col 0 Row 0,  3 for Col 3 Row 3 and
12 for Col 0 Row 3, or as ASCII values which assume a standard keypad layout.

Index mode:

    |0  1  2  3 |
    |4  5  6  7 |
    |8  9  10 11|
    |12 13 14 15|
    With 16 being used for no current press, depending on mode setting

Ascii Mode:

    |1 2 3 A|
    |4 5 6 B|
    |7 8 9 C|
    |* 0 # D|

Note that the key indexes remain the same regardless of how many rows and columns are enabled.

The Serial Wombat 18AB firmware also keeps track of button transition counts and time since last
transition for all 16 buttons.  In this way each key of the keypad can be treated equivalently to
a SerialWombatDebouncedInput class when encapsulated in a SerialWombatMatrixInput class.  See the
documentation on this class and Arduino examples for details.

The 16 Bit public data presented internally to other Serial Wombat pins and through the SerialWombatChip.readPublicData
method can be configured to present the binary state of 16 buttons, the last button index pressed, 
the last button index pressed or 16 if no button is pressed, or ASCII of last button pressed.

*/

class SerialWombatMatrixKeypad :
    public Stream
{
public:
	/*!
    @brief Constructor for the SerialWombatMatrixKeypad class.  
    @param serialWombat The Serial Wombat Chip on which the SerialWombatUART instance will run.
    	*/
    SerialWombatMatrixKeypad(SerialWombatChip& serialWombat):_sw(serialWombat)
	{
		_sw = serialWombat;
	}
    /*!
    @brief Initalize the SerialWombatMatrixKeypad.  
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
    int16_t begin(uint8_t controlPin, 
        uint8_t row0pin, uint8_t row1pin, uint8_t row2pin, uint8_t row3pin, 
        uint8_t column0pin, uint8_t column1pin, uint8_t column2pin, uint8_t column3pin, 
        uint8_t bufferMode = 0, uint8_t queueMode = 1, uint8_t rowTiming = 5)
	{
		_pin = controlPin;

		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
							_pin,
							(uint8_t)PIN_MODE_MATRIX_KEYPAD ,
							row0pin,
							row1pin,
							row2pin,
							row3pin,
							column0pin };
		int16_t result = _sw.sendPacket(tx);
		if (result < 0)
		{
			return result;
		}

		uint8_t tx5[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE5,
							_pin,
							(uint8_t)PIN_MODE_MATRIX_KEYPAD ,
							column1pin,
							column2pin,
							column3pin,
							bufferMode,
							queueMode };
		result =  _sw.sendPacket(tx5);
		if (result < 0)
		{
			return result;
		}
		uint8_t tx8[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE8,
							_pin,
							(uint8_t)PIN_MODE_MATRIX_KEYPAD ,
rowTiming, 
0x55,
0x55,
0x55,
0x55
							};
		return _sw.sendPacket(tx8);
	}
    

    /*!
    @brief Set a binary mask for which keys are added to Queue
    
    This commands allows exclusion of keys from being queued.  This
    can be useful if, for instance, only the numeric keys of a keypad
    are to be added to the queue (excluding #,*, ABCD, etc) 
    
    @param mask  A 16 bit bitmap where a 1 allows queuing of that key index 
    and a 0 does not.  Index 0 is LSB.  For instance, for a typical Phone/ABCD
    keypad, a mask of 0x2777 would allow the numeric keys to be added to the 
    queue but would exclude ABCD#* .
    @return Returns 0 or higher if successfully set or a negative error code otherwise.
    */
    int16_t writeQueueMask(uint16_t mask)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE7,
			_pin, (uint8_t)PIN_MODE_MATRIX_KEYPAD,
			SW_LE16(mask),0x55,0x55,0x55 };
		return _sw.sendPacket(tx);
	}

    /*!
    @brief  Change the default ASCII output for each key
   
	By default the keypad outputs 123A / 456B / 789C / *0#D
	This can be changed by calling this function.  Each call
	sets one byte in a 16 byte array changing the output.
	For example, the pin which normally outputs B could be made to
	output 'G' by calling this function with an index 7 and a value
	(byte)'G'.

	This function would be called 16 times to configure the whole table
    
    @param tableIndex A value from 0 to 15 indicating the key index .
    @param asciiValue  The Ascii Vaule to store in the table
    @return Returns 0 or higher if successfully set or a negative error code otherwise.
    */
    int16_t writeAsciiTable(uint8_t tableIndex, uint8_t asciiValue )
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE9,
			_pin, (uint8_t)PIN_MODE_MATRIX_KEYPAD,
			tableIndex,asciiValue,0x55,0x55,0x55};
		return _sw.sendPacket(tx);
	}
    /*!
    @brief Queries the SerialWombatMatrixKeypad for number bytes available to read
    @return Number of bytes available to read.
    */
    int available()
	{
		uint8_t tx[8] = { 201, _pin, (uint8_t)PIN_MODE_MATRIX_KEYPAD, 0,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);
		return (rx[4]);
	}

    /*!
    @brief Reads a byte from the SerialWombatMatrixKeypad queue
    @return A byte from 0-255, or -1 if no bytes were avaialble
    */
    int read()
	{
		uint8_t tx[8] = { 202, _pin,(uint8_t)PIN_MODE_MATRIX_KEYPAD, 1,0x55,0x55,0x55,0x55 };
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
    /// @brief  Discard all bytes from the SerialWombatMatrixKeypad queue
    void flush()
	{
	//TODO	
	}
    /*!
    \brief Query the SerialWombatMatrixKeypad queue for the next avaialble byte, but don't remove it from the queue
    \return A byte from 0-255, or -1 if no bytes were avaialble
    */
    int peek()
	{
		uint8_t tx[8] = { 203, _pin,(uint8_t)PIN_MODE_MATRIX_KEYPAD,0x55,0x55,0x55,0x55,0x55 };
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
    @brief Write a byte to the SerialWombatMatrixKeypad queue  (Does Nothing)
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
    @brief Write bytes to the SerialWombatMatrixKeypad queue (Does nothing)
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
    @brief Number of bytes avaialble to write to SerialWombatMatrixKeypad queue.  Returns 0
    @return Zero.  Writes are not suppored.
    */
    int availableForWrite()
	{
		return(0);
	}

    /*!
    @brief Reads a specified number of bytes from the SerialWombatMatrixKeypad queue queue
    @param buffer  An array into which to put received bytes
    @param length  The maximum number of bytes to be received
    @return the number of bytes written to buffer
    
    This function will read bytes from the SerialWombatMatrixKeypad queue into buffer.
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

			uint8_t tx[8] = { 202, _pin,(uint8_t)PIN_MODE_MATRIX_KEYPAD, (uint8_t)bytecount,0x55,0x55,0x55,0x55 };
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
    SerialWombatMatrixKeypad operator=(SerialWombatMatrixKeypad& kp)
	{
		return kp;
	}


     SerialWombatChip& _sw;
     uint8_t _pin = 255;
protected:
     uint32_t timeout = 1;
};


/*!
@brief Class that runs on top of SerialWombatMatrixKeypad to treat a key as an individual button

This class allows a single key from a SerialWombatMatrixKeypad to be treated as an individual SerialWombatAbstractButton
that can be read as such or passed to SerialWombatButtonCounter .  
*/
class SerialWombatMatrixButton : public SerialWombatAbstractButton
{
public:
	/*!
    @brief Instantiate a SerialWombatMatrixButton
    
    @param kp An initialized SerialWombatMatrixKeypad
    @param keyIndex a number 0-15 indicating which key (index, not ascii value) is treated as a button
    */
    SerialWombatMatrixButton(SerialWombatMatrixKeypad& kp, uint8_t keyIndex):SerialWombatAbstractButton(),_keypad(kp)
	{
		_keypad = kp;
		_keyIndex = keyIndex;
	}

    /*!
    @brief Returns the  state of the input
    
    This function reads from the public data of the pin which 
    indicates the state of the
    input
    @return TRUE for pressed or FALSE.  
    */
    bool digitalRead()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
					_keypad._pin,
					PIN_MODE_MATRIX_KEYPAD,
					0,
					_keyIndex,0x55,0x55,0x55 };
	uint8_t rx[8];
	int result = _keypad._sw.sendPacket(tx, rx);
	if (result >= 0)
	{
		transitions = rx[4] + 256 * rx[5];

			return (rx[3] > 0);
	}
	return(0);
}
    /*!
    @brief return the number of mS that the button has been in false state
 
 
 @return returns a value in mS which saturates at 65535.  Returns 0 if currently true.
 	*/
    uint16_t readDurationInFalseState_mS()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
					_keypad._pin,
					PIN_MODE_MATRIX_KEYPAD,
					0,
					_keyIndex,0x55,0x55,0x55 };
	uint8_t rx[8];
	int result = _keypad._sw.sendPacket(tx, rx);
	if (result >= 0)
	{
		transitions = rx[4] + 256 * rx[5];
		uint16_t time = rx[6] + 256 * rx[7];
		if (rx[3] == 0)
		{
			return(time);
		}	
	}
	return(0);
}

    /*!
    @brief return the number of mS that the button has been in true state
    
    @return returns a value in mS which saturates at 65535.  Returns 0 if currently false.
    */
    uint16_t readDurationInTrueState_mS()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
					_keypad._pin,
					PIN_MODE_MATRIX_KEYPAD,
					0,
					_keyIndex,0x55,0x55,0x55 };
	uint8_t rx[8];
	int result = _keypad._sw.sendPacket(tx, rx);
	if (result >= 0)
	{
		transitions = rx[4] + 256 * rx[5];
		uint16_t time = rx[6] + 256 * rx[7];
		if (rx[3] == 1)
		{
			return(time);
		}
	}
	return(0);
}

    /*!
    @brief Queries the number of transistions that have occured on the button
    
    This function queries the button for current state and transitions since last call.
    transition count is put in the global member transitions.  The keypad driver in the Serial
    Wombat chip resets its count to zero after this call.
    
    @return TRUE or FALSE, current status of debounced input
    */
    bool readTransitionsState(bool resetTransitionCount = true)
{
	return digitalRead();
}

private:
    SerialWombatMatrixKeypad& _keypad;
    uint8_t _keyIndex;
};

