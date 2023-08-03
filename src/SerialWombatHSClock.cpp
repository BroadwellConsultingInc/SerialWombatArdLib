#include "SerialWombatHSClock.h"


SerialWombatHSClock::SerialWombatHSClock(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat)
{
}

int32_t SerialWombatHSClock::begin(uint8_t pin, uint32_t frequency_Hz)
{
    _pin = pin;
    _pinMode = PIN_MODE_HS_CLOCK;

    uint8_t tx[] =
    {
        (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
        _pin,
        _pinMode,
        SW_LE32(frequency_Hz),
        0x55
    };
    uint8_t rx[8];
    int16_t result = _sw.sendPacket(tx, rx);
    if (result < 0)
    {
        return (result);
    }

    int32_t returnval = (((uint32_t)rx[5]) << 16) + (((uint32_t)rx[4]) << 8) + rx[3];
    return (returnval);
}

int16_t SerialWombatHSClock::disable()
{
    uint8_t tx[] =
    {
        (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE_DISABLE,
        _pin,
        _pinMode,
        0x55,0x55,0x55,0x55,0x55
    };
   return _sw.sendPacket(tx);
}
