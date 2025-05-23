#pragma once

/*
Copyright 2023 Broadwell Consulting Inc.

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
#include "Stream.h"
#include "SerialWombat.h"
/*! \file SerialWombatPS2Keyboard.h
*/


typedef enum
{
    SCANCODE_OPENSINGLEQUOTE = 0x0E,
    SCANCODE_1 = 0x16,
    SCANCODE_2 = 0x1E,
    SCANCODE_3 = 0x26,
    SCANCODE_4 = 0x25,
    SCANCODE_5 = 0x2E,
    SCANCODE_6 = 0x36,
    SCANCODE_7 = 0x3D,
    SCANCODE_8 = 0x3E,
    SCANCODE_9 = 0x46,
    SCANCODE_0 = 0x45,
    SCANCODE_DASH = 0x4E,
    SCANCODE_EQUALS = 0x55,
    SCANCODE_BACKSPACE = 0x66,
    SCANCODE_TAB = 0x0D,
    SCANCODE_Q = 0x15,
    SCANCODE_W = 0x1D,
    SCANCODE_E = 0x24,
    SCANCODE_R = 0x2D,
    SCANCODE_T = 0x2C,
    SCANCODE_Y = 0x35,
    SCANCODE_U = 0x3C,
    SCANCODE_I = 0x43,
    SCANCODE_O = 0x44,
    SCANCODE_P = 0x4D,
    SCANCODE_LEFTBRACKET = 0x54,
    SCANCODE_RIGHTBRACKET = 0x5B,
    SCANCODE_CAPSLOCK = 0x58,
    SCANCODE_A = 0x1C,
    SCANCODE_S = 0x1B,
    SCANCODE_D = 0x23,
    SCANCODE_G = 0x34,
    SCANCODE_H = 0x33,
    SCANCODE_J = 0x3B,
    SCANCODE_K = 0x42,
    SCANCODE_L = 0x4B,
    SCANCODE_SEMICOLON = 0x4C,
    SCANCODE_QUOTE = 0x52,
    SCANCODE_ENTER = 0x5A,
    SCANCODE_LEFTSHIFT = 0x12,
    SCANCODE_Z = 0x1A,
    SCANCODE_X = 0x22,
    SCANCODE_C = 0x21,
    SCANCODE_V = 0x2A,
    SCANCODE_B = 0x32,
    SCANCODE_N = 0x31,
    SCANCODE_M = 0x3A,
    SCANCODE_COMMA = 0x41,
    SCANCODE_PERIOD = 0x49,
    SCANCODE_SLASH = 0x4A,
    SCANCODE_RIGHTSHIFT = 0x59,
    SCANCODE_LEFTCTRL = 0x14,
    SCANCODE_LEFTALT = 0x11,
    SCANCODE_SPACEBAR = 0x29,
    SCANCODE_RIGHTALT = 0x91,
    SCANCODE_RIGHTCTRL = 0x94,
    SCANCODE_INSERT = 0xF0,
    SCANCODE_DELETE = 0xF1,
    SCANCODE_LEFTARROW = 0xEB,
    SCANCODE_HOME = 0xEC,
    SCANCODE_END = 0xE9,
    SCANCODE_UPARROW = 0xF5,
    SCANCODE_DOWNARROW = 0xF2,
    SCANCODE_PAGEUP = 0xFD,
    SCANCODE_PAGEDOWN = 0xFA,
    SCANCODE_RIGHTARROW = 0xF4,
    SCANCODE_NUMLOCK = 0x76,
    SCANCODE_KP7 = 0x6C,
    SCANCODE_KP4 = 0x6B,
    SCANCODE_KP1 = 0x69,
    SCANCODE_KPSLASH = 0xCA,
    SCANCODE_KP8 = 0x75,
    SCANCODE_KP5 = 0x73,
    SCANCODE_KP2 = 0x72,
    SCANCODE_KP0 = 0x70,
    SCANCODE_KPASTERISK = 0x7C,
    SCANCODE_KP9 = 0x7D,
    SCANCODE_KP6 = 0x74,
    SCANCODE_KP3 = 0x7A,
    SCANCODE_KPPERIOD = 0x71,
    SCANCODE_KPMINUS = 0x7B,
    SCANCODE_KPPLUS = 0x79,
    SCANCODE_KPENTER = 0xDA,
    SCANCODE_ESC = 0x76,
    SCANCODE_F1 = 0x05,
    SCANCODE_F2 = 0x06,
    SCANCODE_F3 = 0x04,
    SCANCODE_F4 = 0x0C,
    SCANCODE_F5 = 0x03,
    SCANCODE_F6 = 0x0B,
    SCANCODE_F7 = 0x83,
    SCANCODE_F8 = 0x0A,
    SCANCODE_F9 = 0x01,
    SCANCODE_F10 = 0x09,
    SCANCODE_F11 = 0x78,
    SCANCODE_F12 = 0x07,
    SCANCODE_PRINTSCREEN = 0x92,
    SCANCODE_SCROLLLOCK = 0x7E,
    SCANCODE_BACKSLASH = 0x5D
}PS2KeyboardScanCode;


/*! @brief A class for the Serial Wombat SW18AB chips which recieves input from IBM PS2 Keyboards



A Tutorial video is avaialble:

https://youtu.be/TODO
\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" 
title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; 
clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly



The class inherits from the Arduino Stream class, so queued ps2 keyboard presses can be read like a
Serial port.

This class allows the user to declare a PS2 Keyboard.  The PS2 Keyboard class is currently only supported
on the Serial Wombat 18AB chip. 

\warning The PS2 Keyboard pin mode requires 20 to 25% of the Serial Wombat 18AB chip's processor capacity.  
Assigning pin modes which together exceed avaialble processing capacity causes malfunctions within
the Serial Wombat chip.

The PS2 Keyboard pin mode requires a clock pin (to which this pin mode is assigned) and an
additional data pin.  Both should be tied high to 5v with a pull up resistor.  I use a 5.1k.

\warning The PS2 Keyboard inputs are 5V inputs.  It is suggested that pins 9,10,11,12,14, or 15 on the Serial
Wombat 18AB chip be used for PS2 Keyboard because they are 5V tollerant.  Using other pins may
damage the Serial Wombat chip.  

A video Tutorial on this pin mode is available:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/YV00GfyxFJU" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/YV00GfyxFJU

*/

class SerialWombatPS2Keyboard :
    public Stream, public SerialWombatPin
{
public:
	/*!
    @brief Constructor for the SerialWombatPS2Keyboard class.  
    @param serialWombat The Serial Wombat Chip on which the SerialWombatPS2Keyboard instance will run.
    */
    SerialWombatPS2Keyboard(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat){}
    /*!
    @brief Initalize the SerialWombatPS2Keyboard.  
    @param clockPin Pin attached to the PS2 Keyboard Clock line.  This line shoudl be pulled up to 5V with a resistor (5.1k suggested).  This pin should be a 5V tolerant pin.
    @param dataPin Pin attached to the PS2 Keyboard data line.  This line shoudl be pulled up to 5V with a resistor (5.1k suggested).  This pin should be a 5V tolerant pin.
    @param bufferMode 0: Public data is lower case ASCII of key pressed (Default)  1:  Public data is PS2 Keyboard 'make' code of last key pressed or released.
    @param queueMode  0: Queued data is ASCII values, taking into account shift keys 1: Queued data is make codes of keys when pressed  2:  All PS2 codes are queued 3: A bitfield of held keys is maintained instead of a queue
    @param queueAddress An optional parameter that allows a previously initialized queue in User RAM on the SW18AB chip to be used instead of the pin mode's internal 16 byte buffer.  0xFFFF uses pin mode buffer.
    @param pullUpDown 0: No pull ups or pull downs 1:  Internal 3.3v pullup (not recommended) 2: Internal Pulldown (not recommended)
    */
    int16_t begin(uint8_t clockPin, uint8_t dataPin, uint8_t bufferMode = 0, uint8_t queueMode = 0, uint16_t queueAddress = 0xFFFF, uint8_t pullUpDown = 0)
	{
		_pin = clockPin;
		_pinMode = (uint8_t)PIN_MODE_PS2KEYBOARD;

		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
							_pin,
							(uint8_t)_pinMode ,
							dataPin,
							queueMode,
							bufferMode,
							0x55,
							 pullUpDown};
		int16_t result = _sw.sendPacket(tx);
		if (result < 0)
		{
			return result;
		}

		if (queueAddress != 0xFFFF)
		{
		uint8_t tx6[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
							_pin,
							_pinMode,
							SW_LE16(queueAddress),
							0x55,
							0x55,
							0x55 };

		return _sw.sendPacket(tx6);
		}
		return (result);
	}
    


    /*!
    @brief Queries the SerialWombatPS2Keyboard for number bytes available to read
    @return Number of bytes available to read.
    */
    int available()
	{
		uint8_t tx[8] = { 201, _pin, _pinMode, 0,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		_sw.sendPacket(tx, rx);
		return (rx[4]);
	}
    /*!
    @brief Reads a byte from the SerialWombatPS2Keyboard queue
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
    /*!
    @brief  Discard all bytes from the SerialWombatPS2Keyboard queue
    */
    void flush()
	{
		
	}

    /*!
    @brief Query the SerialWombatPS2Keyboard queue for the next avaialble byte, but don't remove it from the queue
    @return A byte from 0-255, or -1 if no bytes were avaialble
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
    @brief Write a byte to the SerialWombatPS2Keyboard queue  (Does Nothing)
    @param data  Byte to write
    @return Number of bytes written
    
    This function exists to fully implement the Stream class.  It throws away the byte.
    */
    size_t write(uint8_t data)
{
	(void)data; // Avoid compiler warning for unused parameter
	
	return (1);
}

	/*!
    @brief Write bytes to the SerialWombatPS2Keyboard queue (Does nothing)
    @param buffer  An array of uint8_t bytes to send
    @param size the number of bytes to send
    @return the number of bytes sent
    
    This function exists to fully implement the Stream class.  It throws away the bytes.
    */
    size_t write(const uint8_t* buffer, size_t size)
{
	(void)buffer; // Avoid compiler warning for unused parameter
	return(size);
}

	/*!
    @brief Number of bytes avaialble to write to SerialWombatPS2Keyboard queue.  Returns 0
    @return Zero.  Writes are not suppored.
    */
    int availableForWrite()
{
	return(0);
}

	/*!
    @brief Reads a specified number of bytes from the SerialWombatPS2Keyboard queue queue
    @param buffer  An array into which to put received bytes
    @param length  The maximum number of bytes to be received
    @return the number of bytes written to buffer
    
    This function will read bytes from the SerialWombatPS2Keyboard queue into buffer.
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

    int16_t readCurrentScanCodes(uint8_t* buffer, uint8_t startValue)
	{
		uint8_t tx[8] = { 207, _pin,_pinMode, startValue,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];

		int16_t result = _sw.sendPacket(tx, rx);

		if (result < 0)
		{
			return result;
		}
		int count = 0;
		for (int i = 3; i < 8; ++i)
		{
			if (rx[i] != 0)
			{
				++count;
			}
			buffer[i - 3] = rx[i];
		}
		return count;
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
     @brief convernts a set 2 Scan Code to Ascii
    */
    uint8_t scanCodeToAscii(uint8_t scanCode, bool shiftActive = false)
	{

		if (scanCode >= 0x80)
		{
			return 0;
		}
		//This array is used to convert PS2 Code set 2 codes to Ascii. The first column is unshfited data, the second if shift is active
		const uint8_t KBSCSet2[][2] =
		{
		{ ' ' , ' ' },  /* 0 */
		{ ' ' , ' ' },  /* 1 */
		{ ' ' , ' ' },  /* 2 */
		{ ' ' , ' ' },  /* 3 */
		{ ' ' , ' ' },  /* 4 */
		{ ' ' , ' ' },  /* 5 */
		{ ' ' , ' ' },  /* 6 */
		{ ' ' , ' ' },  /* 7 */
		{ ' ' , ' ' },  /* 8 */
		{ ' ' , ' ' },  /* 9 */
		{ ' ' , ' ' },  /* A */
		{ ' ' , ' ' },  /* B */
		{ ' ' , ' ' },  /* C */
		{ 9 , 9 },  /* D */
		{ '`' , '~' },  /* E */
		{ ' ' , ' ' },  /* F */
		{ ' ' , ' ' },  /* 10 */
		{ ' ' , ' ' },  /* 11 */
		{ 0 , 0 },  /* 12 */
		{ ' ' , ' ' },  /* 13 */
		{ ' ' , ' ' },  /* 14 */
		{ 'q' , 'Q' },  /* 15 */
		{ '1' , '!' },  /* 16 */
		{ ' ' , ' ' },  /* 17 */
		{ ' ' , ' ' },  /* 18 */
		{ ' ' , ' ' },  /* 19 */
		{ 'z' , 'Z' },  /* 1A */
		{ 's' , 'S' },  /* 1B */
		{ 'a' , 'A' },  /* 1C */
		{ 'w' , 'W' },  /* 1D */
		{ '2' , '@' },  /* 1E */
		{ ' ' , ' ' },  /* 1F */
		{ ' ' , ' ' },  /* 20 */
		{ 'c' , 'C' },  /* 21 */
		{ 'x' , 'X' },  /* 22 */
		{ 'd' , 'D' },  /* 23 */
		{ 'e' , 'E' },  /* 24 */
		{ '4' , '$' },  /* 25 */
		{ '3' , '#' },  /* 26 */
		{ ' ' , ' ' },  /* 27 */
		{ ' ' , ' ' },  /* 28 */
		{ ' ' , ' ' },  /* 29 - SPACE */
		{ 'v' , 'V' },  /* 2A */
		{ 'f' , 'F' },  /* 2B */
		{ 't' , 'T' },  /* 2C */
		{ 'r' , 'R' },  /* 2D */
		{ '5' , '%' },  /* 2E */
		{ ' ' , ' ' },  /* 2F */
		{ ' ' , ' ' },  /* 30 */
		{ 'n' , 'N' },  /* 31 */
		{ 'b' , 'B' },  /* 32 */
		{ 'h' , 'H' },  /* 33 */
		{ 'g' , 'G' },  /* 34 */
		{ 'y' , 'Y' },  /* 35 */
		{ '6' , '^' },  /* 36 */
		{ ' ' , ' ' },  /* 37 */
		{ ' ' , ' ' },  /* 38 */
		{ ' ' , ' ' },  /* 39 */
		{ 'm' , 'M' },  /* 3A */
		{ 'j' , 'J' },  /* 3B */
		{ 'u' , 'U' },  /* 3C */
		{ '7' , '&' },  /* 3D */
		{ '8' , '*' },  /* 3E */
		{ ' ' , ' ' },  /* 3F */
		{ ' ' , ' ' },  /* 40 */
		{ ',' , '>' },  /* 41 */
		{ 'k' , 'K' },  /* 42 */
		{ 'i' , 'I' },  /* 43 */
		{ 'o' , 'O' },  /* 44 */
		{ '0' , ')' },  /* 45 */
		{ '9' , '(' },  /* 46 */
		{ ' ' , ' ' },  /* 47 */
		{ ' ' , ' ' },  /* 48 */
		{ '.' , '<' },  /* 49 */
		{ '/' , '?' },  /* 4A */
		{ 'l' , 'L' },  /* 4B */
		{ ';' , ':' },  /* 4C */
		{ 'p' , 'P' },  /* 4D */
		{ '-' , '_' },  /* 4E */
		{ ' ' , ' ' },  /* 4F */
		{ ' ' , ' ' },  /* 50 */
		{ ' ' , ' ' },  /* 51 */
		{ '\'' , '"' },  /* 52 */
		{ ' ' , ' ' },  /* 53 */
		{ '[' , '{' },  /* 54 */
		{ '=' , '+' },  /* 55 */
		{ ' ' , ' ' },  /* 56 */
		{ ' ' , ' ' },  /* 57 */
		{ ' ' , ' ' },  /* 58 */
		{ 0 , 0 },  /* 59 */
		{ 0xD , 0xD },  /* 5A */
		{ ']' , '}' },  /* 5B */
		{ ' ' , ' ' },  /* 5C */
		{ '\\' , '|' },  /* 5D */
		{ ' ' , ' ' },  /* 5E */
		{ ' ' , ' ' },  /* 5F */
		{ ' ' , ' ' },  /* 60 */
		{ ' ' , ' ' },  /* 61 */
		{ ' ' , ' ' },  /* 62 */
		{ ' ' , ' ' },  /* 63 */
		{ ' ' , ' ' },  /* 64 */
		{ ' ' , ' ' },  /* 65 */
		{ 8 , 8 },  /* 66 */
		{ ' ' , ' ' },  /* 67 */
		{ ' ' , ' ' },  /* 68 */
		{ '1' , '1' },  /* 69 */
		{ ' ' , ' ' },  /* 6A */
		{ '4' , '4' },  /* 6B */
		{ '7' , '7' },  /* 6C */
		{ ' ' , ' ' },  /* 6D */
		{ ' ' , ' ' },  /* 6E */
		{ ' ' , ' ' },  /* 6F */
		{ '0' , '0' },  /* 70 */
		{ ' ' , ' ' },  /* 71 */
		{ '2' , '2' },  /* 72 */
		{ '5' , '5' },  /* 73 */
		{ '6' , '6' },  /* 74 */
		{ '8' , '8' },  /* 75 */
		{ 0x1B , 0x1B },  /* 76 */
		{ ' ' , ' ' },  /* 77 */
		{ ' ' , ' ' },  /* 78 */
		{ ' ' , ' ' },  /* 79 */
		{ '3' , '3' },  /* 7A */
		{ ' ' , ' ' },  /* 7B */
		{ ' ' , ' ' },  /* 7C */
		{ '9' , '9' },  /* 7D */
		{ ' ' , ' ' },  /* 7E */
		{ ' ' , ' ' },  /* 7F */
		};

		if (shiftActive)
		{
			return ( KBSCSet2[scanCode][ 1]);
		}

		return  (  KBSCSet2[scanCode][0]);
	}

	/*!
    @brief Check to see if a key is currently pressed (pin mode must be configured for bitfield mode)
    @param scanCode The scan code of the key being checked.  Set the 0x80 bit if it's an extended code
    @return returns true if the key is currently pressed
    */
    bool isKeyPressed(uint8_t scanCode)
{
	uint8_t buffer[5];
	int16_t result = readCurrentScanCodes(buffer, scanCode);

	if (result < 0)
	{
		return false;
	}

	if (buffer[0] == scanCode)
	{
		return true;
	}

	return false;

}

    bool isKeyPressed(PS2KeyboardScanCode scanCode)
{
	return isKeyPressed((uint8_t) scanCode);
}

     uint8_t _pin = 255;
protected:
     uint32_t timeout = 1; 
};




