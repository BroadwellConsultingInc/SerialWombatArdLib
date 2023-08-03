#include "SerialWombat.h"
#include <stdint.h>
#include "SerialWombatPS2Keyboard.h"



SerialWombat18ABVGA::SerialWombat18ABVGA(SerialWombatChip& serialWombat):SerialWombatPin(serialWombat)
{
}

int16_t SerialWombat18ABVGA::begin(uint8_t vsyncPin, uint16_t bufferIndex)
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

int16_t SerialWombat18ABVGA::writePixel(uint8_t x, uint8_t y, uint8_t color)
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

int16_t SerialWombat18ABVGA::fillScreen(uint8_t color)
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

int16_t SerialWombat18ABVGA::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
						_pin,
						(uint8_t)_pinMode ,
						2, //Fill Rect
						x,
						y,
						(uint8_t)(x + w - 1),
		(uint8_t)(y+h - 1)

	};
	if (color == 0)
	{
		tx[3] = 3; // Make clear rect
	}
	return _sw.sendPacket(tx);
}

int16_t SerialWombat18ABVGA::setLineColor(uint8_t color, uint8_t start)
{
	return setLineColor(color,start,start);
}

int16_t SerialWombat18ABVGA::setLineColor(uint8_t color, uint8_t start, uint8_t end)
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
