#pragma once
#include "Stream.h"
#include "SerialWombat.h"


class SerialWombatUART :
    public Stream
{
public:
    SerialWombatUART(SerialWombat& serialWombat);
    void begin(int baudRate,uint8_t pin, uint8_t rxPin, uint8_t txPin);
    int available();
    int read();
    void flush();
    int peek();
    size_t write(uint8_t data);
    size_t write(const uint8_t* buffer, size_t size);
    int availableForWrite();
    size_t readBytes(char* buffer, size_t length);

private:
     SerialWombat& _sw;
     uint8_t _rxPin  = 255;
     uint8_t _txPin = 255;
     uint8_t _pin = 255;
     uint8_t _baudMarker = 0;
};

