#pragma once
/*
Copyright 2022-2023 Broadwell Consulting Inc.

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


/*! @file SerialWombatLiquidCrystal.h
*/

/*! @brief A Class which connects to HD44780 / 1602 / 4002 / 2004   or similar interface Character LCDs


  This pin mode has nearly identical interfaces to the classic Arduino LiquidCrystal library and can be used
  with similar parallel character LCDs.  This pin mode is only intended for use with character LCDs that are connected in
  4 bit parallel (E, RS, D4, D5, D6, D7) with the Serial Wombat  chip.  RW pin on the LCD must be grounded.

  In addition to the classic LiquidCrystal interfaces, this pin mode has a more advanced mode avaialble through the
 initializeBufferCopy() interface which allows displays to be updated from data stored in the Serial Wombat Chip's
User Buffer.  The Serial Wombat chip will handle getting the right data to the right location on the display.  This
is convenient for displays such as 20x4 displays which alternate lines when addressing.  When combined with a shifting
queue initialized with SerialWombatQueueType::QUEUE_TYPE_RAM_BYTE_SHIFT, the display can be treated like any other
Stream Class under Arduino.  See the Arduino examples directory for an example of this.

This class also supports buffer copying to large 40x4 character LCDs that have two E lines.  These displays are essentially two 44780 controllers
connected to a single piece of glass.  See the example in the Arduino examples directory.

E and E2 lines must be exclusive to a single LCD display.  If multiple LCDs are attached to a single Serial Wombat Chip then RS, D4, D5, D6, and D7
can be shared by both displays.

When in buffer mode the class updates one character per mS.  

A video Tutorial on this pin mode is available:

\htmlonly
//TODO
\endhtmlonly

//TODO https://youtu.be/


*/

class 
	SerialWombatLiquidCrystal : public Print, public SerialWombatPin
{
public:
	/*!
	@brief Class constructor for SerialWombatLiquidCrystal
	@param serialWombat The Serial Wombat chip on which the SerialWombatLiquidCrystal pin mode will run
	@param rs  The Serial Wombat Pin connected to the LCD rs pin.
	@param enable The Serial Wombat pin connected to the LCD e pin.
	@param d4 The Serial Wombat pin connected to the LCD d4 pin.
	@param d5 The Serial Wombat pin connected to the LCD d5 pin.
	@param d6 The Serial Wombat pin connected to the LCD d6 pin.
	@param d7 The Serial Wombat pin connected to the LCD d7 pin.
	*/
	SerialWombatLiquidCrystal(SerialWombatChip& serialWombat, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7):SerialWombatPin(serialWombat)
	{
		_pinMode = PIN_MODE_LIQUIDCRYSTAL;
		_pin = enable;
		_rs = rs;
		_d4 = d4;
		_d5 = d5;
		_d6 = d6;
		_d7 = d7;
	}


	/*!	
	@brief Initialization routine for SerialWombatLiquidCrystal
	
	@param cols  The width of the LCD in characters
	@param  rows The height of the LCD in character rows
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t begin(uint8_t cols = 80, uint8_t rows = 1)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
						_pin,
						_pinMode,
						_rs,
						_d4,
						_d5,
						_d6,
						_d7 };
		int16_t result = _sw.sendPacket(tx);
		if (result < 0)
		{
			return (result);
		}
		_rows = rows;
		_columns = cols;
		result = setRowOffsets(0, 0x40, cols, 0x40 + cols);
		if (result < 0)
		{
			return (result);
		}

		return(0);
	}

	/*!
	@brief Clear the LCD display
	
	By default this function delays 2 to 3 mS after issuing the command to be compatible with LiquidCrystal::clear
	This library adds an optional parameter that eliminates this delay.
	
	@param delay Whether or not to block for 2mS after issuing the command
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t clear(bool delayAfterClear = true)
	{
		return command(0x01);  // Clear display
		if (delayAfterClear)
		{
			delay(2);
		}
	}


	/*!
	@brief Set the LCD cursor / entry position to the 0,0 position
	
	By default this function delays 2 to 3 mS after issuing the command to be compatible with LiquidCrystal::home
	This library adds an optional parameter that eliminates this delay.

	@param delay Whether or not to block for 2mS after issuing the command

	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t home(bool delayAfterHome = true)
	{
		return command(0x02);  // Clear display
		if (delayAfterHome)
		{
			delay(2);
		}
	}

	/*!
	@brief Set the cursor / next entry position 
	
	@param col Column to set
	@param row Row to set

	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setCursor(uint8_t col, uint8_t row)
	{
		if (row >= _rows)
		{
			return -1;
		}

		if (col >= _columns)
		{
			return -1;
		}
		return command(0x80 + _offsets[row] + col);
	}

	/*!
	@brief Makes the cursor invisible
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t noCursor()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
						_pin,
						_pinMode,
						0, // DisplayControl
						(uint8_t)~0x02, // And Mask
						0, // OR MASK
						0x55,0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief Makes the cursor visible
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t cursor()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
						_pin,
						_pinMode,
								0, // DisplayControl
						0xFF, // And Mask
						0x02, // OR MASK
						0x55,0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief Makes the cursor blink
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t blink()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
						_pin,
						_pinMode,
								0, // DisplayControl

						0xFF, // And Mask
						0x01, // OR MASK
						0x55,0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief Makes the cursor not blink
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t noBlink()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
						_pin,
						_pinMode,
								0, // DisplayControl

						(uint8_t)~0x01, // And Mask
						0, // OR MASK
						0x55,0x55
		};
		return _sw.sendPacket(tx);
	}


	/*!
	@brief Enables LCD display
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t display()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
							_pin,
							_pinMode,
								0, // DisplayControl

							0xFF, // And Mask
							0x04, // OR MASK
							0x55,0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief Disables LCD display
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t noDisplay()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
						_pin,
						_pinMode,
								0, // DisplayControl

						(uint8_t)~0x04, // And Mask
						0, // OR MASK
						0x55,0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief Display moves left when scrolling
	
	 Note that this command has no effect in initializeBufferCopy() mode.

	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t scrollDisplayLeft()
	{
		return command(0x18); // Display Move left LCD
		
	}

	/*!
	@brief Display moves right when scrolling
	
	 Note that this command has no effect in initializeBufferCopy() mode.

	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t scrollDisplayRight()
	{
		return command(0x1C); // Display Move right LCD
	}

	/*!
	@brief Turn on autoscrolling mode on the LCD display (not in the Serial Wombat chip)
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t autoscroll()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
						_pin,
						_pinMode,
								1, // DisplayMode
						0xFF, // And Mask
						0x01, // OR MASK
						0x55,0x55
		};
		return _sw.sendPacket(tx);
	}


	/*!
	@brief Turn off autoscrolling mode on the LCD display (not in the Serial Wombat chip)
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t noAutoscroll()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
					_pin,
					_pinMode,
							1, // DisplayMode
					(uint8_t)~0x01, // And Mask
					0x00, // OR MASK
					0x55,0x55
		};
		return _sw.sendPacket(tx);
	}


	/*!
	@brief LCD Display entry occurs left to right
	
	 Note that this command has no effect in initializeBufferCopy() mode.
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t leftToRight()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
					_pin,
					_pinMode,
							1, // DisplayMode
					0xFF, // And Mask
					0x02, // OR MASK
					0x55,0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief LCD Display entry occurs right to left
	
	 Note that this command has no effect in initializeBufferCopy() mode.
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t rightToLeft()
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
					_pin,
					_pinMode,
							1, // DisplayMode
					(uint8_t)~0x02, // And Mask
					0x00, // OR MASK
					0x55,0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief Write a byte of data directly to the LCD
	
	 Use this command if using the LCD like a classic LiquidCrystal display.  Do not use this 
	command if in initializeBufferCopy() mode.
	
	@param data A byte of data to write to the display
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	size_t write(uint8_t data)
{
	uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE4,
	_pin,
	(uint8_t)PIN_MODE_LIQUIDCRYSTAL,
		2, // Data
		data,
		0x55,
		0x55,
		0x55
	};
	if (_sw.sendPacket(tx) >= 0)
	{
		return 1;
	}
	return 0;
}

	/*!
	@brief Write an array of data directly to the LCD
	
	 Use this command if using the LCD like a classic LiquidCrystal display.  Do not use this 
	command if in initializeBufferCopy() mode.
	
	@param buffer address of a buffer to be written to the display
	@param count number of bytes to write to the display
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	size_t write(uint8_t* buffer, size_t count)
	{
		size_t sent = 0;
		uint32_t startTime = millis();
		while (sent < count - 5) 
		{
			uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE5,
			_pin,
			(uint8_t)PIN_MODE_LIQUIDCRYSTAL,
				buffer[sent],
				buffer[sent + 1],
				buffer[sent + 2],
				buffer[sent + 3],
				buffer[sent + 4],		
			};
			if (_sw.sendPacket(tx) < 0)
			{
				return sent;
			}
			sent += 5;
		}
		while (sent < count - 1)  
		{
			uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE4,
			_pin,
			(uint8_t)PIN_MODE_LIQUIDCRYSTAL,
				2, // Data
				buffer[sent],
				2, // Data
				buffer[sent + 1],
				0x55
			};
			if (_sw.sendPacket(tx) < 0)
			{
				return sent;
			}
			sent += 2;
		}
		if (sent < count)
		{
			uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE4,
			   _pin,
			   (uint8_t)PIN_MODE_LIQUIDCRYSTAL,
				   2, // Data
				   buffer[sent],
				  0x55,
				0x55,
				0x55
			};
			if (_sw.sendPacket(tx) >= 0)
			{
				++sent;
			}
		}
		uint32_t endTime = millis();
		return sent;
	}

	/*!
	@brief Write bitmap data to the display for custom characters
	
	@param index The character to be written.  Valid values are typically 0-7
	@param bitmap An array of 7 bytes which represent the character.  bitmap[0] is the top, Least significant bit is the rightmost pixel.  1 is black pixel
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t createChar(uint8_t index, uint8_t bitmap[])
	{
		if (index >= 8)
		{
			return (-1);
		}
		int16_t result = command((uint8_t)(0x40 | (index << 3))); // Set character ram location
		if (result < 0)
		{
			return result;
		}
		for (int i = 0; i < 8; ++i)
		{
			result =(int16_t) write(bitmap[i]);
			if (result < 0)
			{
				return result;
			}
		}
		return 0;
	}

	/*!
	@brief Set the offsets in the LCD on-board memory for the beginning of each row
	
	These values are used for the setCursor() and initializeBufferCopy() functions and
	should be set before these functions are called.  
	An Arduino example is avaiable which displays addresses on the LCD which correspond to
	the parameters to this function to help the user determine proper values.
	
	@param row1 The offset of the top first row
	@param row2 The offset of the second row
	@param row3 The offset of the third row
	@param row4 The offset of the fourth row
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t setRowOffsets(uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4)
	{
		_offsets[0] = row1;
		_offsets[1] = row2;
		_offsets[2] = row3;
		_offsets[3] = row4;
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE8,
					_pin,
					_pinMode,
						0,row1,0x55,0x55
		};
		int16_t result = _sw.sendPacket(tx);	if (result < 0) { return result; };
		tx[3] = 1; tx[4] = row2;
	    result = _sw.sendPacket(tx);	if (result < 0) { return result; };
		tx[3] = 2; tx[4] = row3;
		result = _sw.sendPacket(tx);	if (result < 0) { return result; };
		tx[3] = 3; tx[4] = row4;
		result = _sw.sendPacket(tx);	if (result < 0) { return result; };
		return(0);
	}

	/*!
	@brief send a command directly to the LCD
	
	@param cmd Byte to be sent to the LCD while the RS line is set to command
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t command(uint8_t cmd)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE4,
		_pin,
		(uint8_t)PIN_MODE_LIQUIDCRYSTAL,
			1, // Command
			cmd, 
			0x55,
			0x55,
			0x55
		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief returns the e pin number.  Override for use with classes that require this as a virtual function
	
	@return Pin number of the e pin
	*/
	uint8_t pin() { return _pin; }

	/*!
	@brief returns the pin mode number.  Override for use with classes that require this as a virtual function
	
	@return Pin mode of the current pin
	*/
	uint8_t swPinModeNumber() { return _pinMode; }

	/*!
	@brief kicks off the Serial Wombat chip state machine that copies one character to the LCD each second from a buffer
	
	 This function extends the LiquidCrystal functionality by adding a state machine that runs on the Serial Wombat chip
	This state machine copies a byte every ms from an index into the User Buffer area to the LCD.  It uses the offsets
	set using setRowOffsets() and the width parameter to automatically format the data from the User Buffer into lines in
	the proper order.  When used with the SerialWombatQueueType::QUEUE_TYPE_RAM_BYTE_SHIFT type queue to create a FIFO 
	style display.  
	@param bufferIndex Index into the User Buffer where data copy starts.  When using a shifted Queue, this is the queue address + 8.
	@param width The width data to copy on each line.  Defaults to the number of columns specified in begin() if not provided.
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t initializeBufferCopy(uint16_t bufferIndex, uint8_t width = 255)
	{
		if (width == 255)
		{
			width = _columns;
		}
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
		_pin,
		(uint8_t)PIN_MODE_LIQUIDCRYSTAL,
			(byte)(bufferIndex & 0xFF),
			(byte)(bufferIndex >>8),
			width,
			0x55,
			0x55,

		};
		return _sw.sendPacket(tx);
	}
//TODO	int16_t dataMonitor(uint8_t pin, uint8_t col, uint8_t row, uint8_t index, bool hex=false);

	/*!
	@brief Add a second e pin for use wtih 40x4 LCD displays
	
	This commands causes the Serial Wombat chip to do a second initialization of a display.  This is commonly required
	for 40x4 LCDs which are essenctially two LCD drivers attached to the same glass, one driver per two 40 character lines.
	
	This is only used when using the initializeBufferCopy() mode of this pin mode.  When this value is not set 80 bytes
	are copied to the display attached to the e pin.  When this value is set, 80 bytes are copied to the display attached
	to the e pin, followed by an additional 80 bytes attached to the e2 pin. 
	
	@return returns a number 0 or greater for success, negative numbers indicate an error occured.
	*/
	int16_t beginE2(uint8_t e2Pin)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE7,
		_pin,
		(uint8_t)PIN_MODE_LIQUIDCRYSTAL,
			e2Pin,
			0x55,
			0x55,
			0x55,
			0x55,

		};
		return _sw.sendPacket(tx);
	}

	using Print::write;  //Inherit the write commands from print.


private:
	uint8_t _rs = 255,
		_d4 = 255,
		_d5 = 255,
		_d6 = 255,
		_d7 = 255;
	uint8_t _rows = 2;
	uint8_t _columns = 40;
	uint8_t _offsets[8] = { 0,64,20,84,0,64,20,84 };

};
