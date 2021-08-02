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
The SerialWombat class is the basis for all sketches.
Include SerialWombat.h to access all Serial Wombat classes.

One Serial Wombat instance should be declared for each Serial Wombat
attached to the host.  This is typically done by declaring it globally:

    SerialWombat sw;

Once the Serial Wombat instance is declared, it must be initialized using the SerialWombat.begin()
method.  There are separate begin methods for I2C (Wire) and Uart (Serial):

	sw.begin(Serial);

or

	sw.begin(Wire, 0x6C);  //I2C initialization with address 6C

Presense of a Serial wombat can be determined by testing the return value
of the SerialWombat.queryVersion() method:

	if (sw.queryVersion())
	{
		//Serial Wombat chip detected.
	}
	else
	{
		//Serial Wombat  chip not detected.
	}

Pin Modes
==============

Most Serial Wombat functionality is achieved through Pin Modes.  Most pin modes are achieved by
declaring an instance of a class for that pin mode with the Serial Wombat instance that pin
mode will be attached to as a parameter.  A .begin call with additional parameters is typically
called during the Arduino setup() function.

The following Pin Modes are avaialble on all 4 pins of the Serial Wombat 4B chip:
---------------------------------

- Button Debouncing ( SerialWombatDebouncedInput class )
- Pulse Timing ( SerialWombatPulseTimer class )
- Digital Input (SerialWombat.digitalRead() ) with optional Weak Pull Up
- Quadrature Encoder ( SerialWombatQuadEnc class)
- UART Receive ( SerialWombatUART class)

The following Pin Modes are avaialble on pins 1,2,3 of the Serial Wombat 4B and all 4 pins of the Serial Wombat 4A chips:
---------------------------

- Analog Input (SerialWombatAnalogInput class)
- Protected Output (SerialWombatProtectedOutput class)
- PWM (SerialWombatPWM class )
- Servo (SerialWombatServo class)
- UART Transmit ( SerialWombatUART class)
- Watchdog (SerialWombatWatchdog class)
- Digital Output ( SerialWombat.digitalWrite() ) with optional Open Drain mode

\warning Due to hardware initializations unexpected side effects may occur if the same pin is configured multiple times to different pin modes after Serial Wombat chip initialization

Additional Capabilities:
-----------------------

The Serial Wombat chip can measure its own source voltage with the SerialWombat.readSupplyVoltage_mV method.

The Serial Wombat chip can report its Model and Firmware revision with the SerialWombat.queryVersion method.

The Serial Wombat chip can manage power consumption with the SerialWombat.sleep and SerialWombat.wake funcitons.

The Serial Wombat chip has a unique ID available through the SerialWombat.uniqueIdentifier property.  See:
https://youtu.be/IHTcKyXT_2Q  




