#include <SerialWombat.h>
#include "SerialWombatUltrasonicDistanceSensor.h"

SerialWombatUltrasonicDistanceSensor::SerialWombatUltrasonicDistanceSensor(SerialWombatChip& serialWombat) :SerialWombatPin(serialWombat), SerialWombatAbstractProcessedInput(serialWombat)
{
}

int16_t SerialWombatUltrasonicDistanceSensor::begin(uint8_t echoPin, driver driver, uint8_t triggerPin, bool autoTrigger, bool pullUp)
{
	_pin = echoPin;
	_pinMode = PIN_MODE_ULTRASONIC_DISTANCE;

	uint8_t tx[] = { 200,_pin,_pinMode,(uint8_t)driver, triggerPin, (uint8_t)pullUp,(uint8_t)autoTrigger, 0x55 };
	return(_sw.sendPacket(tx));
}

uint16_t SerialWombatUltrasonicDistanceSensor::readPulseCount()
{

	uint8_t tx[] = { 202,_pin,_pinMode,0x55,0x55,0x55,0x55, 0x55 };
	uint8_t rx[8];
	if(_sw.sendPacket(tx,rx) >= 0)
	{
		return (rx[5] + 256 * rx[6]);
	}
	else
	{
		return 0;
	}


}

int16_t SerialWombatUltrasonicDistanceSensor::manualTrigger()
{
	uint8_t tx[] = { 201,_pin,_pinMode,1,0x55,0x55,0x55, 0x55 };
	
	return _sw.sendPacket(tx);
}
