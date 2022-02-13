#include "SerialWombat18CapTouch.h"

SerialWombat18CapTouch::SerialWombat18CapTouch(SerialWombatChip& serialWombat) :SerialWombatAbstractButton(),SerialWombatPin(serialWombat)
{
    _sw = serialWombat;
}



int16_t SerialWombat18CapTouch::begin(uint8_t pin, uint16_t chargeTime, uint16_t delay)
{
    _pin = pin;
    _pinMode = PIN_MODE_SW18AB_CAPTOUCH;

    uint8_t tx[8] = { 200,_pin, 22, SW_LE16(chargeTime), SW_LE16(delay),0x55 };

    return _sw.sendPacket(tx);
}

int16_t SerialWombat18CapTouch::makeDigital(uint16_t touchLimit, uint16_t noTouchLimit)
{
    return makeDigital(touchLimit, noTouchLimit, 1, 0, 0, 0);
}

int16_t SerialWombat18CapTouch::makeDigital(uint16_t touchLimit, uint16_t noTouchLimit, uint16_t touchValue, uint16_t noTouchValue, bool invert, uint16_t debounceCount)
{
    uint8_t tx1[8] = { 201,_pin,22,SW_LE16(touchLimit), SW_LE16(noTouchLimit),0x55 };
    int16_t result = _sw.sendPacket(tx1);
    if (result < 0)
    {
        return (result);
    }

    _trueOutput = touchValue;
    _falseOutput = noTouchValue;
    uint8_t tx2[8] = { 202,_pin,22,SW_LE16(touchValue), SW_LE16(noTouchValue),0x55 };
    result = _sw.sendPacket(tx2);
    if (result < 0)
    {
        return (result);
    }
    uint8_t tx3[8] = { 203,_pin,22,1, invert? (uint8_t)1: (uint8_t)0,SW_LE16(debounceCount),0x55 };
    result = _sw.sendPacket(tx3);
    if (result < 0)
    {
        return (result);
    }
    return(0);


}

int16_t SerialWombat18CapTouch::makeAnalog()
{
    uint8_t tx3[8] = { 203,_pin,22,0, 0x55,0x55,0x55,0x55 };
   return _sw.sendPacket(tx3);
}

bool SerialWombat18CapTouch::digitalRead()
{
    uint8_t tx[8] = { 204,_pin,22,0,0x55,0x55,0x55,0x55 };
    uint8_t rx[8];
    if (_sw.sendPacket(tx, rx) >= 0)
    {
        return(rx[3] > 0);
    }
    return(false);

}

uint16_t SerialWombat18CapTouch::readDurationInTrueState_mS()
{
    uint8_t tx[8] = { 204,_pin,22,1,0x55,0x55,0x55,0x55 };
    uint8_t rx[8];
    _sw.sendPacket(tx, rx);

    transitions += (256 * rx[5] + rx[4]);
    if (rx[3] == 0)
    {
        return (0);
    }
    else
    {
        return(256 * rx[7] + rx[6]);
    }
}

uint16_t SerialWombat18CapTouch::readDurationInFalseState_mS()
{
    uint8_t tx[8] = { 204,_pin,22,1,0x55,0x55,0x55,0x55 };
    uint8_t rx[8];
    _sw.sendPacket(tx, rx);

    transitions += (256 * rx[5] + rx[4]);

    if (rx[3] == 1)
    {
        return (0);
    }
    else
    {
        return(256 * rx[7] + rx[6]);
    }
}

bool SerialWombat18CapTouch::readTransitionsState()
{
    uint8_t tx[8] = { 204,_pin,22,1,0x55,0x55,0x55,0x55 };
    uint8_t rx[8];
    _sw.sendPacket(tx, rx);
    transitions = (256 * rx[5] + rx[4]);
    return (rx[3] > 0);
}

