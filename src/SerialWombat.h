#ifndef SERIAL_WOMBAT_H__
#define SERIAL_WOMBAT_H__
#include <stdint.h>
#include "HardwareSerial.h" // Using "" rather than <> for compatibility with Visual C++ simulation project
#include "Wire.h"// Using "" rather than <> for compatibility with Visual C++ simulation project
#include "Arduino.h"


typedef enum
{
	SW_LOW = 0,
	SW_HIGH = 1,
	SW_INPUT = 2,
}SerialWombatPinState_t;



#define WOMBAT_MAXIMUM_PINS 19
typedef enum
{
	CMD_ECHO ='!',
	CMD_READ_BUFFER_ASCII = 'G',
	CMD_ASCII_SET_PIN ='P',
	CMD_RESET = 'R',
	CMD_SET_BUFFER_ASCII = 'S',
	CMD_RESYNC = 'U',
	CMD_VERSION = 'V',
	CMD_SUPPLYVOLTAGE = 'V',
	CMD_READBUFFERBINARY = 129,
	CMD_SETBUFFERBINARY = 130,
	CMD_SET_PIN_MODE0 = 200,
	CMD_SET_PIN_MODE1 = 201,
	CMD_SET_PIN_MODE2 = 202,
	CMD_SET_PIN_MODE3 = 203,
	CMD_SET_PIN_MODE4 = 204,
	CMD_SET_PIN_MODE5 = 205,
	CMD_SET_PIN_MODE6 = 206,
	CMD_SET_PIN_MODE7 = 207,
	CMD_SET_PIN_MODE8 = 208,
	CMD_SET_PIN_MODE9 = 209,

}SerialWombatCommand_t;


typedef enum {
	PIN_MODE_DIGITALIO = 0,
	PIN_MODE_CONTROLLED = 1,
	PIN_MODE_ANALOGINPUT = 2,
	PIN_MODE_SERVO = 3,
	PIN_MODE_QUADRATUREENCODER = 5,
	PIN_MODE_WATCHDOG = 7,
	PIN_MODE_PROTECTED_OUTPUT = 8,
	PIN_MODE_PWM = 16,
	PIN_MODE_UART_RX_TX = 17,
	PIN_MODE_PULSETIMER = 18,
	PIN_MODE_UNKNOWN = 255
}SerialWombatPinMode_t;
class WombatPacket
{
public:
	 uint8_t rx[8];
	 uint8_t tx[8];
	 bool success;
	 uint16_t errorCode;

	 void setTxUint16(uint16_t data, uint8_t index);
	 void setTxUint32(uint32_t data, uint8_t index);
	 uint16_t getRxUint16(uint8_t index);
	 uint32_t getRxUint32(uint8_t index);
	 SerialWombatCommand_t getCommand();
	 void setCommand(SerialWombatCommand_t command);
	 void setCommand(uint8_t commandByte);
	 

};


class SerialWombat
{
public:
	SerialWombat();
	void begin(HardwareSerial &serial);
	void begin(HardwareSerial& serial, bool reset);
	void begin(TwoWire &wire, uint8_t i2cAddress);
	void begin(uint8_t i2cAddress);
	void begin(TwoWire& wire, uint8_t i2cAddress, bool reset);
	~SerialWombat();
	int sendPacket( uint8_t tx[], uint8_t rx[]);
	int sendPacket(uint8_t tx[]);
	char* readVersion(void);
	uint16_t readPublicData(uint8_t pin);
	void writePublicData(uint8_t pin, uint16_t value);
	uint16_t readSupplyVoltage_mV(void);
	void hardwareReset();
	void pinMode(uint8_t pin, uint8_t mode);
	void pinMode(uint8_t pin, uint8_t mode, bool pullDown, bool openDrain);
	void digitalWrite(uint8_t pin, uint8_t val);
	void digitalWrite(uint8_t pin, uint8_t val, bool openDrain);
	int digitalRead(uint8_t pin);
	int analogRead(uint8_t pin);
	void analogWrite(uint8_t pin, int val);
private:

	uint8_t address = 0;
	char version[8] = { 0 };
	HardwareSerial * Serial = NULL;
	TwoWire* i2cInterface = NULL;
	uint8_t _pinmode[WOMBAT_MAXIMUM_PINS]={}; // Includes Pullup
	bool _pullDown[WOMBAT_MAXIMUM_PINS]={};
	bool _openDrain[WOMBAT_MAXIMUM_PINS]={};
	void configureDigitalPin(uint8_t pin, uint8_t highLow);
	unsigned long sendReadyTime = 0;
};

#define SW_LE16(_a)  (uint8_t)(_a & 0xFF), (uint8_t)(_a >>8)  // Convert a uint16_t to two bytes in little endian format for array initialization

#include "SerialWombatServo.h"
#include "SerialWombatPWM.h"
#include "SerialWombatAnalogInput.h"
#include "SerialWombatQuadEnc.h"
#include "SerialWombatProtectedOutput.h"
#include "SerialWombatPulseTimer.h"
#include "SerialWombatUART.h"
#include "SerialWombatWatchdog.h"

#endif
