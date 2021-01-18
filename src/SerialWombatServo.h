	#ifndef SERIAL_WOMBAT_SERVO_H__
#define SERIAL_WOMBAT_SERVO_H__
#include "SerialWombat.h"

class SerialWombat;

/*! \file SerialWombatServo.h
*/

/*!

\brief A class to control Servos attached to the Serial Wombat.

The SerialWombatServo class is used to control hobby servos attached to the Serial Wombat.

Any output-capable pin may be attached to an instance of the SerialWombatServo class.
A separate instance should be declared for each Servo.

The SW4B_Ard_Servo example included with the Arduino library shows how to use this class.

This class uses "attach" rather than "begin" to initialize servos to be consistent with the
Arduino Servo native API.

A Tutorial video is also avaialble:

\htmlonly
<iframe width="560" height="315" src="https://www.youtube.com/embed/WiciAtS1ng0" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
\endhtmlonly

https://youtu.be/WiciAtS1ng0

*/
class SerialWombatServo
{
public:
	/// \brief Constructor for the SerialWombatServo Class
	/// 
	/// \param sw A reference to a previously declared SerialWombat to which the Servo is connected.
	SerialWombatServo(SerialWombat& sw);
	~SerialWombatServo();

	/// \brief Initialize a servo on the specified pin
	/// 
	/// This function must be called after the SerialWombat instance specified in the constructor
	/// has been initialized with a begin call.
	/// 
	/// This function initializes a servo with a minimum pulse of 544 uS and a maximum pulse of 2400uS
	/// to be consistent with the native Arduino API attach function.
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	void attach(uint8_t pin);
	
	/// \brief Initialize a servo on the specified pin
	/// 
	/// This function must be called after the SerialWombat instance specified in the constructor
	/// has been initialized with a begin call.
	/// 
	/// This function initializes a servo with a minimum pulse of 544 uS and a maximum pulse of 2400uS
	/// to be consistent with the native Arduino API attach function.
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param reverse  If true, the pulse widths will be reversed, with the longest pulse width corresponding to 0 position.
	void attach(uint8_t pin, bool reverse);
	/// \brief Initialize a servo on the specified pin
	/// 
	/// This function must be called after the SerialWombat instance specified in the constructor
	/// has been initialized with a begin call.
	/// 
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param min Minimum pulse width in uS.  Minimum functional value is 100.
	/// \param max Maximum pulse width in uS
	void attach(uint8_t pin, uint16_t min, uint16_t max);
	/// \brief Initialize a servo on the specified pin
	/// 
	/// This function must be called after the SerialWombat instance specified in the constructor
	/// has been initialized with a begin call.
	/// 
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param min Minimum pulse width in uS.  Minimum functional value is 100.
	/// \param max Maximum pulse width in uS
 	/// \param reverse  If true, the pulse widths will be reversed, with the longest pulse width corresponding to 0 position.
	void attach(uint8_t pin, uint16_t min, uint16_t max, bool reverse);

	/// \brief Writes a value to the servo
	/// 
	/// This funciton allows setting the "angle" of a servo, although actual travel may vary.
	/// Consider using the Serial Wombat function write16bit for higher resolution
	/// 
	/// \param angle  A value from 0 to 180
	void write(uint8_t angle);

	/// \brief Writes a 16 bit value to the servo
	/// 
	/// This funciton scales the pulse width of the signal being sent to the  servo.
	/// 
	/// \param position Servo position.  0 sends the minimum pulse width.  65535 sends the maximum pulse width (unless Reverse is enabled)
	void write16bit(uint16_t position);

	/// \brief returns the last position of the servo scaled to a number from 0 to 180.
	/// 
	/// \return A position value from 0 to 180.
	uint8_t read(void);

private:
	SerialWombat& _sw;
	uint8_t _pin = 255;
	uint16_t _position = 0;
	uint16_t _min = 544;  // Default for Arduino Servo library
	uint16_t _max = 2400; // Default for Arduino Servo Library
	bool _reverse = false;
	void initializeServo();
};
#endif
