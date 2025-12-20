#pragma once
/*
Copyright 2025 Broadwell Consulting Inc.

"Serial Wombat" is a registered trademark of Broadwell Consulting Inc. in
the United States.  See SerialWombat.com for usage guidance.

Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
*/
/*! 
@file PCB0031_Grip.h
*/
#include "SerialWombat.h"

/*!

@brief A class representing an Allegro ACS712 5A current sensor connected to a SW8B Analog input on a PCB0031 Grip board

The GripACS712 class is a child class of SerialWombatAnalogInput_18AB.  It has all of the same methods and adds methods
for converting the output to mA, and setting a reference zero current.  This can be either real zero current, or an offset
such as in the case of a Servo which pulls a small current to drive its electronics even when the motor is not generating
torque.

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/


class GripACS712 : public SerialWombatAnalogInput_18AB
{
public:
	/*!
	\brief Constructor for the GripACS712 Class
	
	@param sw A reference to a previously declared SerialWombatChip to which the Servo is connected.
	*/
	GripACS712(SerialWombatChip& serialWombatChip) :SerialWombatAnalogInput_18AB(serialWombatChip) {}

	/*!	
	@brief Initialize a GripACS712 on the specified pin
	
	This function must be called after the SerialWombatChip instance specified in the constructor
	has been initialized with a begin call.
	
	This function initializes the underlying analog input with default parameters.
	@param pin The Serial Wombat pin to set.  Valid values are 4-7 as those pins are ACS712 pins on the grip board
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/

	int16_t begin(uint8_t pin)
	{
		if (pin < 4 || pin > 7)
		{
			return ((int16_t)(-1 * SW_ERROR_PIN_NOT_CAPABLE));
		}
		return SerialWombatAnalogInput_18AB::begin(pin);
	}

	/*! 	
	@brief Variable holding the zero calibration value in counts
	*/
	uint16_t zeroCurrentCalibration = 32768;

	/*!
	@brief called to set the zero calibration current
	
	@return returns the value in counts of the calibration
	*/
	uint16_t calibrateIdleCurrent()
	{
		
		zeroCurrentCalibration = readAveragedCounts();
		return zeroCurrentCalibration;
	}

	/*!
	@brief returns the current reading in Milliamps.  May be negative
	
	@return returns the current value in mA
	*/
	int16_t readCurrent_mA()
	{
		volatile uint16_t initialReading = readAveragedCounts();
		//ACS712 moves 185mV / A.
		// 65535 / 5000mV = 13.107 counts / mV	
		// 2425 counts / A
		int32_t reading = initialReading;
		reading -= zeroCurrentCalibration;
		reading *= 1000;  // Go to mA
		reading /= 2425;
		int16_t result = reading;
		return (result);

	}
};

/*!

@brief A class representing a servo output and an Allegro ACS712 5A current sensor connected to a SW8B Analog input on a PCB0031 Grip board

The GripServo class is a child class of SerialWombatServo_18AB.  It has all of the same methods.  It adds a GripACS712 named sensor
which corresponds to the sensor attached to that servo, allowing a single class to represent both the servo and its associated sensor.

This class includes methods for automatically determinging the range of motion of a servo based mechanism based on current draw, and 
methods for automatically gripping and releasing an object with force feedback when a servo based gripping mechanism is used.

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/

class GripServo : public SerialWombatServo_18AB
{
public:
	/*!
	@brief Constructor for the GripServo Class
	
	@param sw A reference to a previously declared SerialWombatChip to which the Servo is connected.
	*/
	GripServo(SerialWombatChip & serialWombatChip):SerialWombatServo_18AB(serialWombatChip), sensor(serialWombatChip)
	{
	}
	GripACS712 sensor;


	/*!	
	@brief Initialize a GripServo on the specified pin, and its associated sensor on pin + 4
	
	This function must be called after the SerialWombatChip instance specified in the constructor
	has been initialized with a begin call.
	
	@param pin The Serial Wombat pin to set.  Valid values are 4-7 as those pins are ACS712 pins on the grip board
	@param reverse Whether the attached servo should be reversed
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/
	int16_t begin(byte pin, bool reverse = false)
	{
		attach(pin, reverse);
		return sensor.begin(pin + 4);
	}

	/*!
	@brief  The position at which the least current was pulled during calibration
	*/  
	uint16_t calibratedMinPosition = 0;
	/*!
	@brief  The highest position at which relatively low current was pulled during calibration
	*/  
	uint16_t calibratedMaxPosition = 65535;
	/*!
	@brief  The lowest observed current during calibration
	*/  
	uint16_t calibratedMinCurrent = 0;
	/*!
	@brief  The highest observed current during calibration
	*/  
	uint16_t calibratedMaxCurrent = 65535;


	/*!	
	@brief Sweep a servo across its range measuring current to determine endstops
	
	This function must be called after the begin call for the GripServo is called.  
	
	@param expectedCurrentRise A threshold to help indicate the difference in current in counts of a servo that has reached its position vs one that is stalled.  Default value works good for an SG90
	@param calibrationStartPosition  A position in 16 bit counts where the servo should be able to move freely.  it will then move in both directions from this point looking for an endstop. 
	*/
	void calibrateServoRange(uint16_t expectedCurrentRise = 400, uint16_t calibrationStartPosition = 0x8000)
	{
		// Attach again to remove any scaled output settings
		attach(_pin,//Pin  
			_min, //Minimum Pulse Time
			_max, //MaximumPulse time
			_reverse);  //Reverse//put this line in setup.
		writePublicData(calibrationStartPosition);
		delay(500);
		uint16_t position = calibrationStartPosition;
		while (sensor.readAveragedCounts() < sensor.zeroCurrentCalibration + expectedCurrentRise && position >= 2000)
		{
			position -= 2000;
			writePublicData(position);
			delay(150);
		}

		calibratedMinPosition = position + 2000;

		position = calibrationStartPosition;
		writePublicData(position);
		delay(500);
		while (sensor.readAveragedCounts() < sensor.zeroCurrentCalibration + expectedCurrentRise && position <= 63535)
		{
			position += 2000;
			writePublicData(position);
			delay(150);
		}
		calibratedMaxPosition = position - 2000;

	}

	/*!	
	@brief Close a servo based gripper with a specified force
	
	This function must be called after the begin call for the GripServo is called and after calibration

	Default values work well for an SG90 servo with a rack and pinion grabber.  Other gripper servo models and configurations may need differng
	calibration constants to achieve a predicable and quick grip.
	
	@param gripStrength The proportion of current difference between minimum and maximum observed current that the control algorithm should attempt to achieve.  A fraction of 65535.  A default value of 0 corresponds to 75%
	@param slowIncrement  The amount of change in servo counts when the servo is near the target current.  Based on the ScaledOutput ramp control algoritm.  Used to grip firmly with minimal vibration
	@param fastSlowThreshold  If the measured current is within this number of counts of the target, slowIncrement is used.  Otherwise fast increment.
	@param fastIncrement The amount of change in servo counts when the servo is far from the target current.  Used to close quickly.
	*/

	void grip(uint16_t gripStrength = 0, uint16_t slowIncrement = 100, uint16_t fastSlowThreshold = 350, uint16_t fastIncrement = 3500)
	{
		attach(_pin,//Pin  
			_min, //Minimum Pulse Time
			_max, //MaximumPulse time
			_reverse);  //Reverse//put this line in setup.
		
		if (gripStrength == 0)  // Zero default is 3/4 current.   Allows neat call with no parameters.
		{
			gripStrength = 49152; // 3/4
		}
		uint32_t gripCurrent = (calibratedMaxCurrent - calibratedMinCurrent);
		gripCurrent *= gripStrength;
		gripCurrent >>= 16;
		writeScalingTargetValue(((uint16_t) gripCurrent) + sensor.zeroCurrentCalibration);
		writeRamp(slowIncrement, //Slow Increment  How many counts the gripper moves if the current is above or below the target.
			fastSlowThreshold, //Fast/slow threshold  //  If the difference between measured and target current is more than this, move fast instead
			fastIncrement, //Fast Increment rate
			SerialWombatAbstractScaledOutput::Period::PERIOD_32mS,//How often the control loop updates.  
			SerialWombatAbstractScaledOutput::RampMode::RAMP_MODE_BOTH); // Ramp mode

		writeOutputScaling(calibratedMinPosition, calibratedMaxPosition);

		writeScalingEnabled(true, //Enabled
			sensor.pin()); //Pin providing input to control loop.  
	}

	/*!	
	@brief Disable the grip control algorithm and move the servo to the smallest non-stall position
	
	This function must be called after the begin call for the GripServo is called and after calibration

	*/
	void release()
	{
		attach(_pin,//Pin
			500, //Minimum Pulse Time
			2500, //MaximumPulse time
			false);  //Reverse//put this line in setup.

		// Set output scaling.  This will remap the meaning of the 0-65535 value from full range of the servo to full range of the gripper.
		writeOutputScaling(calibratedMinPosition, calibratedMaxPosition);

		writeScalingEnabled(true, //Enabled
			_pin); //DataSource
		writePublicData(0);
	}

	/*!
	@brief Reports if an object is in the gripper based on stop position of servo

	This function uses circumstantial evidence to determine if an object is in the gripper.  If the control algorithm stops
	the gripper closing before the close position then it is likely an object is in the gripper.  The amount of difference between
	the closed calibration and current position which represents a positive object detection is configurable.

	This function must be called after the begin call for the GripServo is called and after calibration, and after grip.

	@param divisor A divisor of the difference between open and closed calibration points.  If the stop position is more than
	this amount from closed, and object is considered detected.  Increasing this value makes it more likely that small objects will
	be detected, but also more likely that false positive detections will occur.

	Sufficent time must be given after grip is called to allow the gripper to reach steady state.  True will be returned if
	the gripper is still converging on its endpoint.

	@return Returns true if an object is detected.

	*/
	bool objectPresent(uint16_t divisor = 50)
	{
		return(readPublicData() < (calibratedMaxPosition - ((calibratedMaxPosition - calibratedMinPosition) / divisor)));
	}
	/*!	
	@brief A single calibration call that will fully calibrate a gripper
	
	This function must be called after the begin call for the GripServo is called.

	This function will iterate over the entire range of motion of the servo mapping current readings, and determinging end stops and
	gripper current ranges. 

	@warning This function will attempt to move the servo through its full range of motion.  Do not use this servo if your physical
	assembly can be damaged by attempts to move the servo through its full 180 degrees of motion

	@param reverse   Reverse the direction of the servo.

	This is a blocking function that takes a few seconds to complete.
	
	*/
	void calibrateGripper(bool reverse = false, uint16_t rangeConstant = 49152)
	{
		uint32_t position = 0;

		begin(_pin, reverse);
		calibratedMinCurrent = 65535;
		calibratedMaxCurrent = 0;
		uint16_t minCurrentPosition = 0;
		for (position = 0; position <= 65000; position += 1000)
		{
			writePublicData((uint16_t)position);
			delay(250);
			uint16_t current =sensor.readAveragedCounts();
			if (current < calibratedMinCurrent)
			{
				calibratedMinCurrent = current;
				minCurrentPosition = position;
			}		
			if (current > calibratedMaxCurrent)
			{
				calibratedMaxCurrent = current;
			}
		}

		writePublicData(minCurrentPosition);
		delay(1000);
		sensor.calibrateIdleCurrent();
		uint32_t range = (calibratedMaxCurrent - calibratedMinCurrent);
		range *= rangeConstant;
		range >>= 16;
		calibrateServoRange((uint16_t) range, minCurrentPosition);

	}

};

/*!

@brief A class representing a PCB0031 Grip PCB

The PCB0031_Grip class is a child class of SerialWombatChip.  It adds additional functionality to the begin class to 
initialize GripServo classes associated with the 4 (or 3) servo/current sensor combinations.  
In the case where the solder jumper for servo power voltage measurement is closed, 3 servo combinations are initialized, and an
analog input for the voltage is initialized.  

The class creates 4 GripServo s named gs0 thorugh gs3 .

@warning This board requires 5V logic for proper operation of the Current Sensor ICs  Servo control will work at 3.3V, but feedback may not.

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/TODO" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/TODO

*/

class PCB0031_Grip : public SerialWombatChip
{
public:
	PCB0031_Grip() : SerialWombatChip(), gs0(*this), gs1(*this), gs2(*this), gs3(*this), powerVoltage(*this)
	{}
	
	GripServo gs0,gs1,gs2,gs3;
	SerialWombatAnalogInput_18AB powerVoltage;

	GripServo* gsArray[4] = { &gs0,&gs1,&gs2,&gs3 };


	/*!	
	@brief Initialize a PCB0031_Grip pcb on a given I2C address
	
	Wire must be initialized before calling this function	
	
	@param i2cAddress The I2C address of the PCB
	@param pin3IsVoltage Set this to true if the voltage measurement solder jumper has been soldered closed
	@return  Returns a negative number indicating an error code, or a non negative number indicating success
	*/
	int16_t begin(uint8_t i2cAddress,bool pin3IsVoltage = false)
	{
		SerialWombatChip::begin(Wire, i2cAddress, true);
		for (int i = 0; i < 3; ++i)
		{
			gsArray[i]->begin(i);
		}
		if (!pin3IsVoltage)
		{
			gs3.begin(3);
		}
		else
		{
			powerVoltage.begin(3);
			pin3VoltageEnable = true;

		}
		return (0);
	}


	/*!	
	@brief Returns the voltage provided to the Servos in mV
	
	begin must be called with pin3VoltageEnable = true  before using this method

	this funciton calculates the value based on the 8200/2000 ohm voltage divider on the board


	
	@return  Returns the servo voltage in millivolts
	*/
	uint16_t readPowerVoltage_mv()
	{
		if (pin3VoltageEnable)
		{
			uint32_t v = powerVoltage.readAveraged_mV();
			v *= (8200 + 2000);  // Board has an 8200 / 2000 ohm voltage divider
			v /= 2000;
			return ((uint16_t)v);
		}
		return (0);
			
	}

private:
	bool pin3VoltageEnable = false;
};

