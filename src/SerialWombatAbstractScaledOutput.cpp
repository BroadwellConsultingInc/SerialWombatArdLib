#include "SerialWombat.h"




SerialWombatAbstractScaledOutput::SerialWombatAbstractScaledOutput(SerialWombatChip& sw):_asosw(sw)
{
}

int16_t SerialWombatAbstractScaledOutput::writeTimeout(uint16_t timeout_mS, uint16_t timeoutOutputValue)
{

	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		1,
		SW_LE16(timeout_mS),
		SW_LE16(timeoutOutputValue),
		};
	
	int16_t result = _asosw.sendPacket(tx);

	return(result);

}

int16_t SerialWombatAbstractScaledOutput::writeScalingEnabled(bool enabled, uint8_t sourcePin)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),	swPinModeNumber(),
		0, //Enable/disable scaling, set source pin
		enabled,
		sourcePin,
		0x55, 0x55};
	
	int16_t result = _asosw.sendPacket(tx);
	return(result);
}

int16_t SerialWombatAbstractScaledOutput::writeInputScaling(uint16_t inputMin, uint16_t inputMax)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		2,  // Set input scaling
		SW_LE16(inputMin),
		SW_LE16(inputMax),
		};
	return(_asosw.sendPacket(tx));
}

int16_t SerialWombatAbstractScaledOutput::writeOutputScaling(uint16_t outputMin, uint16_t outputMax)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		5,  //Set output scaling
		SW_LE16(outputMin),
		SW_LE16(outputMax),
		};
	return(_asosw.sendPacket(tx));
}

int16_t SerialWombatAbstractScaledOutput::writeScalingInvertedInput(bool inverted)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		3,  // Set inverted/not inverted
		inverted,
		0x55,0x55,0x55,
	};
	return(_asosw.sendPacket(tx));
}

int16_t SerialWombatAbstractScaledOutput::writeScalingTargetValue(uint16_t target)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		6,  // Set target value for PID controller
		SW_LE16(target),0x55,0x55,
	};
	return(_asosw.sendPacket(tx));
}

int16_t SerialWombatAbstractScaledOutput::writeRateControl(SerialWombatAbstractScaledOutput::Period sampleRate, uint16_t filterConstant)
{
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			4, // Set filter mode
			1, // Filter mode rate control
			SW_LE16(filterConstant),0x55,
		};
		int16_t result = _asosw.sendPacket(tx);
		if (result < 0)
		{
			return(result);
		}
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		7, // Set Sample Rate
		sampleRate,
		0x55,0x55,0x55,
	};
	return(_asosw.sendPacket(tx));
	}
}

int16_t SerialWombatAbstractScaledOutput::write1stOrderFiltering(Period sampleRate, uint16_t filterConstant)
{
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			4, // Set filter mode
			2, // Filter mode 1st Order
			SW_LE16(filterConstant),0x55,
		};
		int16_t result = _asosw.sendPacket(tx);
		if (result < 0)
		{
			return(result);
		}
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		7, // Set Sample Rate
		sampleRate,
		0x55,0x55,0x55,
		};
		return(_asosw.sendPacket(tx));
	}
}

int16_t SerialWombatAbstractScaledOutput::writeHysteresis(uint16_t lowLimit, uint16_t lowOutputValue, uint16_t highLimit, uint16_t highOutputValue, uint16_t initialOutputValue)
{
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			50, // Set hysteresis high limit/output
			SW_LE16(highLimit),
			SW_LE16(highOutputValue)
		};
		int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
	}

	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			51, // Set hysteresis low limit/output
			SW_LE16(lowLimit),
			SW_LE16(lowOutputValue)
		};
		int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			52, // Set hysteresis low limit/output
			SW_LE16(initialOutputValue),
			0x55,0x55
		};
		int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
	}
	return(0);
}

int16_t SerialWombatAbstractScaledOutput::writePID(uint16_t kp, uint16_t ki, uint16_t kd, uint16_t target, SerialWombatAbstractScaledOutput::Period sampleRate)
{
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			100, // Set kp and ki
			SW_LE16(kp),
			SW_LE16(ki)
		};
		int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			101, // Set kd
			SW_LE16(kd),
			0x55,0x55
		};
		int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
	}
	writeScalingTargetValue(target);
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
		pin(),
		swPinModeNumber(),
		7, // Set Sample Rate
		sampleRate,
		0x55,0x55,0x55,
		};
		int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			102, // Reset Integrator
			0x55,0x55,0x55,0x55
		};
		int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
	}
	return 0;

}
