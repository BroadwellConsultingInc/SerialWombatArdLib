#include "Arduino.h"
#include "SerialWombat.h"
/*
Copyright 2020-2022 Broadwell Consulting Inc.

Serial Wombat is a registered trademark in the US of Broadwell Consulting Inc.

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

/*! \file SerialWombat.cpp
*/


SerialWombatChip::SerialWombatChip()
{
}



SerialWombatChip::~SerialWombatChip()
{
}





int16_t SerialWombatChip::begin(uint8_t i2cAddress)
{
	//i2cInterface = &Wire;
	//address = i2cAddress;
	return begin(Wire, i2cAddress);
}

int SerialWombatChip::sendPacket(uint8_t tx[], bool retryIfEchoDoesntMatch)
{
	uint8_t rx[8];
	return  (sendPacket(tx,rx, retryIfEchoDoesntMatch));
}

int SerialWombatChip::sendPacket(uint8_t tx[], uint8_t rx[], bool retryIfEchoDoesntMatch, uint8_t startBytesToMatch, uint8_t endBytesToMatch)
{
	uint8_t retry = 4; //TODO communicationErrorRetries;

	
	if (!retryIfEchoDoesntMatch)
	{
		retry = 1;
	}
	
	int result = 0;
	for (int i = 0; i < 8; ++i)
	{
		rx[i] = 0;
	}
	while (retry)
	{
		 result = sendPacket(tx, rx);
		
		if (rx[0] == 'E')
		{
			return result;
		}
		uint8_t i;
		bool success = true;
		for (i = 0; i < startBytesToMatch; ++i)
		{
			if ( tx[i] != rx[i])
			{
				success = false;
			}
		}
		for (i = 8- endBytesToMatch; i < 8; ++i)
		{
			if (tx[i] != rx[i])
			{
				success = false;
			}
		}
		if (success)
		{
			return (result);
		}
		--retry;
		delayMicroseconds(100);
		{
			//char echoTx[] = "!COM_ERR";
			//sendPacket((uint8_t*)echoTx);
			continue;
		}
		delayMicroseconds(100);
		
	}

	return(result);
}

int SerialWombatChip::sendPacket(uint8_t tx[], uint8_t rx[])
{
	if (sendReadyTime != 0 )
	{
		uint32_t currentTime = millis();
		if (currentTime < sendReadyTime)
		{
			delay(sendReadyTime - currentTime);

		}
		sendReadyTime = 0;
		initialize(); 
	}

	if (Serial != NULL)
	{
		if (_currentlyCommunicating)
		{
			//TODO return (-1 * SW_ERROR_REENTRANCY_NOT_SUPPORTED);
		}
		_currentlyCommunicating = true;
		while (Serial->read() >= 0);
		Serial->write(tx, 8);  //TODO add addressing, CRC
		uint32_t millisWrite = millis();
		int bytesRx = 0;

		uint32_t timenow = millis();
		while (bytesRx < 8 && timenow <= (millisWrite + 50))
		{
			int32_t data = Serial->read();
			if (data >= 0)
			{
				rx[bytesRx] = data;
				++bytesRx;
			}
			timenow = millis();
		}
		if (bytesRx < 8)
		{
			if (errorHandler != NULL)
			{
				errorHandler(SW_ERROR_LESS_THAN_8_BYTES_RETURNED,this);
			}
			++errorCount;
			_currentlyCommunicating = false;
			return (-1 * SW_ERROR_LESS_THAN_8_BYTES_RETURNED);
		}
		if (rx[0] == 'E')
		{
			lastErrorCode = returnErrorCode(rx);
			if (errorHandler != NULL)
			{
				errorHandler(lastErrorCode,this);
			}
			lastErrorCode *= -1;
			++errorCount;
			_currentlyCommunicating = false;
			return (lastErrorCode);
		}
		_currentlyCommunicating = false;
		return (8);
	}

	if (i2cInterface != NULL)
	{
		int bytesWritten;
		uint8_t retry = communicationErrorRetries;
		int i2cResult;
		int count = 8;
		//while (tx[count - 1] == 0x55)
		//{
		//	--count;
		//}
		if (_currentlyCommunicating)
		{
			//TODO return (-1 * SW_ERROR_REENTRANCY_NOT_SUPPORTED);
		}
		_currentlyCommunicating = true;
		while (retry)
		{
			--retry;
			i2cInterface->beginTransmission(address);
			bytesWritten = i2cInterface->write(tx, 8);
			i2cResult = i2cInterface->endTransmission();

			if (bytesWritten == 8 && i2cResult == 0)
			{
				retry = 0;
			}
			else
			{
				delayMicroseconds(100);
				continue;
			}
		}
		//delay(3);
		//Serial->setTimeout(10);
		delayMicroseconds(100);
		i2cInterface->requestFrom(address, (uint8_t)8);

		count = 0;
		int r = 0;
		while (r >= 0 && count < 8)
		{
			r = i2cInterface->read();

			if (r >= 0)
			{
				rx[count] = (uint8_t) r;
				++count;
			}
			else
			{
				break;
			}
		}

		if (count < 8)
		{
			delayMicroseconds(100);
			i2cInterface->requestFrom(address, (uint8_t)8);

			count = 0;
			while (i2cInterface->available() && count < 8)
			{
				int r = i2cInterface->read();

				if (r >= 0)
				{
					rx[count] = (uint8_t)r;
					++count;
				}
				else
				{
					break;
				}
			}
			if (count < 8)
			{
				if (errorHandler != NULL)
				{
					errorHandler(SW_ERROR_LESS_THAN_8_BYTES_RETURNED, this);
				}
				++errorCount;
				_currentlyCommunicating = false;
				return (-1 * SW_ERROR_LESS_THAN_8_BYTES_RETURNED);
			}
		}
		if (rx[0] == 'E')
		{
			lastErrorCode = returnErrorCode(rx);
			if (errorHandler != NULL)
			{
				errorHandler(lastErrorCode,this);
			}
			lastErrorCode *= -1;
			++errorCount;
			_currentlyCommunicating = false;
			return (lastErrorCode);
		}
	}
	_currentlyCommunicating = false;
	return(0);

}
int SerialWombatChip::sendPacketNoResponse(uint8_t tx[])
{

	if (Serial != NULL)
	{
		Serial->write(tx, 8);  //TODO add addressing, CRC
		return (8);
	}

	if (i2cInterface != NULL)
	{
		i2cInterface->beginTransmission(address);
		i2cInterface->write(tx, 8);
		i2cInterface->endTransmission();
	}
	return(0);

}

int SerialWombatChip::sendPacket(uint8_t tx[])
{
	uint8_t rx[8];
	
	if (_asleep)
	{
		_asleep = false;
		uint8_t txw[8] = { '!','!','!','!','!','!','!','!' };  //TODO adapt for UART
		sendPacket(txw);
		delayMicroseconds(200);
	}

	return(sendPacket(tx,rx));

	if (sendReadyTime != 0)
	{
		unsigned long currentTime = millis();
		if (currentTime < sendReadyTime)
		{
			delay(sendReadyTime - currentTime);
			
		}
		sendReadyTime = 0;
		initialize();
	}

	if (Serial != NULL)
	{
		return Serial->write(tx, 8);  //TODO add addressing, CRC		
	}

	if (i2cInterface != NULL)
	{
		i2cInterface->beginTransmission(address);
		i2cInterface->write(tx, 8);
		i2cInterface->endTransmission();
	}
	return(0);

}

void SerialWombatSerialErrorHandlerBrief(uint16_t error, SerialWombatChip* sw)
{
	char s[10];
	Serial.print("Comm Error on SW at addr ");
	sprintf(s, "0x%02X ", sw->address);
	Serial.print(s);
	Serial.print(" Error code ");
	
		Serial.print(error);

		Serial.print(" Error count ");
		Serial.print(sw->errorCount);
	
		uint8_t errorCmd[8];
		if (sw->readLastErrorCommand(errorCmd) >= 0)
		{
			Serial.print(" Command: ");
			for (int i = 0; i < 8; ++i)
			{
				sprintf(s, "0x%02X ", errorCmd[i]);
					Serial.print(s);
			}
		}

		Serial.println();
}

