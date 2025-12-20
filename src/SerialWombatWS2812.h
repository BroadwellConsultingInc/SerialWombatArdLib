#pragma once
/*
Copyright 2021-2025 Broadwell Consulting Inc.

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


enum SWWS2812Mode {
	ws2812ModeBuffered = 0,  ///< Standard buffered mode.  Colors are uploaded by the host
	ws2812ModeAnimation = 1,	///< Multiple arrays with delays are uploaded by the host and displayed over time by the Serial Wombat chip
	ws2812ModeChase = 2, ///< A single lit LED cycles through all of the LEDs.
};

/*!
\brief A Class representing a WS2812 or compatible RGB LED string connected to a Serial Wombat pin

This class is only supported on the Serial Wombat SW18AB chip.  It is not supported on the
Serial Wombat 4X line.  This pin mode can only be used on enhanced capability pins (WP0-4, WP7, or WP9-19)

This class controls a State Machine driven driver for a WS2812 compatible RGB LED string.

Each instance of this class uses an average of approximately TBD% of the SW18's processing time.
This varies by configuration options and usage.

The Serial Wombat WS2812 driver can be configured in a number of ways:
* The driver lights up the LEDs one at a time in sequence
* The driver shows colors as commanded by the host
* The driver cycles through arrays of colors at a specified rate


See the available examples in the Arduino Library for usage.

\warning Different WS2812 pcbs behave differently based on how the manufacturer routed the LEDs on the PCB Board.
For instance a square 4x4 matrix may not light in the order expected.  This is not an issue with the library.

\warning An array of WS2812 LEDs can pull lots of current.  Lighting multiple LEDs at full brightness may consume
more power than your supply can provide, causing the system voltage to become unstable.  An unstable system voltage
can cause unreliable operation of the Serial Wombat chip.

The Serial Wombat WS2812 driver is extremely efficient in terms of processor time since it uses
the PIC24FJ256GA702's DMA and SPI hardware to generate the WS2812 signal.  This allows the Serial
Wombat firmware to easily clock out WS2812 signals while doing other thigns.  However, this method
is very RAM intensive, requiring about 50 bytes of ram for each LED.  

The RAM used for buffering this signal is stored in the User Buffer RAM, an array available for the
user to allocate to various PIN modes' uses.  In Version 2.0.3 of the Serial Wombat 18AB firmware
there is 8k of RAM allocated to User Buffer, allowing about 160 LEDs to be used if all RAM is
allocated to the WS2812.  

A number of frames to be shown in rotation with configurable delays inbetween can also be stored
in the User Buffer.  This is in additional to the rendering buffer.  Each animation frame requires
2+3*NumberOfLEDs bytes.

The Update rate is variable with the number of LEDs so that rendering of colors into the User Buffer
is spread across multiple Serial Wombat 1mS execution frames.  The LEDs will be updated approximately every
X mS, where X is the number of LEDs plus 20.

A tutorial is available here:

https://youtu.be/WoXvLBJFpXk

\htmlonly
<iframe width = "560" height = "315" src = "https://youtu.be/WoXvLBJFpXk" title = "YouTube video player"
frameborder = "0" allow = "accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; 
picture - in - picture" allowfullscreen></iframe>
\endhtmlonly
*/

class SerialWombatWS2812 : public SerialWombatPin
{
public:
	/*!
	\brief Constructor for SerialWombatWS2812 class
   \param serialWombat SerialWombat chip on which the driver will run
   	*/
	SerialWombatWS2812(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat)
	{
	}

	/*!
	\brief Initialize a WS2812 LED driver object
	
	This function initializes hardware and data arrays for the WS2812 driver.
	It requires about 30uS per LED to initialize the data array.  During this
	time pin processing is suspended.  This can cause glitches in other pins.
	It is recommended that this pin mode be initialized once near the beginning of
	at power up and 
	that begin() not be called during real-time operation.
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	\param pin  The pin connected to the WS2812.  This must be an enhanced capability pin ( WP0-4, WP7, or WP9-19)
	\param numberOfLEDs The number of LEDs connected in series to the pin
	\param userBufferIndex The index in bytes into the User Buffer area where the signal train to be sent to the LEDs is stored.  
	The amount of data bytes required for the configured number of LEDs can be queried with readBufferSize.  This area must not
	be used by other pins, and cannot extend past the end of the 8k of space.
	*/
	int16_t begin(uint8_t pin, uint8_t numberOfLEDs, uint16_t userBufferIndex)
	{
		_pin = pin;
		_numLEDS = numberOfLEDs;
		_userBufferIndex = userBufferIndex;

		uint8_t tx[8] = { 200,_pin,12,SW_LE16(userBufferIndex),_numLEDS,0x55 };
		return (_sw.sendPacket(tx));
	}

	/*!
	\brief Set an LED color
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	\param led The index of the LED to be set to color
	\param color The color of the LED in 0x00RRGGBB format
	*/
	int16_t write(uint8_t led, uint32_t color)
	{
		uint8_t tx[8] = { 201,_pin,12,led,SW_LE32(color) };
		if (swapRG)
		{
			uint8_t x = tx[6];
			tx[6] = tx[5];
			tx[5] = x;
		}
		tx[7] = 0x55;
		return _sw.sendPacket(tx);
	}

	/// \brief An overload for Write in case write(x,0); is interpreted as an int16_t rather than uint32_t
	int16_t write(uint8_t led, int16_t color)
	{
		return write(led, (uint32_t)color);
	}

	/// \brief An overload for Write in case write(x,0); is interpreted as an int32_t rather than uint32_t
	int16_t write(uint8_t led, int32_t color)
	{
		return write(led, (uint32_t)color);
	}

	/*
	\brief Set a number of LEDs to colors based on an array of uint32_t colors
	
	\param led  The index of the first led to set
	\param length The number of LEDs to set, and the number of entires in colors array
	\param An array of uint32_t integer colors in the format 0x00RRGGBB format
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	*/
	int16_t write(uint8_t led, uint8_t length,  uint32_t colors[])
	{
		for (int i = 0; i < length; ++i)
		{
			int16_t result = 
			write(led + i, colors[i]);

			if (result < 0)
			{
				return (result);
			}

		}
		return(0);
	}

	/*
	\brief set the color of one LED in an animation frame
	
	\param frame The Frame index of the color being set
	\param led The LED index in that frame of the color being set
	\param color The color of the LED in 0x00RRGGBB format
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	*/
	int16_t writeAnimationLED(uint8_t frame, uint8_t led, uint32_t color)
	{
		uint8_t tx[8] = { 203,_pin,12,frame,led,(uint8_t)((color >>16 ) & 0xFF),(uint8_t)((color >> 8) & 0xFF),(uint8_t)( color & 0xFF) };
		return _sw.sendPacket(tx);
	}

	/// \brief An overload color is interpreted as an int16_t rather than uint32_t
	int16_t writeAnimationLED(uint8_t frame, uint8_t led, int16_t color)
	{
		return writeAnimationLED(frame, led,(uint32_t)color);
	}

	/// \brief An overload color is interpreted as an int32_t rather than uint32_t
	int16_t writeAnimationLED(uint8_t frame, uint8_t led, int32_t color)
	{
		return writeAnimationLED(frame, led, (uint32_t)color);
	}

	/*!
	\brief Store an array of colors for an entire animation frame
	
	\param frame The index of the frame being stored
	\param colors an array of uint32_t colors in 0x00RRGGBB format to be stored in the frame.  The length of the array must match the number of LEDs
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	*/
	int16_t writeAnimationFrame(uint8_t frame, uint32_t colors[])
	{
		for (int i = 0; i < _numLEDS; ++i)
		{
			int16_t result;
			result = writeAnimationLED(frame, i, colors[i]);
			if (result < 0)
			{
				return (result);
			}
		}
		return(0);
		
	}

	/*!
	\brief Set how long an animation frame should be displayed before moving to the next frame
	\param frame The index of the frame being set
	\param delay_mS the amount of time to display the frame in mS
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	*/
	int16_t writeAnimationFrameDelay(uint8_t frame, uint16_t delay_mS)
	{
		uint8_t tx[8] = { 205,_pin,12,frame,SW_LE16(delay_mS),0x55,0x55 };
		return (_sw.sendPacket(tx));
	}

	/*!
	\brief set the location in UserBuffer where the animation array will be stored and number of frames
	
	\param index The index into UserBuffer
	\param numberOfFrames The number of frames that make up the animation
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	*/
	int16_t writeAnimationUserBufferIndex(uint16_t index, uint8_t numberOfFrames)
	{
		uint8_t tx[8] = { 204,_pin,12,SW_LE16(index),numberOfFrames,0x55,0x55 };
		return (_sw.sendPacket(tx));
	}

	/*!
	\brief returns the number of bytes of UserBuffer required to service the configured number of LEDs
	
	This number does not include any animation frames.
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	*/
	int16_t readBufferSize()
	{
		uint8_t tx[8] = { 202,_pin,12,_numLEDS,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		int16_t result = _sw.sendPacket(tx,rx);
		if (result >= 0)
		{
			return (rx[3] + rx[4] * 256);
		}
		else
		{
			return (result);
		}

		return int16_t();
	}


	/*!
	\brief Sets the mode of the WS2812 LED Driver
	\return 0 or higher for success or a negative number indicating an error code from the Serial Wombat chip.
	*/
	int16_t writeMode(SWWS2812Mode mode)
	{
		uint8_t tx[8] = { 206,_pin,12,(uint8_t)mode,0x55,0x55,0x55,0x55 };
		return _sw.sendPacket(tx);
	}

	/*!
	\brief Display a bargraph using the configured ws2812 class
	
	\param sourcePin  The data source to use for the bargraph
	\param offRGB The color to use for LEDs beyond the bargraph level
	\param onRGB The color to use for LEDs lit by the bargraph
	\param min The public data value (or below) to be treated as the beginning of the bargraph
	\param max The public data value (or above) to be treated as the end of the bargraph
	*/
	int16_t barGraph(uint8_t sourcePin, uint32_t offRGB, uint32_t onRGB, uint16_t min, uint16_t max)
	{
		uint8_t tx[8] = { 206,_pin,12,3,sourcePin,0x55,0x55,0x55 };
		int16_t result = 0;
		result = _sw.sendPacket(tx);  if (result < 0) { return result; }
		result = write(0, offRGB); if (result < 0) { return result; }
		result = write(1, onRGB);  if (result < 0) { return result; }

		uint8_t minMax[8] = { 207,_pin,12,SW_LE16(min), SW_LE16(max),0x55 };
		return _sw.sendPacket(minMax);

		
	}

	/*!
	\brief Swap the Red and Green byte values.  Set this to true for WS2811 chips which reverse the red and green byte order.
	*/
	bool swapRG = false;
private:
	uint8_t _numLEDS = 0;
	uint16_t _userBufferIndex=0;
};
