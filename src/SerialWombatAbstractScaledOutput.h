#pragma once
/*
Copyright 2021 Broadwell Consulting Inc.

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

#include "SerialWombat.h"
#include "limits.h"

/*! \file SerialWombatAbstractScaledOutput.h
*/


/*!
 \brief SerialWombatServo, SerialWombatPWM and other proportional ouptut classes inherit from this class.  it provides control blocks for manipulating output based on input

  This class encapsulates the Serial Wombat Scaled Output functional block.  This block is included in all proportial output classes in the SerialWombat 18AB chip.  Each pin's output block is separate from the others.
 
 This block is designed to facilitate real time control of outputs based on configuration from the host without the need to issue additional commands after the initial configuration.   The block also includes a timeout function which is capable of setting an output to pre-determined value if the host does not reset a countdown timer within a specified number of mS.  This allows a controlled shutdown if the host crashes, the data bus becomes inoperable, etc.  Because the output block is serviced every 1mS, real time control can be achived with higher performance than if control was performed over the data bus.  This functionality also frees the host of the need to maintain timing sensitive communication with the Serial Wombat chip.

 This block can limit output rate of change either by a limited amout of change per time, or by first-order filtering output changes.  Rate limiting is useful to implement smooth motion over time (controlled within the Serial Wombat chip) such as a model railroad crossing gate attached to a servo.  It also can prevent a current spike caused by requesting large changes in position at one time.  First order filtering is useful to change position rapidly at the beginning, but slow near the end to reduce impact speed.

 The target output value can be provided by the host, or the Scaled Output block can be configured to get its target value from another pin or a public data source such as the Serial Wombat 18AB chip's internal temperature sensor or source voltage measurement.  This capability to act on another pin's data would be useful for example if one wanted to control a 6 DOF / 6 servo robotic arm with 6 potentiometer outputs.  The Serial Wombat chip could filter user inputs to provide smooth movement as well as scale the outputs so that the full range of potentiometer travel can be mapped to each joint's range of servo motion in degrees.  The Arduino or other host could monitor the controls and poentially intervene if higher level logic deemed it necessary, but would be freed of the need to constantly poll the pots and update servo pulse values.

  The Scaled Output block can also do simple real-time control of an output based on an input.  For instance, a heater could be set to pwm at some duty cycle if an analog input dropped below a threshold, then shut off when it rose above some other higher threshold.  This is hystersis mode.

  Another control method is Proportial/Integral/Derivative (PID) control.  In this mode the host provides P I and D calibrations for the system, and specifies an input pin and target value.  The output of the Scaled Output block is then controlled via PID to try and reach the target value.  As an example, a motor's encoder output could be attached to a SerialWombatPulseInput pin configured to output the frequency of incoming pulses.  The PID controller in the Scaled Output block could then vary the PWM driving a FET controlling the motor to keep the motor running at constant speed that adapts to changing motor load or source voltage.  The PID controller requires that a postive output cause a positive input from the feedback system.  If they are opposite then the invert function of the block can be used.


 \image html ScaledOutputFlowchart.png
 Scaling operations happen in the following sequence:
 - Read the source pin's or data source's public data (Note that the Host can also provide the input value by setting the source pin to the output pin and writing that pin's public data.  In this case the output value of the pin will not be written to the pin's public data)
 - Scale the inputs from a specified Min/Max range to 0 to 65535
 - Invert if configured by subtracting the scaled value from 65535
 - Pass the input value to the specified control algorithm (PID, Hysteresis or PassThrough) to determine the output value
 - Check to see if a communication timeout has occurred if configured.  If so, substitute the default output value
 - Perform output filtering if configured to smooth transitions in the output value
 - Scale the output value from 0-65535 to some other range if configured.  This is useful for example if a servo is physically limited to a portion of its normal rotation.
 - Write the output data to the output pin's publc data (unless the pin is configured to use its own public data as an input source)
 - Write the output data to the underlying pin mode (Servo, PWM, etc) so that the physical output is updated
*/
class SerialWombatAbstractScaledOutput 
{
public:
	/*!
	 \brief Constructor for the SerialWombatAbstractScaledOutput Class
	 
	 \param sw A reference to a previously declared SerialWombatPin to which the output is connected.
	 */
	SerialWombatAbstractScaledOutput(SerialWombatChip& sw):_asosw(sw){}

	enum  Period {
		PERIOD_1mS = 0,
		PERIOD_2mS = 1,
		PERIOD_4mS = 2,
		PERIOD_8mS = 3,
		PERIOD_16mS = 4,
		PERIOD_32mS = 5,
		PERIOD_64mS = 6,
		PERIOD_128mS = 7,
		PERIOD_256mS = 8,
		PERIOD_512mS = 9,
		PERIOD_1024mS = 10,
	};

	/*!
	 \brief Enable a timeout value which will cause the output to go to a default value if not updated
	
	  When this function is called with a timeout value longer than 0 mS the timer becomes enabled.
	  Internally a counter is set to 0 and increments each 1ms.  If the counter reaches the timeout
	  value then the output is set to timeoutOutputValue.  Calling writeTimeout again resets the counter
	 to 0, so it is intended to be called periodicallly to ensure that the timeout never occurs under 
	 normal circumstances.  It can also be used to generate timed output which is active for some
	 period of time then reverts to timeoutOutputValue .  The length of this pulse may vary by
	 a few hundred microseconds because it is done in software, not hardware.
	
	 \param timeout_mS  How long in mS before the timeout occurs.  Set to zero to disable the timeout
	 \param timeoutOutputValue The 16 bit value that should be output if a timeout occurs
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeTimeout(uint16_t timeout_mS, uint16_t timeoutOutputValue)
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

	/*!
	 \brief Enable scaling and set which pin or public data is used as the input source
	
	 This function should be called after all other desired configuration functions have been called.
	
	 \param enabled Enables the Scaled Output block.  If false then the current pin's public data value is used as the setting for the output
	 \param sourcePin  The pin or public data id that should be read to drive the output value.  Only used when enabled is set to true
	*/
	int16_t writeScalingEnabled(bool enabled, uint8_t sourcePin)
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

	/*!
	 \brief Scale incoming values to a range of 0 to 65535
	
	 This function allows configuration of an input scaling range that maps to 0-65535.  
	 For example, if another pin or public data returns a range from 2000 to 5000, setting the minimum to 2000 and
	 maximum to 5000 will cause values below 2000 to be 0, values above 5000 to be 65535, and values
	 in between will be scaled accordingly.  This allows a sensor or other input device to scale to the
	 Serial Wombat philosophy of using a 16 bit resolution number to represent the the possible range of values
	
	 \param min The minimum value of the input range.  Input values less than or equal to that will be scaled to 0
	 \param max The maximum value of the input range.  Input values greater or equal to that will be scaled to 65535
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeInputScaling(uint16_t inputMin, uint16_t inputMax)
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

	/*!
	 \brief Reduces the output range from 0 to 65535 to user specified range
	
	 This functionn is useful for scaling the 0 to 65535 full range to a specific
	 output range to correspond to the desired output range of the actual physical
	 output.
	
	 For instance, if a 180 degree servo needed to move through its center 90 degrees
	 (45 degrees dead band, 90 degrees of movement, and 45 degrees of dead band) then
	 minimum and maximum values of 16384 and 49152 could be used. 
	 
	 \param outputMin The value to which a calculated output of 0 will map
	 \param outputMax The value to which a calculated output of 65535 will map
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeOutputScaling(uint16_t outputMin, uint16_t outputMax)
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

	/*!
	 \brief if enabled subtract the input value from 65535 before doing any other processing.
	
	 \param inverted False - input value isn't changed.  True- input value is subtracted from 65535
	
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeScalingInvertedInput(bool inverted)
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

	/*!
	 \brief The target input value for PID control
	
	 When configured to drive the output value via PID control the target value is
	 the desired value for the input to reach based on modification of the output value.
	 For example, in a system where an analog temperatue sensor was influnced by a heater
	 attached to a PWM output, the input would be the actual temperature, the target
	 would be the desired temperature, and the output would be the PWM driving the heater.
	 
	 \param target The 16 bit value the system will attempt to achieve by varying the output 
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeScalingTargetValue(uint16_t target)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			6,  // Set target value for PID controller
			SW_LE16(target),0x55,0x55,
		};
		return(_asosw.sendPacket(tx));
	}

	/*!
	 \begin Controls how fast an ouput can change in counts
	
	 This function allows configuration of how fast an output's value is allowed
	 to change in counts on a 0-65535 scale.  The Rate control runs on a fixed
	 period based on the Period enumerated type.  Every samplePeriod ms the target
	 is reevaluated.  If the expected output would change the output by more
	 than maximumChangecounts, then the output only changes by maximumChangecounts 
	
	 \param samplePeriod How often the output is updated (enumerated type)
	 \param maximumChangecounts The maximum number of counts of change allowed over samplePeriod
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeRateControl(Period samplePeriod, uint16_t maximumChangecounts)
	{
		{
			uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
				pin(),
				swPinModeNumber(),
				4, // Set filter mode
				1, // Filter mode rate control
				SW_LE16(maximumChangecounts),0x55,
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
			(uint8_t)samplePeriod,
			0x55,0x55,0x55,
		};
		return(_asosw.sendPacket(tx));
		}
	}

	/*!
	 \begin Controls how fast an ouput can change based on filtering 
	
	 This function allows configuration of how fast an output's value is allowed
	 to change based on a first order filter.  The Rate control runs on a fixed
	 period based on the Period enumerated type.  Every samplePeriod ms the target
	 is reevaluated.  The requested output is combined with the current output 
	 using filterConstant.  Higher values for filterConstant cause heavier filtering
	
	 \param samplePeriod How often the output is updated (enumerated type)
	 \param filterConstant A value from 0 to 65535 indicating the amount of filtering.  Higher is more filtering.
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t write1stOrderFiltering(Period sampleRate, uint16_t filterConstant)
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
			(uint8_t)sampleRate,
			0x55,0x55,0x55,
			};
			return(_asosw.sendPacket(tx));
		}
	}

	/*!
	 \brief Controls the output based on hystersis control
	
	 Calling this function places the output in hysteresis mode.  In this mode the input source is monitored and
	 compared against a high and low threshold. If the input source is equal or greater to highLimit then the 
	 output becomes highOutputValue.  If it is equal or less than lowLimit then lowOutputValue is output.  
	  If the input source is between lowLimit and highLimit then no change occurs.  
	  \param lowLimit If the input source is at or below this level the output value is lowOutputValue
	 \param lowOutputValue if the input source is at or below lowLimit then this value becomes the output
	 \param highLimit if the input source is at or above this level then the output value is highOutputValue
	 \param highOutputValue if the input source is at or above highLimit then this value becomes the output
	 \param initialOutputValue  if the input source is between lowLimit and highLimit at initialization then this value is output
	 \return returns 0 or higher if success, or a negative error code
	*/
	int16_t writeHysteresis(uint16_t lowLimit, uint16_t lowOutputValue, uint16_t highLimit, uint16_t highOutputValue, uint16_t initialOutputValue)
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

	/*!
	 \return returns 0 or higher if success, or a negative error code
	
	 \brief Configure the scaled output block into PID control mode
	
	 This pin mode implements Propotional / Integral / Derivative (PID) control.  It attempts to
	 drive the input to a target value based on calibration constants provided by the host.
	
	 For a discussion of PID control in general, this link is suggested:
	 https://www.ni.com/en-us/innovations/white-papers/06/pid-theory-explained.html  
	
	 This block implements PID using the diagram shown in this link where the kP term is
	 applied after the sum of the error, 1/ki * integration and kd * derivative sum.  This
	 is different from the order used in some PID algorithms.  The net effect is the same but
	 the values of the ki and kd are different than if the constants were all applied first and
	 the sum taken afterwards.  This was done to best distribute values so that calculations could
	 be done using integral math rather than floating point due to the performance cost of floating
	 point on small microcontrollers.
	
	  Note that kp, ki and kd are unsigned integers.  For this reason it is necessary that 
	 an increase in controller output creates a positive change in the measurement of the plant.
	  The writeScalingInvertedInput function can be used to invert the input if the system reacts
	 negatively to a an increase in output.
	 
	  See the pid() function in the Serial Wombat 18AB firmware to see exactly how the math is implemented:
	 https://github.com/BroadwellConsultingInc/SerialWombat/blob/main/SerialWombat18A_18B/SerialWombat18A_18B.X/outputScale.c
	
	 \param kp  The proportional constant applied to the sum of the error, 1/ki * error sum, and kd * derivative.  Scaled to 1/256ths .
	 \param ki  The integral constant applied to the integral.  Scaled to 1/16384ths, as typically you want this value to be less than 1.0
	 \param kd  The derivative contant applied to the derivative.  This value is scaled to 1/16384.  
	 \param target  The value the output will attempt to control the input to
	 \param samplePeriod an enumerated time for how often the PID controller updates.  This value should be based on how fast the system responds to change in output so that integral and derivative terms work correctly.
	 \return returns 0 or higher if success, or a negative error code
	 */
	int16_t writePID(uint16_t kp, uint16_t ki, uint16_t kd,uint16_t target,Period samplePeriod, uint8_t targetPin = 255, bool biDirectional = false)
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
			(uint8_t)samplePeriod,
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
		{
			uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
				pin(),
				swPinModeNumber(),
				109, // Configure target and bidirectional
				targetPin,
				biDirectional,
				0x55,0x55
			};
			int16_t result = _asosw.sendPacket(tx); if (result < 0) { return(result); }
		}
		return 0;

	}


	/*!
	 \brief Request Last Output Value
	 
	 This function is useful to see intermediate output values when the scaled output is using its
	 same pin's public data for input (so the output value is not written back to public data)
	 \return Last 16 bit value output to hardware
	*/
	uint16_t readLastOutputValue()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
			9, // Read Last Value
			0x55, 0x55,0x55,0x55,
		};
		uint8_t rx[8];
		if (_asosw.sendPacket(tx, rx) >= 0)
		{
			return(rx[4] + (uint16_t)rx[5] * 256);
		}
		else
		{
			return (0);
		}

	}

	int16_t writeScalingTargetValueResetIntegrator(uint16_t target)
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		110, // Write target Value and Reset Integrator
		(byte)(target & 0xFF),(byte)(target >> 8),0x55,0x55
		};
		return _asosw.sendPacket(tx);
	}
	int32_t PIDGetLastError()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		103, // Get Last Error
		0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];

		_asosw.sendPacket(tx, rx);
		int32_t returnVal;
		memcpy(&returnVal,&rx[4],4);
		return returnVal;

	}


	int32_t PIDGetLastIntegrator()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		104, // Get Last Integrator
		0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];

		_asosw.sendPacket(tx, rx);

		int32_t returnVal;
		memcpy(&returnVal,&rx[4],4);
		return returnVal;
	}

	int32_t PIDGetLastIntegratorEffort()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		105, // Get Last Integrator Effort
		0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];

		_asosw.sendPacket(tx, rx);

		int32_t returnVal;
		memcpy(&returnVal,&rx[4],4);
		return returnVal;
	}

	int32_t PIDGetLastProportionalEffort()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		106, // Get Last Proportional Effort
		0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];

		_asosw.sendPacket(tx, rx);

		int32_t returnVal;
		memcpy(&returnVal,&rx[4],4);
		return returnVal;
	}

	int32_t PIDGetLastDerivativeEffort()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		107, // Get Last Derivative Effort
		0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];

		_asosw.sendPacket(tx, rx);

		int32_t returnVal;
		memcpy(&returnVal,&rx[4],4);
		return returnVal;
	}

	int32_t PIDGetLastEffort()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		108, // Get Last Total Effort
		0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];

		_asosw.sendPacket(tx, rx);

		int32_t returnVal;
		memcpy(&returnVal,&rx[4],4);
		return returnVal;
	}

	uint16_t ReadLastTarget()
	{
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		111, // Get Last Target
		0x55,0x55,0x55,0x55
		};
		uint8_t rx[8];

		_asosw.sendPacket(tx, rx);

		return ((uint16_t)(rx[4] + 256 * rx[5]));
	}

	/*!
	 \brief Set Up 2D Lookup Output Scaling

	 Configure 2d Lookup scaling based on an array of 16 bit unsigned numbers in user memory
	 at index IndexInUserMemory .  The first entry must be 0x0000 0xYYYY where YYYY is the output
	 when 0x0000 is the input.  The final entry must be 0xFFFF 0xZZZZ where ZZZZ is the output
	 when 0xFFFF is the input.  Any number of additional point pairs can come between 0x0000 and 0xFFFF.
	 Point pairs must be in assending order by input.

	 \return Returns a negative error code if configuration generated an error.
	*/
	int16_t Enable2DLookupOutputScaling(uint16_t IndexInUserMemory ///< Index in user memory where 2d lookup table is located
	)
	{
		uint8_t tx[] = {(uint8_t)SerialWombatCommands::CONFIGURE_PIN_OUTPUTSCALE,
			pin(),
			swPinModeNumber(),
		10, // Set 2D Lookup Index 
		SW_LE16(IndexInUserMemory),0x55,0x55 };
		return _asosw.sendPacket(tx);
	}

	/// \brief Facilitates inheritance
	virtual uint8_t pin() = 0;
	/// \brief Facilitates inheritance
	virtual uint8_t swPinModeNumber() = 0;

private:
	SerialWombatChip& _asosw;

};


