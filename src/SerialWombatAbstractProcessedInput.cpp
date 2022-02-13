#include "SerialWombat.h"

SerialWombatAbstractProcessedInput::SerialWombatAbstractProcessedInput(SerialWombatChip& sw):_pisw(sw)
{

}

int16_t SerialWombatAbstractProcessedInput::writeProcessedInputEnable(bool enabled)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		0,
		(uint8_t)enabled,
		0x55,0x55,0x55
	};

	int16_t result = _pisw.sendPacket(tx);

	return(result);
}

uint16_t SerialWombatAbstractProcessedInput::readMinimum(bool reset)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		9,
		(uint8_t)reset,
		0x55,0x55,0x55
	};
	uint8_t rx[8];
	int16_t result = _pisw.sendPacket(tx,rx);
	if (result < 0)
	{
		return (65535);
	}
	return(rx[4] + 256*rx[5]);
}

uint16_t SerialWombatAbstractProcessedInput::readMaximum(bool reset)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		10,
		(uint8_t)reset,
		0x55,0x55,0x55
	};
	uint8_t rx[8];
	int16_t result = _pisw.sendPacket(tx, rx);
	if (result < 0)
	{
		return (65535);
	}
	return(rx[4] + 256 * rx[5]);
}

uint16_t SerialWombatAbstractProcessedInput::readAverage()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		11,
		0x55,0x55,0x55,0x55
	};
	uint8_t rx[8];
	int16_t result = _pisw.sendPacket(tx, rx);
	if (result < 0)
	{
		return (0);
	}
	return(rx[4] + 256 * rx[5]);
}

uint16_t SerialWombatAbstractProcessedInput::readFiltered()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		11,
		0x55,0x55,0x55,0x55
	};
	uint8_t rx[8];
	int16_t result = _pisw.sendPacket(tx, rx);
	if (result < 0)
	{
		return (0);
	}
	return(rx[6] + 256 * rx[7]);
}

int16_t SerialWombatAbstractProcessedInput::writeInverted(bool inverted)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		3,
		(uint8_t) inverted,
		0x55,0x55,0x55
	};

	int16_t result = _pisw.sendPacket(tx);

	return(result);
}

int16_t SerialWombatAbstractProcessedInput::writeFirstOrderFilteringConstant(uint16_t constant)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		11,
		SW_LE16(constant),
		0x55,0x55
	};
	return (_pisw.sendPacket(tx));
}

int16_t SerialWombatAbstractProcessedInput::writeAveragingNumberOfSamples(uint16_t numberOfSamples)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		1,
		SW_LE16(numberOfSamples),
		0x55,0x55
	};

	int16_t result = _pisw.sendPacket(tx);

	return(result);
}

int16_t SerialWombatAbstractProcessedInput::writeExcludeBelowAbove(uint16_t low, uint16_t high)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		2,
		SW_LE16(low),
		SW_LE16(high)
	};

	int16_t result = _pisw.sendPacket(tx);

	return(result);
}

int16_t SerialWombatAbstractProcessedInput::configureQueue(SerialWombatQueue* queue, Frequency frequency, bool queueHighByte, bool queueLowByte)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		5,
		SW_LE16(queue->startIndex)
		,(uint8_t)frequency,
		(uint8_t)((((uint8_t) queueHighByte) << (uint8_t)1) | (uint8_t)queueLowByte)
	};

	int16_t result = _pisw.sendPacket(tx);

	return(result);
}

int16_t SerialWombatAbstractProcessedInput::configureOutputValue(OutputValue outputValue)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		4,
		(uint8_t)outputValue,
		0x55,0x55,0x55
	};

	int16_t result = _pisw.sendPacket(tx);

	return(result);
}

int16_t SerialWombatAbstractProcessedInput::configureTransformScaleRange(uint16_t min, uint16_t max)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		6,
		SW_LE16(min),
		SW_LE16(max)
	};

	int16_t result = _pisw.sendPacket(tx);

	return(result);
}

int16_t SerialWombatAbstractProcessedInput::configureTransformLinearMXB(int32_t m, int32_t b)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		7,
		SW_LE32(((uint32_t)m)),
	};

	int16_t result = _pisw.sendPacket(tx);
	if (result < 0)
	{
		return(result);
	}

	uint8_t tx2[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_INPUTPROCESS,
		pin(),
		swPinModeNumber(),
		8,
		SW_LE32(((uint32_t)b)),
	};
    result = _pisw.sendPacket(tx2);
	return(result);
}
