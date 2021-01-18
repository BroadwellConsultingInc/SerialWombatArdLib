#ifndef SERIAL_WOMBAT_H__
#define SERIAL_WOMBAT_H__
#include <stdint.h>
#include "HardwareSerial.h" // Using "" rather than <> for compatibility with Visual C++ simulation project
#include "Wire.h"// Using "" rather than <> for compatibility with Visual C++ simulation project
#include "Arduino.h"

/*! \file SerialWombat.h
*/

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

/*! /brief Class for a Serial Wombat.  Each Serial Wombat chip on a project should have its own instance.

This class describes the capabilties of a Serial Wombat Chip that are not Pin Mode functionalities

*/
class SerialWombat
{
public:
	SerialWombat();
	/// \brief initialize a Serial Wombat to use a Serial Interface.
	/// 
	/// This call causes the Serial Wombat to be reset through its reset
	/// command as the first operation.  The Serial Wombat's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param serial Serial Interface to be used by the Serial Wombat library	
	void begin(HardwareSerial &serial);
	/// \brief initialize a Serial Wombat to use a Serial Interface.
	/// 
	/// The reset parameter determines if the Serial Wombat is reset
	/// prior to other initialization operations.  If false,
	/// then any prior pin modes and configurations may still be in
	/// place.
	/// The Serial Wombat's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param serial Serial Interface to be used by the Serial Wombat library
	/// \param reset Whether or not to reset the Serial Wombat via command as the first initialization operation
	void begin(HardwareSerial& serial, bool reset);

	/// \brief initialize a Serial Wombat to use a specified I2C Interface and address.
	/// 
	/// This call causes the Serial Wombat to be reset through its reset
	/// command as the first operation.  The Serial Wombat's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param wire I2C interface to be used by the Serial Wombat Library
	/// \param i2cAddress I2C Follower address of the Serial Wombat commanded by this instance
	void begin(TwoWire &wire, uint8_t i2cAddress);
	
	/// \brief initialize a Serial Wombat to use the default Wire I2C Interface and specified address.
	/// 
	/// This call causes the Serial Wombat to be reset through its reset
	/// command as the first operation.  The Serial Wombat's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param i2cAddress I2C Follower address of the Serial Wombat commanded by this instance
	void begin(uint8_t i2cAddress);

	/// \brief initialize a Serial Wombat to use a specified I2C Interface and address.
	/// 
	/// This call causes the Serial Wombat to optionally reset through its reset
	/// command as the first operation.  The Serial Wombat's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param wire I2C interface to be used by the Serial Wombat Library
	/// \param i2cAddress I2C Follower address of the Serial Wombat commanded by this instance
	/// \param reset Whether or not to reset the Serial Wombat via command as the first initialization operation. If false,
	/// then any prior pin modes and configurations may still be in
	/// place.
	void begin(TwoWire& wire, uint8_t i2cAddress, bool reset);
	~SerialWombat();

	/// \brief Send an 8 byte packet to the Serial Wombat and wait for 8 bytes back
	/// 
	/// This method sends 8 bytes via I2C or Serial and blocks until 8 bytes are receieved
	/// back
	/// 
	/// \param tx address of an array of 8 bytes to send
	/// \param rx address of an array of 8 bytes into which to put response.
	/// \return The number of bytes received as a response, or a negative value if an error was returned
	/// from the Serial Wombat
	int sendPacket( uint8_t tx[], uint8_t rx[]);

	/// \brief Send an 8 byte packet to the Serial Wombat.
	/// 
	/// This method sends 8 bytes via I2C and does not wait for a response.
	/// When sending to UART, the Library waits for an 8 byte response.
	/// 
	/// \param tx address of an array of 8 bytes to send
	/// \return The number of bytes sent
	int sendPacket(uint8_t tx[]);

	/// \brief Request version string (combined model and firmware) and return pointer to it
	/// 
	/// This queries the Serial Wombat for the 7 characters:   product line (1 character)
	/// Model (3 characters) and firmware version (3 characters)
	/// This is stored in a string in the Serial Wombat object.  A pointer to this string
	/// is returned.
	char* readVersion(void);

	/// \brief Read the 16 Bit public data associated with a Serial Wombat Pin Mode 
	/// 
	/// Reads and returns the 16 bit value associated with a Serial Wombat Pin Mode.
	/// Additionally, values of 65 and higher have special meanings.  See
	/// Serial Wombat firmware documentation for details.
	/// \return 16 bit public data for pin specified
	/// \param pin The pin (or special meaning value) for which to retreive data
	uint16_t readPublicData(uint8_t pin);

	/// \brief Write a 16 bit value to a Serial Wombat pin Mode
	/// \param pin The pin number to which to write
	/// \param value The 16 bit value to write
	uint16_t writePublicData(uint8_t pin, uint16_t value);

	/// \brief Measure the Serial Wombat's Supply voltage
	/// 
	/// Causes the Serial Wombat to measure the counts for the 
	/// internal 1.024V reference voltage.  The Arduino library
	/// then converts these counts to a Source votlage in mV
	/// 
	/// \return The Serial Wombat's source voltage in mV
	uint16_t readSupplyVoltage_mV(void);

	/// \brief Send a reset command to the Serial Wombat
	/// 
	/// Sends a reset command to the Serial Wombat.  The calling function
	/// should wait 500mS before sending additional commands.
	void hardwareReset();

	/// \brief Set a pin to INPUT or OUTPUT
	/// 
	/// This method matches the Arduino Digital io pinMode command
	/// It should only be used on pins that have not been configured to a more
	/// sophisticated (e.g. debounce or servo) pin mode.  
	/// 
	/// \param pin The Serial Wombat pin to set
	/// \param mode Valid values are INPUT or OUTPUT as defined by arduino.  Do 
	/// not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	void pinMode(uint8_t pin, uint8_t mode);

	/// \brief Set a pin to INPUT or OUTPUT, with options for pull Ups and open Drain settings
	/// 
	/// \param pin The Serial Wombat pin to set
	/// \param mode Valid values are INPUT, OUTPUT or INPUT_PULLUP as defined by arduino.  Do 
	/// not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	/// \param pullDown  If True, a weak pull down will be enabled on this pin (No effect on SW4A/SW4B)
	/// \param openDrain If True, output becomes openDrain output rather than push / pull
	void pinMode(uint8_t pin, uint8_t mode, bool pullDown, bool openDrain);

	/// \brief Set an output pin High or Low
	/// 
	/// Before calling this function, the pin should be configured as an input or output with pinMode()
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param val  Valid values are HIGH or LOW
	/// not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	void digitalWrite(uint8_t pin, uint8_t val);

	/// \brief Reads the state of a Pin
	/// 
	/// \return Returns LOW if pin is low or public data is 0.  Returns HIGH if pin is high or public data is > 0
	int digitalRead(uint8_t pin);

	/// \brief Configures pin as analog input and does an immediate A/D conversion.  
	/// 
	/// This function is compatible with the Arduino Uno analogRead function.  
	/// It does not make use of advanced Serial Wombat functionality such as averaging and
	/// filtering.  Consider declaring a SerialWombatAnalogInput instead.
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \return An Analog to Digital conversion ranging from 0 to 1023 (10-bit)
	int analogRead(uint8_t pin);

	/// \brief Set a pin to PWM output
	/// 
	/// This function is compatible with the Arduino Uno analogWrite function, but will
	/// output a PWM with a different frequency.
	/// Consider declaring a SerialWombatPWM instead.  It has higher resolution and
	/// the ability to choose frequency.
	/// 
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param val A value from 0 (always low) to 255(always high) for the PWM duty cycle
	void analogWrite(uint8_t pin, int val);


	/// \brief Send a version request to the Serial Wombat
	/// 
	/// This function queries the Serial Wombat for its model and version
	/// and stores the result in the public members model and fwVersion
	/// as zero terminated strings.  Returns true if the response is
	/// likely a proper version response and false otherwise.
	/// 
	/// \return TRUE if response was likely a valid version, FALSE otherwise
	bool queryVersion();

	/// \brief Jump to Bootloader and wait for a UART download of new firmware
	/// 
	/// This function causes a reset of the Serial Wombat and causes it to remain
	/// in the bootloader until a power-cycle occurs.  This allows loading new
	/// firmware via a UART connection to the bottom two pins (DIP pins 4 (RX) and 5(TX))
	/// on the SW4A/SW4B.  When jumping to boot the TX pin will go high.  All other 
	/// communication or functional pins will become inputs (i.e. PWMS, etc will stop).
	void jumpToBoot();

	/// Stores the last value retreived by readSupplyVoltage_mV().  Used by SerialWombatAnalogInput 
	/// class to calculate mV outputs from retreived A/D counts.
	/// Don't access this member, as it may become private and SerialWombatAnalog input be made
	/// a friend of SerialWombat in the future.  Call readSupplyVoltage_mV instead.
	uint16_t _supplyVoltagemV = 0;

	/// Contains the last model retreived by queryVersion() as a zero-terminated string
	uint8_t model[4] = { 0 };

	/// Contains the last firmware Version retreived by queryVersion() as a zero-terminated string
	uint8_t fwVersion[4] = { 0 };
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
	void initialize();
	
};

/// Convert a uint16_t to two bytes in little endian format for array initialization
#define SW_LE16(_a)  (uint8_t)(_a & 0xFF), (uint8_t)(_a >>8)  

#include "SerialWombatServo.h"
#include "SerialWombatPWM.h"
#include "SerialWombatAnalogInput.h"
#include "SerialWombatDebouncedInput.h"
#include "SerialWombatQuadEnc.h"
#include "SerialWombatProtectedOutput.h"
#include "SerialWombatPulseTimer.h"
#include "SerialWombatUART.h"
#include "SerialWombatWatchdog.h"

#endif
