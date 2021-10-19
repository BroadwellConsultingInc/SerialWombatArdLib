#include "SerialWombat.h"


SerialWombatWS2812::SerialWombatWS2812(SerialWombat& serialWombat)
{
	_sw = &serialWombat;
}

int16_t SerialWombatWS2812::begin(uint8_t pin, uint8_t numberOfLEDs, uint16_t userBufferIndex)
{
	_pin = pin;
	_numLEDS = numberOfLEDs;
	_userBufferIndex = userBufferIndex;

	uint8_t tx[8] = { 200,_pin,12,SW_LE16(userBufferIndex),_numLEDS,0x55 };
	return (_sw->sendPacket(tx));
}

int16_t SerialWombatWS2812::write(uint8_t led, uint32_t color)
{
	uint8_t tx[8] = { 201,_pin,12,led,SW_LE32(color) };
	tx[7] = 0x55;
	return _sw->sendPacket(tx);
}

int16_t SerialWombatWS2812::write(uint8_t led, int16_t color)
{
	return write(led, (uint32_t)color);
}

int16_t SerialWombatWS2812::write(uint8_t led, int32_t color)
{
	return write(led, (uint32_t)color);
}

int16_t SerialWombatWS2812::write(uint8_t led, uint8_t length, uint32_t colors[])
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

int16_t SerialWombatWS2812::writeAnimationLED(uint8_t frame, uint8_t led, uint32_t color)
{
	uint8_t tx[8] = { 203,_pin,12,frame,led,(color >>16 ) & 0xFF,(color >> 8) & 0xFF, color & 0xFF };
	return _sw->sendPacket(tx);
}

int16_t SerialWombatWS2812::writeAnimationLED(uint8_t frame, uint8_t led, int16_t color)
{
	writeAnimationLED(frame, led,(uint32_t)color);
}

int16_t SerialWombatWS2812::writeAnimationLED(uint8_t frame, uint8_t led, int32_t color)
{
	writeAnimationLED(frame, led, (uint32_t)color);
}

int16_t SerialWombatWS2812::writeAnimationFrame(uint8_t frame, uint32_t colors[])
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

int16_t SerialWombatWS2812::writeAnimationFrameDelay(uint8_t frame, uint16_t delay_mS)
{
	uint8_t tx[8] = { 205,_pin,12,frame,SW_LE16(delay_mS),0x55,0x55 };
	return (_sw->sendPacket(tx));
}

int16_t SerialWombatWS2812::writeAnimationUserBufferIndex(uint16_t index, uint8_t numberOfFrames)
{
	uint8_t tx[8] = { 204,_pin,12,SW_LE16(index),numberOfFrames,0x55,0x55 };
	return (_sw->sendPacket(tx));
}

int16_t SerialWombatWS2812::readBufferSize()
{
	uint8_t tx[8] = { 202,_pin,12,_numLEDS,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t result = _sw->sendPacket(tx,rx);
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


int16_t SerialWombatWS2812::setMode(SWWS2812Mode mode)
{
	uint8_t tx[8] = { 206,_pin,12,mode,0x55,0x55,0x55,0x55 };
	return _sw->sendPacket(tx);
}
