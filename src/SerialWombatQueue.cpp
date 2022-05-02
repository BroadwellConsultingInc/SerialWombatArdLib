#include "SerialWombat.h"
#include "SerialWombatQueue.h"

SerialWombatQueue::SerialWombatQueue(SerialWombatChip& serialWombat) :_sw(serialWombat)
{
	_sw = serialWombat;
}

int16_t SerialWombatQueue::begin(uint16_t index, uint16_t length, SerialWombatQueueType qtype)
{
	startIndex = index;
	length = length;
	uint8_t tx[] = {(uint8_t) SerialWombatCommands::COMMAND_BINARY_QUEUE_INITIALIZE ,SW_LE16(index),SW_LE16(length),(uint8_t)qtype, 0x55,0x55 };
	uint8_t rx[8];
	int16_t result =  _sw.sendPacket(tx,rx);
	if (result < 0)
	{
		return result;
	}
	else
	{
		return ((int16_t)(rx[3] + 256 * rx[4]));
	}
}

int SerialWombatQueue::available()
{
	uint8_t tx[] = {(uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult =  _sw.sendPacket(tx,rx);
	if (sendResult >= 0)
	{
		return (rx[4] + 256 * rx[5]);
	}
	return (0);
}

int SerialWombatQueue::read()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_READ_BYTES ,SW_LE16(startIndex),1,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		if (rx[1] == 1)
		return (rx[2]);
	}
	return (-1);
}

void SerialWombatQueue::flush()
{
	begin(startIndex, length);
}

int SerialWombatQueue::peek()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		if ((rx[4] + 256 * rx[5]) > 0)
		{
			return(rx[3]);
		}
	}
	return (-1);
}

size_t SerialWombatQueue::write(uint8_t data)
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_BYTES ,SW_LE16(startIndex),1,data,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		
			return(rx[3]);
	}
	return (0);
}

size_t SerialWombatQueue::write(const uint8_t* buffer, size_t size)
{
	uint8_t nextWriteSize;
	uint16_t bytesWritten = 0;
	uint32_t startTime = millis();

	//Write up to the first 4 bytes
	if (size >= 4)
	{
		nextWriteSize = 4;
	}
	else
	{
		nextWriteSize = (uint8_t)size;
	}
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_BYTES ,SW_LE16(startIndex),0,0x55,0x55,0x55,0x55 };
		uint8_t i;
		for (i = 0; i < nextWriteSize; ++i)
		{
			tx[4 + i] = buffer[i];
		}
		tx[3] = nextWriteSize;
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult < 0)
		{
			return(bytesWritten);
		}
		bytesWritten += rx[3];
	}
	while ((size - bytesWritten) >= 7)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_7BYTES ,
			buffer[bytesWritten],
			buffer[bytesWritten + 1],
			buffer[bytesWritten + 2],
			buffer[bytesWritten + 3],
			buffer[bytesWritten + 4],
			buffer[bytesWritten + 5],
			buffer[bytesWritten + 6],
			buffer[bytesWritten + 7] };

		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult < 0)
		{
			return(bytesWritten);
		}
		bytesWritten += rx[3];
		delay(0);
		if (millis() > startTime + _timeout)
		{
			return(bytesWritten);
		}
	}

	while (size - bytesWritten > 0)
	{
		if (size - bytesWritten >= 4)
		{
			nextWriteSize = 4;
		}
		else
		{
			nextWriteSize = (uint8_t)(size - bytesWritten);
		}
		{
			uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_ADD_BYTES ,SW_LE16(startIndex),0,0x55,0x55,0x55,0x55 };
			uint8_t i;
			for (i = 0; i < nextWriteSize; ++i)
			{
				tx[4 + i] = buffer[i + bytesWritten];
			}
			tx[3] = nextWriteSize;
			uint8_t rx[8];
			int16_t sendResult = _sw.sendPacket(tx, rx);
			if (sendResult < 0)
			{
				return(bytesWritten);
			}
			bytesWritten += rx[3];
		}
		if (millis() > startTime + _timeout)
		{
			return(bytesWritten);
		}
	}
	return (bytesWritten);
}

int SerialWombatQueue::availableForWrite()
{
	uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	int16_t sendResult = _sw.sendPacket(tx, rx);
	if (sendResult >= 0)
	{
		return (rx[6] + 256 * rx[7]);
	}
	return (0);
}

size_t SerialWombatQueue::readBytes(char* buffer, size_t length)
{
	uint16_t bytesAvailable = 0;
	uint32_t startTime = millis();

	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_INFORMATION ,SW_LE16(startIndex),0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult >= 0)
		{
			bytesAvailable = (rx[4] + 256 * rx[5]);
		}

		if (bytesAvailable < length)
		{
			length = bytesAvailable;
		}
	}
	uint16_t bytesRead = 0;
	while (bytesRead < length)
	{
		uint8_t bytesToRead =(uint8_t)( length - bytesRead);
		if (bytesToRead > 6)
		{
			bytesToRead = 6;
		}
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_QUEUE_READ_BYTES ,SW_LE16(startIndex),bytesToRead,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		int16_t sendResult = _sw.sendPacket(tx, rx);
		if (sendResult >= 0)
		{
			uint8_t i;
				for (i = 0; i < rx[1] && bytesRead < length; ++i)
				{
					buffer[bytesRead] = rx[2 + i];
					++bytesRead;
				}
			
		}
		else
		{
			return (bytesRead);
		}
		if (millis() > startTime + _timeout)
		{
			return(bytesRead);
		}

	}
	return bytesRead;
}

void SerialWombatQueue::setTimeout(long timeout_mS)
{
	_timeout = timeout_mS;
}
