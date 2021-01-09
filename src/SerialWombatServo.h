	#ifndef SERIAL_WOMBAT_SERVO_H__
#define SERIAL_WOMBAT_SERVO_H__
#include "SerialWombat.h"

class SerialWombat;

class SerialWombatServo
{
public:
	SerialWombatServo(SerialWombat& sw);
	~SerialWombatServo();
	void attach(uint8_t pin);
	void attach(uint8_t pin, bool reverse);
	void attach(uint8_t pin, uint16_t min, uint16_t max);
	void attach(uint8_t pin, uint16_t min, uint16_t max, bool reverse);
	void write(uint8_t angle);
	void write16bit(uint16_t position);
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
