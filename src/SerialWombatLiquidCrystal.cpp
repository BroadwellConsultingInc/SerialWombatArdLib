#include "SerialWombat.h"
#include "SerialWombatLiquidCrystal.h"

int16_t SerialWombatLiquidCrystal::scrollDisplayLeft()
{
	return command(0x18); // Display Move left LCD
	
}

int16_t SerialWombatLiquidCrystal::scrollDisplayRight()
{
	return command(0x1C); // Display Move right LCD
}

int16_t SerialWombatLiquidCrystal::autoscroll()
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

int16_t SerialWombatLiquidCrystal::noAutoscroll()
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

int16_t SerialWombatLiquidCrystal::leftToRight()
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

size_t SerialWombatLiquidCrystal::write(uint8_t data)
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
int16_t SerialWombatLiquidCrystal::createChar(uint8_t index, uint8_t bitmap[])
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
size_t SerialWombatLiquidCrystal::write(uint8_t* buffer, size_t count)
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

int16_t SerialWombatLiquidCrystal::rightToLeft()
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

int16_t SerialWombatLiquidCrystal::setRowOffsets(uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4)
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

}

int16_t SerialWombatLiquidCrystal::command(uint8_t cmd)
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

int16_t SerialWombatLiquidCrystal::initializeBufferCopy(uint16_t bufferIndex, uint8_t width)
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

int16_t SerialWombatLiquidCrystal::beginE2(uint8_t e2Pin)
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

SerialWombatLiquidCrystal::SerialWombatLiquidCrystal(SerialWombatChip& serialWombat, uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7):SerialWombatPin(serialWombat)
{
	_pinMode = PIN_MODE_LIQUIDCRYSTAL;
	_pin = enable;
	_rs = rs;
	_d4 = d4;
	_d5 = d5;
	_d6 = d6;
	_d7 = d7;

}

int16_t SerialWombatLiquidCrystal::begin(uint8_t cols, uint8_t rows)
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

int16_t SerialWombatLiquidCrystal::clear(bool delayAfterClear )
{
	return command(0x01);  // Clear display
	if (delayAfterClear)
	{
		delay(2);
	}
}

int16_t SerialWombatLiquidCrystal::home(bool delayAfterHome)
{
	return command(0x02);  // Clear display
	if (delayAfterHome)
	{
		delay(2);
	}
}

int16_t SerialWombatLiquidCrystal::setCursor(uint8_t col, uint8_t row)
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

int16_t SerialWombatLiquidCrystal::noCursor()
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

int16_t SerialWombatLiquidCrystal::cursor()
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

int16_t SerialWombatLiquidCrystal::noBlink()
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

int16_t SerialWombatLiquidCrystal::display()
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

int16_t SerialWombatLiquidCrystal::noDisplay()
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

int16_t SerialWombatLiquidCrystal::blink()
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
