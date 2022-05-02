\file SerialWombat.h
\mainpage Serial Wombat Arduino Library



Video Tutorial
==============
A video is available for Getting Started with the Serial Wombat 4B using I2C:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/UZOnq2FdrvU" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://www.youtube.com/embed/UZOnq2FdrvU

Overview
==============
This library is designed for use with the Arduio platform.  
The SerialWombatChip class is the basis for all sketches.
Include SerialWombat.h to access all Serial Wombat classes.

One SerialWombatChip instance should be declared for each Serial Wombat chip
attached to the host.  This is typically done by declaring it globally:

    SerialWombatChip sw;

Once the SerialWombatChip instance is declared, it must be initialized using the SerialWombatChip.begin()
method.  There are separate begin methods for I2C (Wire) and Uart (Serial):

	sw.begin(Serial);

or

	sw.begin(Wire, 0x6C);  //I2C initialization with address 6C

Presense of a Serial wombat can be determined by testing the return value
of the SerialWombatChip.queryVersion() method:

	if (sw.queryVersion())
	{
		//Serial Wombat chip detected.
	}
	else
	{
		//Serial Wombat  chip not detected.
	}


Serial Wombat Models
==============

There are currently two Serial Wombat chip models available: The Serial Wombat 4B and the Serial Wombat 18AB.

The Serial Wombat 4B communicates over I2C and has 4 I/O pins not counting communication pins, 1 which is digital input only, and 3 which are Input/Output
and analog capable.

The Serial Wombat 18AB can communicate over I2C or UART based on its address pin configuration.  It has 18 I/O pins not counting communication pins.  7 or 9 pins (I2C or UART mode) are Analog capable, and 15 are capable of enhanced digital capability (pins 5,6, and 8 are not).  The SW18AB has pins numbered from 0 to 19.  Two of these pins are used for communication (3 and 4 for I2C or 7 and 9 for UART).

Serial Wombat Pin Modes
==============

Most Serial Wombat functionality is achieved through Pin Modes.  Most pin modes are achieved by
declaring an instance of a class for that pin mode with the Serial Wombat instance that pin
mode will be attached to as a parameter.  A .begin call with additional parameters is typically
called during the Arduino setup() function.

SW4B pin modes (All pins)
---------------------------------

- Button Debouncing ( SerialWombatDebouncedInput class )
- Pulse Timing ( SerialWombatPulseTimer class )
- Digital Input (SerialWombatChip.digitalRead() ) with optional Weak Pull Up
- Quadrature Encoder ( SerialWombatQuadEnc class)
- UART Receive ( SerialWombatUART class)

SW4B pin modes (pins 1,2,3 only)
---------------------------

- Analog Input (SerialWombatAnalogInput class)
- Protected Output (SerialWombatProtectedOutput class)
- PWM (SerialWombatPWM class or extended SerialWombatPWM_4AB class )
- Servo (SerialWombatServo class)
- UART Transmit ( SerialWombatUART class)
- Watchdog (SerialWombatWatchdog class)
- Digital Output ( SerialWombatChip.digitalWrite() ) with optional Open Drain mode

SW18AB pin modes (All pins)
--------------------------------
- Button Debouncing ( SerialWombatDebouncedInput class )
- Pulse Timing ( SerialWombatPulseTimer or extended SerialWombatPulseTimer_18AB class )
- Digital Input (SerialWombatChip.digitalRead() ) with optional Weak Pull Up or Pull Down
- Digital Output (SerialWombatChip.digitalWrite() ) with optional Open Drain Mode
- Matrix Keypad (SerialWombatMatrixKeypad class)
- Protected Output (SerialWombatProtectedOutput class)
- PWM (SerialWombatPWM class, or extended SerialWombatPWM_18AB class)
- Pulse on Change ( SerialWombatPulseOnChange class)
- Quadrature Encoder ( SerialWombatQuadEnc class)
- Servo (SerialWombatServo class or extended SerialWombatServo_18AB class)
- Software UART TX/RX ( SerialWombatSWUART class)
- TM1637 Display Driver (SerialWombatTM1637 class)
- Ultrasonic distance sensing (SerialWombatUltrasonicDistanceSensor class)
- Watchdog (SerialWombatWatchdog class)

SW18AB Analog pin modes ( pins 0,1,2,3,4,16,17,18,19)
--------------------------------
- Analog Input (SerialWombatAnalogInput class or extended SerialWombatAnalogInput_18AB)
- Capacitive Touch (SerialWombat18CapTouch class)
- Resistance Input (SerialWombatResistanceInput class)

SW18AB Enhanced Digital pin modes (All pins except 5,6,8)
--------------------------------
- High Frequency Servo (SerialWombatHighFrequencyServo class)
- Hardware UART ( SerialWombatUART  class)
- WS2812 Driver (SerialWombatWS2812 class)
- PWM with enhanced frequency/resolution ( SerialWombatPWM_18AB class)
- Servo with enhanced resolution ( SerialWombatServo_18AB class)



\warning Due to hardware initializations unexpected side effects may occur if the same pin is configured multiple times to different pin modes after Serial Wombat chip initialization

Additional Capabilities:
-----------------------

A Serial Wombat chip can measure its own source voltage with the SerialWombatChip.readSupplyVoltage_mV method.

A Serial Wombat chip can report its Model and Firmware revision with the SerialWombatChip.queryVersion method.

A Serial Wombat chip can manage power consumption with the SerialWombatChip.sleep and SerialWombatChip.wake functions.

A Serial Wombat chip has a unique ID available through the SerialWombatChip.uniqueIdentifier property.  See:
https://youtu.be/IHTcKyXT_2Q  

The Serial Wombat 18AB chip can measure its die temperature (low accuracy) thorugh the SerialWombatChip.readTemperature_100thsDegC method.


Public Data and Pin Modes
===========================

A fundamental part of the Serial Wombat firmware design is public data.  Each pin has a 16-bit value that can be read or written by
the host or by other pins.  This piece of data is typically the most important piece of data for a pin mode, such as the position
of a Servo, the duty cycle of a PWM, the high time of a pulse measurement or the result of an analog to digital conversion.

The ability to leverage this capability on the Serial Wombat 4B is quite limited due to the limited space available for code.

However, the Serial Wombat 18AB chip can be configured in many ways to create efficient and interesting pin-to-pin interaction.

For instance, a Servo could be configured to read its position from an AnalogInput pin attached to a potentiometer.  The servo
would then automatically poll the Analog Input for its reading and set the servo position accordingly in real time with no
interaction by the host beyond the initial configuration.

Many of the Arduino examples for input functions show how the TM1637 driver can be used to display public data from another pin on
a 7 segment led array.

The SerialWombatPulseOnChange mode is specifically designed to leverage this capability.  For instance, if a pulse on change pin was
wired to a piezo beeper and configured to monitor the public data of the first pin from a 4x4 matrix keypad, it could cause an audible feedback each time
a new key was recognized.  This beeping would be done without any real-time monitoring by the Arduino host.

Public data values range from 0 to 63 for pins and from 64 to 254 for other system provided values.

The system provided values can be used for a variety of purposes.   Available data sources are included in the 

SerialWombatDataSource enum.  For instance, the value 
\link SerialWombatDataSource::SW_DATA_SOURCE_PACKETS_RECEIVED SW_DATA_SOURCE_PACKETS_RECEIVED \endlink 
increments each time the Serial Wombat firmware
processes a packet.  By configuring a Pulse On Change pin to read this value it becomes easy to create an "activity" LED which blinks
when the host sends commands.  

The value 
\link SerialWombatDataSource::SW_DATA_SOURCE_VCC_mVOLTS SW_DATA_SOURCE_VCC_mVOLTS \endlink 
provides the system voltage in mV.  By configuring a TM1637 display to display this public data it 
is easy to add a system voltage indicator to the system.

More complex interactions can be created by taking advantage of functionality provided by the SerialWombatAbstractScaledOutput and
SerialWombatAbstractProcessedInput classes.   

SerialWombatAbstractScaledOutput is inherited by many pins which provide a non-binary output, such as PWM, or Servo position.  This 
inherited class provides a common framework for scaling, inverting, transforming and filtering output.  The SerialWombatAbstractScaledOutput
class makes it easy to control how fast a Servo is allowed to move, or turn a PWM output into a PID controller which changes its output
based on an analog or pulse timing input pin's data.  This real-time control occurs on a 1kHz basis within the Serial Wombat Chip.

The SerialWombatAbstractProcessedInput class is inherited by many pins which provide non-binary input, such as Analog input, resistance input,
pulse timing, etc.  This class provides averaging, filtering, queing, linear transform or inversion, minimum and maximum tracking, outlier exclusion, etc. for input
pin modes.  An average of 64 raw samples could be provided as a pin's public data for example.


User Buffer
=======================
The Serial Wombat 18AB firmware allocates 8192 bytes (in current version, this amount may change in the future) to a User Buffer.  This is essentially an 8192 byte
array which is used by some pin modes to supplement each pin's statically allocated memory.  The WS2812 driver uses part of the User Buffer to create waveforms and animations for driving LEDs.  The software UART pin modes use the User Buffer to create transmit and receive queues.  It is up to the user to understand how much memory each pin mode uses and provide a starting index into the User Buffer which does not overlap with other areas of the User Buffer used by other pins.  The User Buffer can be read and written directly by the Arduino using the SerialWombatChip::readUserBuffer() and SerialWombatChip::writeUserBuffer() methods.

Error Messages
========================

The Serial Wombat firmware will return an error message when it recognizes an invalid command.  The Serial Wombat 4B chip has limited error checking and no specific return codes
due to limited program space.  The Serial Wombat 18AB chip has extensive error checking of commands, parameters, order of commands, etc. to help detect and debug errors.  Many functions in the Arduino library return an int16_t which returns 0 or higher for success, or a negative number corresponding to an error ID if the Serial Wombat chip returns an error.  Some Serial Wombat Arduino library function calls result in multiple command packets being sent to the Serial Wombat chip.  In some cases a function call may complete one command and error on a subsequent command.  In this case the function will return a negative error code, but unintended side effects may occur due to partial execution.  When properly configured the Serial Wombat Arduino library should not return errors.  An error handler is available which allows registration of a funciton which is called upon errors.  A sample function is provided which prints the error code to Serial if it is registered during initialization.  See the ErrorHandling examples in the Serial Wombat Arduino library examples.  Error codes are defined in the enum \link SW_ERROR_t \endlink .  If an error is detected by the Serial Wombat firmware the last packet which caused an error is stored on the Serial Wombat chip for later retreival by the Arduino through the SerialWombatChip::readLastErrorCommand() method.
To register the default handler, call the SerialWombatChip::registerErrorHandler() method with the sample function:

	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief);


The same enumerated values are used in the Arduino library and the Serial Wombat 18AB firmware, so if you're not clear on why an error is happening, examination of the firmware source code may help.  Use of the avaialble Saleae compatible Serial Wombat Protocol analyzer is also highly recommended.  A tutorial video is avaialble here: https://youtu.be/cL7kUm9qjvU .


