#pragma once

/*
Copyright 2023-2024 Broadwell Consulting Inc.

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
/*! \file SerialWombat18ABVGA.h
*/
/*! @brief A class for the Serial Wombat SW18AB chip VGA Driver


A Tutorial video is avaialble:
https://youtu.be/DcOSat8VybA
\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/DcOSat8VybA" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly


This file/pin mode  is designed to drive a VGA monitor RGB and H and V Sync lines.  The
output is essentially 1 bit, with the ability to change the color between 8
colors (including black) by horizonal line.

This pin mode is unusual among SW18AB pin modes because it requires specific pins to be
used for certain things.  Pins must be:

VGA VSYNC (VGA Pin 14) -> 100 ohm Resistor -> SW Pin 18
VGA HSYNC (VGA Pin 13) -> 100 ohm Resistor ->SW Pin 17
VGA Red	 (VGA Pin 1) -> 280 ohm Resistor -> SW Pin 16
VGA Blue (VGA Pin 2) -> 280 ohm Resistor -> SW Pin 15
VGA Green (VGA Pin 3) -> 280 ohm Resistor -> SW Pin 14

Thank you to Nick Gammon who published a very informative article on driving a VGA monitor here:
http://www.gammon.com.au/forum/?id=11608

\warn This pin mode stretches the limits of what the SW18AB chip can do while still being able to do other things - some flicker / jitter is to be expected

The rate at which the display can be updated is slower than with a hardware connected LCD or OLED.  The Serial Wombat protocol's 8 byte in / 8 byte out 
packet structure is not ideal for moving large blocks of data such as screen pixel data.  This mode is better suited to data displays than
real-time games, for example.

The pin mode is capable of limited color generation with the restriction that an entire horizontal line must be the same color.  Colors are achieved
by turning Red, Green, and Blue lines totally on or off in combinations.

This pin mode implements a 160x120 pixel output to a monitor in 640x480 mode with black bars around part of the 
screen.  This was the best I could do using the chip's SPI in DMA mode.

The SerialWombat18ABVGADriver class (A separate Arduino Library) is designed to act as a wrapper between this pin mode
and the AdafruitGFX library.  See the Arduino examples for this pin mode for an example.

This pin mode uses a significant amount of SW18AB time that is not measured using typical methods due to the high number of
interrupts it produces.  These interrupts happen both during and outside of the main loop executive processing, so actual
system utilization is higher than normal metrics indicate.
*/



class SerialWombat18ABVGA : public SerialWombatPin
{
public:
	/*!
	@brief Constructor for the SerialWombat18ABVGA class.
	@param serialWombat The Serial Wombat Chip on which the SerialWombat18ABVGA instance will run.
	*/
	SerialWombat18ABVGA(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat)
	{
	}
	/*!
	@brief Initalize the SerialWombat18ABVGA.
	@param vsyncPin Pin attached to the VGA VSync pin (Must be 18)
	@param bufferIndex   - Index into the buffer where the  2520 byte frame buffer will be stored
	@return 0 or higher for success, negative number for error
	*/
	int16_t begin(uint8_t vsyncPin, uint16_t bufferIndex = 0)
	{
		_pin = vsyncPin;
		_pinMode = (uint8_t)PIN_MODE_VGA;

		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
							_pin,
							(uint8_t)_pinMode ,
							0x55,
							0x55,
							SW_LE16(bufferIndex),
							0x55
		};
		return _sw.sendPacket(tx);
	}


	/*!
	 @brief Write a pixel to the buffer.
	@param x The x coordinate.  0 - 159
	@param y The y coordinate 0-119
	@param color 0 - off (black) 1- on (assigned line color)
	@return 0 or higher for success, negative number for error
	*/
	int16_t writePixel(uint8_t x, uint8_t y, uint8_t color)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
							_pin,
							(uint8_t)_pinMode ,
							0, //Single pixel
							x,
							y,
							color,
							0x55,

		};
		return _sw.sendPacket(tx);
	}

	/*!
	@brief fill the entire screen
	@param color  0 - off (black) 1- on (assigned line color)
	@return 0 or higher for success, negative number for error
	*/
	int16_t fillScreen(uint8_t color)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
							_pin,
							(uint8_t)_pinMode ,
							1, //FillScreen
							color,
							0x55,
							0x55,
			0x55

		};
		return _sw.sendPacket(tx);
	}


	/*!
	@brief Draw a filled rectangle on the screen
	@param x The x coordinate.  0 - 159
	@param y The y coordinate 0-119
	@param w Width
	@param h Height
	@param color 0 - off (black) 1- on (assigned line color)
	@return 0 or higher for success, negative number for error
	*/
	int16_t fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,
		uint8_t color)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
							_pin,
							(uint8_t)_pinMode ,
							2, //Fill Rect
							x,
							y,
							(uint8_t)(x + w - 1),
			(uint8_t)(y + h - 1)

		};
		if (color == 0)
		{
			tx[3] = 3; // Make clear rect
		}
		return _sw.sendPacket(tx);
	}

	/*!
	@brief Set the color of a horizontal line
	@param color : 1 = Blue, 2 = Green 3= Cyan, 4 = Red, 5 = Magenta, 6 = Yellow, 7 = White
	@param y Line to set
	@return 0 or higher for success, negative number for error
	*/
	int16_t setLineColor(uint8_t color, uint8_t y)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE2,
							_pin,
							(uint8_t)_pinMode ,
							y,
							y,
							color,
			0x55,
			0x55

		};
		return _sw.sendPacket(tx);

	}
	/*!
	@brief Set the color of a horizontal line
	@param color : 1 = Blue, 2 = Green 3= Cyan, 4 = Red, 5 = Magenta, 6 = Yellow, 7 = White
	@param start first y Line to set
	@param end last y Line to set
	@return 0 or higher for success, negative number for error
	*/
	int16_t setLineColor(uint8_t color, uint8_t start, uint8_t end)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE2,
							_pin,
							(uint8_t)_pinMode ,

							start,
							end,
							color,
			0x55,
			0x55

		};
		return _sw.sendPacket(tx);

	}
};
