#include "SerialWombatUART.h"


SerialWombatUART::SerialWombatUART(SerialWombat& serialWombat):_sw(serialWombat)
{
	_sw = serialWombat;
	
}

void SerialWombatUART::begin(int baudRate, uint8_t pin, uint8_t rxPin, uint8_t txPin)
{
	_rxPin = rxPin;
	_txPin = txPin;
	_pin = pin;
	 switch (baudRate)
	 {
	 case 300:
		 _baudMarker = 0;
		 break;
	 case 1200:
		 _baudMarker = 1;
		 break;

	 case 2400:
		 _baudMarker = 2;
		 break;

	 case 4800:
		 _baudMarker = 3;
		 break;
	 case 9600:
		 _baudMarker = 4;
		 break;
	 case 19200:
		 _baudMarker = 5;
		 break;

	 case 38400:
		 _baudMarker = 6;
		 break;
		
	 case 57600:
		 _baudMarker = 7;
		 break;

	 default:
		 case 115200:
		 _baudMarker = 8;
		 break;
	 }
	uint8_t tx[8] = { 200, _pin,PIN_MODE_UART_RX_TX, _baudMarker,_rxPin,_txPin,0x55, 0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);

}

int SerialWombatUART::available()
{
	uint8_t tx[8] = { 201, _pin,PIN_MODE_UART_RX_TX, 0,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	return (rx[4]);
}

int SerialWombatUART::read()
{
	uint8_t tx[8] = { 202, _pin,PIN_MODE_UART_RX_TX, 1,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
	if (rx[3] != 0)
	{
		return (rx[4]);
	}
	else
	{
		return (-1);
	}
}

void SerialWombatUART::flush()
{
	uint8_t tx[8] = { 200, _pin,PIN_MODE_UART_RX_TX, _baudMarker,_rxPin,_txPin,0x55 };
	uint8_t rx[8];
	_sw.sendPacket(tx, rx);
}

int SerialWombatUART::peek()
{
	uint8_t tx[8] = { 203, _pin,PIN_MODE_UART_RX_TX,0x55,0x55,0x55,0x55,0x55 };
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

size_t SerialWombatUART::write(uint8_t data)
{
	uint8_t tx[8] = { 201, _pin,PIN_MODE_UART_RX_TX,1,data,0x55,0x55,0x55 };
	_sw.sendPacket(tx);
	return (1);
}

size_t SerialWombatUART::write(const uint8_t* buffer, size_t size)
{
	size_t bytesAvailable = 0;
	size_t bytesSent;
	uint32_t timeoutMillis = millis() + timeout;
	
	for (bytesSent = 0; bytesSent  < size ;)
	{

		while (bytesAvailable < 4)
		{
			uint8_t peektx[8] = { 203, _pin,PIN_MODE_UART_RX_TX,0x55,0x55,0x55,0x55,0x55 };
			uint8_t peekrx[8];
			_sw.sendPacket(peektx, peekrx);
			bytesAvailable = peekrx[3];
			if (timeoutMillis < millis())
			{
//			       Serial.printf("UART TX TIMEOUT!\r\n");
				return (bytesSent);
			}
		}
		timeoutMillis = millis() + timeout;

		while (bytesSent < size && bytesAvailable > 0)
		{

			if ((size - bytesSent) < 7 || bytesAvailable < 7)
			{
				uint8_t tx[8] = { 201, _pin,PIN_MODE_UART_RX_TX,0,0x55,0x55,0x55,0x55 };
				uint8_t rx[8];
				size_t txLen;
				tx[3] = 0;
				for (txLen = 0; txLen < 4 && txLen < bytesAvailable && bytesSent < size; ++txLen)
				{
					tx[4 + txLen] = buffer[bytesSent];
					++bytesSent;
					++tx[3];
				}
				_sw.sendPacket(tx, rx);
				bytesAvailable = rx[3];
			}
			else
			{
				uint8_t tx[8] = { 0xB0, 0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
				size_t txLen;
				
				for (txLen = 0; txLen < 7 ; ++txLen)
				{
					tx[1 + txLen] = buffer[bytesSent];
					++bytesSent;
					--bytesAvailable;
				}
				_sw.sendPacket(tx);
			}
		}
	}
	return (bytesSent);
}

int SerialWombatUART::availableForWrite()
{
	uint8_t peektx[8] = { 203, _pin,PIN_MODE_UART_RX_TX,0x55,0x55,0x55,0x55,0x55 };
	uint8_t peekrx[8];
	_sw.sendPacket(peektx, peekrx);
	return peekrx[3];
}

void SerialWombatUART::setTimeout(uint32_t timeout_mS)
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

size_t SerialWombatUART::readBytes(char* buffer, size_t length)
{
	int index = 0;
	int bytesAvailable = 0;
	uint8_t debugBuffer[200];
	uint8_t debuglengthBuffer[200];
	uint8_t debuglengthCount = 0;
	uint32_t timeoutMillis = millis() + timeout;
	while (length > 0 && timeoutMillis > millis())
	{
		int bytecount = 4;
		if (length < 4)
		{
			bytecount = length;
		}
		{
			
			uint8_t tx[8] = { 202, _pin,PIN_MODE_UART_RX_TX, (uint8_t) bytecount,0x55,0x55,0x55,0x55 };
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
			debuglengthBuffer[debuglengthCount++] = bytesReturned;
			for (int i = 0; i < bytesReturned; ++i)
			{
				buffer[index] = rx[i + 4];
				debugBuffer[index] = buffer[index];
				++index;
				--bytesAvailable;
				--length;

			}
		}
		while (bytesAvailable >= 7 && length >= 7)
		{
			uint8_t tx[8] = { 0xB1, 0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			_sw.sendPacket(tx, rx);
			for (int i = 0; i <7; ++i)
			{
				buffer[index] = rx[i + 1];
				debugBuffer[index] = buffer[index];
				++index;
				--bytesAvailable;
				--length;

			}
		}

	}
	/*
	if (timeoutMillis < millis()) //TODO Remove
	{
	       Serial.printf("UART READ TIMEOUT!\r\n");
	}
	*/
	return (index);
}
