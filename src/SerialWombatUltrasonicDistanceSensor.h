#pragma once
/*
Copyright 2020-2023 Broadwell Consulting Inc.

"Serial Wombat" is a registered trademark of Broadwell Consulting Inc. in
the United States.  See SerialWombat.com for usage guidance.

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


#include <stdint.h>
#include "SerialWombat.h"


/*! \file SerialWombatUltrasonicDistanceSensor.h
*/

/*! \brief A Class which uses one or two Serial Wombat 18AB pins to measure distance using an Ultrasonic distance sensor.  


A video Tutorial on this pin mode is available:

\htmlonly
//TODO
\endhtmlonly

//TODO https://youtu.be/

Documentation for the SerialWombatUltrasonicDistanceSensor class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_ultrasonic_distance_sensor.html


*/

class SerialWombatUltrasonicDistanceSensor : public SerialWombatPin, public SerialWombatAbstractProcessedInput
{
public:
	/// \brief Class constructor for SerialWombatPulseTimer
	/// \param serialWombat The Serial Wombat chip on which the SerialWombatPulseTimer pinmode will be run
	SerialWombatUltrasonicDistanceSensor(SerialWombatChip& serialWombat): SerialWombatPin(serialWombat),SerialWombatAbstractProcessedInput(serialWombat)
	{}

	enum driver {
	HC_SR04 = 0,  ///< Standard buffered mode.  Colors are uploaded by the host
	};

	
	/// \brief Initialization routine for SerialWombatUltrasonicDistanceSensor
	/// 
	/// \param echoPin Pin used to time input pulses.  For 5V sensors, 5V tolerant pins 9,10,11,12, 14 and 15 are good choices
	/// \param driver Chip used for distance measurement.  Currently only HC_SR04 is supported.
	/// \param triggerPin Pin used for triggering the sensor.  If same as echo pin (e.g. 3 pin sensors) set equal to echoPin
	/// \return 0 or higher if successful, negative error code if not successful.
	int16_t begin(uint8_t echoPin, driver driver, uint8_t triggerPin,bool autoTrigger = true, bool pullUp = false)
	{
		_pin = echoPin;
		_pinMode = PIN_MODE_ULTRASONIC_DISTANCE;

		uint8_t tx[] = { 200,_pin,_pinMode,(uint8_t)driver, triggerPin, (uint8_t)pullUp,(uint8_t)autoTrigger, 0x55 };
		return(_sw.sendPacket(tx));
	}

	/// \brief get the number of pulses that have been sent.  
	///
	/// \return The number of pulses that have been sent.  Rolls over at 65536
	uint16_t readPulseCount()
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

	/// \brief Manually trigger a distance reading
	///
	/// Use this interface to trigger a reading if begin was called with autoTrigger = false
	/// \return 0 or higher if successful, negative error code if not successful.
	int16_t manualTrigger()
	{
		uint8_t tx[] = { 201,_pin,_pinMode,1,0x55,0x55,0x55, 0x55 };
		
		return _sw.sendPacket(tx);
	}

  /// \brief Configure a Servo sweep of Ultrasonic Distance Sensor
        
         int16_t configureServoSweep(uint8_t servoPin,uint16_t memoryIndex,uint16_t servoPositions_,uint16_t servoIncrement, bool reverse = false,  uint16_t servoMoveDelay = 100, uint16_t servoReturnDelay = 1000)
        {
            {
                uint8_t tx[] = { 203, _pin, _pinMode, servoPin, (uint8_t)(memoryIndex & 0xFF), (uint8_t)(memoryIndex >>8),
                (uint8_t)(servoPositions & 0xFF),(uint8_t)(servoPositions >>8)};
                int16_t result = _sw.sendPacket(tx);
                if (result < 0) { return result; }
            }
            {
                uint8_t tx[] = {204, _pin, _pinMode,  (uint8_t)(servoIncrement & 0xFF), (uint8_t)(servoIncrement >> 8),
                0x55,0x55, (uint8_t)(reverse?1:0)};
                int16_t result = _sw.sendPacket(tx);
                if (result < 0) { return result; }
            }
            {
                uint8_t tx[] = {205, _pin, _pinMode,  (uint8_t)(servoMoveDelay & 0xFF), (uint8_t)(servoMoveDelay >> 8),
                (uint8_t)(servoReturnDelay & 0xFF),(uint8_t)(servoReturnDelay >>8), 0x55};
                int16_t result = _sw.sendPacket(tx);
                if (result < 0) { return result; }
            }
            servoMemoryIndex = memoryIndex;
            servoPositions = servoPositions_;
            return 0;
        }
         int16_t enableServoSweep(bool enable)
        {
            uint8_t tx[] = {206, _pin, _pinMode,  (uint8_t)(enable ? 1 : 0),0x55,0x55,0x55, 0x55};
           return _sw.sendPacket(tx);
        }

         uint16_t readServoSweepEntry(uint16_t servoSweepEntry)
        {
            uint8_t b[2];
            _sw.readUserBuffer((uint16_t)(servoMemoryIndex + 2 * servoSweepEntry), b, 2);
            return ((uint16_t)(b[0] + 256 * b[1]));
        }

         int16_t readServoSweepEntries(uint16_t* entries, uint16_t count)
        {
           return  _sw.readUserBuffer(servoMemoryIndex,(uint8_t*) entries,(uint16_t)( 2 * count));
            
        }

	/// \brief Facilitates Inheritance
	uint8_t pin() { return _pin; }
	/// \brief Facilitates Inheritance
	uint8_t swPinModeNumber() { return _pinMode; }
	
	uint16_t servoMemoryIndex = 0;
	uint16_t servoPositions = 1;
private:

};
