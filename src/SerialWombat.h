#ifndef SERIAL_WOMBAT_H__
#define SERIAL_WOMBAT_H__

/*
Copyright 2020-2021 Broadwell Consulting Inc.

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
#include "Stream.h"
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

enum class SerialWombatDataSource{

	SW_DATA_SOURCE_PIN_0 = 0, ///< 16 bit public data provided by Pin 0
	SW_DATA_SOURCE_PIN_1 = 1,///< 16 bit public data provided by Pin 1
	SW_DATA_SOURCE_PIN_2 = 2,///< 16 bit public data provided by Pin 2
	SW_DATA_SOURCE_PIN_3 = 3,///< 16 bit public data provided by Pin 3
	SW_DATA_SOURCE_PIN_4 = 4,///< 16 bit public data provided by Pin 4
	SW_DATA_SOURCE_PIN_5 = 5,///< 16 bit public data provided by Pin 5
	SW_DATA_SOURCE_PIN_6 = 6,///< 16 bit public data provided by Pin 6
	SW_DATA_SOURCE_PIN_7 = 7,///< 16 bit public data provided by Pin 7
	SW_DATA_SOURCE_PIN_8 = 8,///< 16 bit public data provided by Pin 8
	SW_DATA_SOURCE_PIN_9 = 9,///< 16 bit public data provided by Pin 9
	SW_DATA_SOURCE_PIN_10 = 10,///< 16 bit public data provided by Pin 10
	SW_DATA_SOURCE_PIN_11 = 11,///< 16 bit public data provided by Pin 11
	SW_DATA_SOURCE_PIN_12 = 12,///< 16 bit public data provided by Pin 12
	SW_DATA_SOURCE_PIN_13 = 13,///< 16 bit public data provided by Pin 13
	SW_DATA_SOURCE_PIN_14 = 14,///< 16 bit public data provided by Pin 14
	SW_DATA_SOURCE_PIN_15 = 15,///< 16 bit public data provided by Pin 15
	SW_DATA_SOURCE_PIN_16 = 16,///< 16 bit public data provided by Pin 16
	SW_DATA_SOURCE_PIN_17 = 17,///< 16 bit public data provided by Pin 17
	SW_DATA_SOURCE_PIN_18 = 18,///< 16 bit public data provided by Pin 18
	SW_DATA_SOURCE_PIN_19 = 19,///< 16 bit public data provided by Pin 19
//	SW_DATA_SOURCE_PIN_20 = 20,
//	SW_DATA_SOURCE_PIN_21 = 21,
//	SW_DATA_SOURCE_PIN_22 = 22,
//	SW_DATA_SOURCE_PIN_23 = 23,
//	SW_DATA_SOURCE_PIN_24 = 24,
//	SW_DATA_SOURCE_PIN_25 = 25,
//	SW_DATA_SOURCE_PIN_26 = 26,
//	SW_DATA_SOURCE_PIN_27 = 27,
//	SW_DATA_SOURCE_PIN_28 = 28,
//	SW_DATA_SOURCE_PIN_29 = 29,
//	SW_DATA_SOURCE_PIN_30 = 30,
//	SW_DATA_SOURCE_PIN_31 = 31,
//	SW_DATA_SOURCE_PIN_32 = 32,
//	SW_DATA_SOURCE_PIN_33 = 33,
//	SW_DATA_SOURCE_PIN_34 = 34,
//	SW_DATA_SOURCE_PIN_35 = 35,
//	SW_DATA_SOURCE_PIN_36 = 36,
//	SW_DATA_SOURCE_PIN_37 = 37,
//	SW_DATA_SOURCE_PIN_38 = 38,
//	SW_DATA_SOURCE_PIN_39 = 39,
//	SW_DATA_SOURCE_PIN_40 = 40,
//	SW_DATA_SOURCE_PIN_41 = 41,
//	SW_DATA_SOURCE_PIN_42 = 42,
//	SW_DATA_SOURCE_PIN_43 = 43,
//	SW_DATA_SOURCE_PIN_44 = 44,
//	SW_DATA_SOURCE_PIN_45 = 45,
//	SW_DATA_SOURCE_PIN_46 = 46,
//	SW_DATA_SOURCE_PIN_47 = 47,
//	SW_DATA_SOURCE_PIN_48 = 48,
//	SW_DATA_SOURCE_PIN_49 = 49,
//	SW_DATA_SOURCE_PIN_50 = 50,
//	SW_DATA_SOURCE_PIN_51 = 51,
//	SW_DATA_SOURCE_PIN_52 = 52,
//	SW_DATA_SOURCE_PIN_53 = 53,
//	SW_DATA_SOURCE_PIN_54 = 54,
//	SW_DATA_SOURCE_PIN_55 = 55,
//	SW_DATA_SOURCE_PIN_56 = 56,
//	SW_DATA_SOURCE_PIN_57 = 57,
//	SW_DATA_SOURCE_PIN_58 = 58,
//	SW_DATA_SOURCE_PIN_59 = 59,
//	SW_DATA_SOURCE_PIN_60 = 60,
//	SW_DATA_SOURCE_PIN_61 = 61,
//	SW_DATA_SOURCE_PIN_62 = 62,
//	SW_DATA_SOURCE_PIN_63 = 63,
	SW_DATA_SOURCE_INCREMENTING_NUMBER = 65, ///< An number that increments each time it is accessed.
	SW_DATA_SOURCE_1024mvCounts = 66 , ///< The number of ADC counts that result from a 1.024V reading
	SW_DATA_SOURCE_FRAMES_RUN_LSW = 67, ///< The number of frames run since reset, least significant 16 bits
	SW_DATA_SOURCE_FRAMES_RUN_MSW = 68, ///< The number of frames run since reset, most significant 16 bits
	SW_DATA_SOURCE_OVERRUN_FRAMES = 69, ///< The number of frames that ran more than 1mS
	SW_DATA_SOURCE_TEMPERATURE = 70, ///<The internal core temperature expressed in 100ths deg C
	SW_DATA_SOURCE_PACKETS_RECEIVED = 71,  ///<The nubmer of incoming command packets that have been processed since reset (rolls over at 65535)
	SW_DATA_SOURCE_ERRORS = 72,  ///<The number of incoming packets that have caused errors since reset (rolls over at 65535)  
	SW_DATA_SOURCE_DROPPED_FRAMES = 73 , ///<The number of times since reset that a frame ran so far behind that it crossed two subsequent 1ms boundaries, causing a permanent lost frame
	SW_DATA_SOURCE_SYSTEM_UTILIZATION = 74 , ///< A number between 0 and 65535 that scales to the average length of pin processing frames between 0 and 1000mS
	SW_DATA_SOURCE_VCC_mVOLTS = 75, ///<The system source voltage in mV
	SW_DATA_SOURCE_VBG_COUNTS_VS_VREF = 76, ///< A/D conversion of VBG against VRef .  Used for mfg calibration
	SW_DATA_SOURCE_LFSR = 78,
	SW_DATA_SOURCE_PIN_0_MV = 100, ///< Pin 0 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_1_MV = 101, ///< Pin 1 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_2_MV = 102, ///< Pin 2 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_3_MV = 103, ///< Pin 3 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_4_MV = 104, ///< Pin 4 public output expressed in mV (for analog modes only)
	//NOT ANALOG            SW_DATA_SOURCE_PIN_5_MV = 105,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_6_MV = 106,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_7_MV = 107,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_8_MV = 108,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_9_MV = 109,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_10_MV = 110,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_11_MV = 111,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_12_MV = 112,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_13_MV = 113,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_14_MV = 114,
	//NOT ANALOG            SW_DATA_SOURCE_PIN_15_MV = 115,
	SW_DATA_SOURCE_PIN_16_MV = 116, ///< Pin 16 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_17_MV = 117, ///< Pin 17 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_18_MV = 118, ///< Pin 18 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_19_MV = 119, ///< Pin 19 public output expressed in mV (for analog modes only)
};

#define ERROR_HOST_INCORRECT_NUMBER_BYTES_WRITTEN 0x10000 ///< Write routine returned wrong number of bytes
#define ERROR_HOST_DATA_TOO_LONG 0x10001  ///< endTransmission returned data too long
#define ERROR_HOST_NACK_ADDRESS 0x10002  ///< endTransmission returned address NACK
#define ERROR_HOST_NACK_DATA 0x10003   ///< endTransmission returned data NACK
#define ERROR_HOST_OTHER_I2C_ERROR 0x10004 ///< endTransmission returned other error

#define WOMBAT_MAXIMUM_PINS 19

enum class SerialWombatCommands
{
	CMD_ECHO ='!', 
	CMD_READ_BUFFER_ASCII = 'G',
	CMD_ASCII_SET_PIN ='P', 
	CMD_RESET = 'R', 
	CMD_SET_BUFFER_ASCII = 'S',
	CMD_RESYNC = 'U',
	CMD_VERSION = 'V',
	CMD_SUPPLYVOLTAGE = 'v',
	COMMAND_BINARY_READ_PIN_BUFFFER = 0x81,
	COMMAND_BINARY_SET_PIN_BUFFFER = 0x82,
	COMMAND_BINARY_READ_USER_BUFFER = 0x83,
	COMMAND_BINARY_WRITE_USER_BUFFER = 0x84,
	COMMAND_BINARY_WRITE_USER_BUFFER_CONTINUE = 0x85,
	COMMAND_BINARY_QUEUE_INITIALIZE = 0x90,
	COMMAND_BINARY_QUEUE_ADD_BYTES = 0x91,
	COMMAND_BINARY_QUEUE_ADD_7BYTES = 0x92,
	COMMAND_BINARY_QUEUE_READ_BYTES = 0x93,
	COMMAND_BINARY_QUEUE_INFORMATION = 0x94,
	COMMAND_BINARY_CONFIGURE = 0x9F,
	COMMAND_BINARY_READ_RAM = 0xA0,
	COMMAND_BINARY_READ_FLASH = 0xA1,
	COMMAND_BINARY_READ_EEPROM = 0xA2,
	COMMAND_BINARY_WRITE_RAM = 0xA3,
	COMMAND_BINARY_WRITE_FLASH = 0xA4,
	COMMAND_CALIBRATE_ANALOG = 0xA5,
	COMMAND_ENABLE_2ND_UART = 0xA6,
	COMMAND_UART0_TX_7BYTES = 0xB0,
	COMMAND_UART0_RX_7BYTES = 0xB1,
	COMMAND_UART1_TX_7BYTES = 0xB2,
	COMMAND_UART1_RX_7BYTES = 0xB3,
	CONFIGURE_PIN_MODE0 = 200,
	CONFIGURE_PIN_MODE1 = 201,
	CONFIGURE_PIN_MODE2 = 202,
	CONFIGURE_PIN_MODE3 = 203,
	CONFIGURE_PIN_MODE4 = 204,
	CONFIGURE_PIN_MODE5 = 205,
	CONFIGURE_PIN_MODE6 = 206,
	CONFIGURE_PIN_MODE7 = 207,
	CONFIGURE_PIN_MODE8 = 208,
	CONFIGURE_PIN_MODE9 = 209,
	CONFIGURE_PIN_MODE10 = 210,
	CONFIGURE_PIN_OUTPUTSCALE = 210,
	CONFIGURE_PIN_INPUTPROCESS = 211,
	CONFIGURE_PIN_MODE_HW_0 = 220,
	CONFIGURE_CHANNEL_MODE_HW_1 = 221,
	CONFIGURE_CHANNEL_MODE_HW_2 = 222,
	CONFIGURE_CHANNEL_MODE_HW_3 = 223,

};


typedef enum {
	PIN_MODE_DIGITALIO = 0,
	PIN_MODE_CONTROLLED = 1,
	PIN_MODE_ANALOGINPUT = 2,
	PIN_MODE_SERVO = 3,
	PIN_MODE_THROUGHPUT_CONSUMER = 4,
	PIN_MODE_QUADRATUREENCODER = 5,
	PIN_MODE_WATCHDOG = 7,
	PIN_MODE_PROTECTED_OUTPUT = 8,
	PIN_MODE_DEBOUNCE = 10,
	PIN_MODE_TM1637 = 11,
	PIN_MODE_WS2812 = 12,
	PIN_MODE_SW_UART = 13,
	PIN_MODE_INPUT_PROCESSOR = 14,
	PIN_MODE_MATRIX_KEYPAD = 15,
	PIN_MODE_PWM = 16,
	PIN_MODE_UART_RX_TX = 17,  //UART 0 externally is UART 1 internally
	PIN_MODE_PULSETIMER = 18,
	
	PIN_MODE_FRAME_TIMER = 21,
	PIN_MODE_SW18AB_CAPTOUCH = 22,
	PIN_MODE_UART1_RX_TX = 23,
	PIN_MODE_RESISTANCEINPUT = 24,
	PIN_MODE_PULSE_ON_CHANGE = 25,
	PIN_MODE_HS_SERVO = 26,
	PIN_MODE_ULTRASONIC_DISTANCE = 27,
	PIN_MODE_UNKNOWN = 255
}SerialWombatPinMode_t;



/*! \brief Class for a Serial Wombat chip.  Each Serial Wombat chip on a project should have its own instance.

This class describes the capabilties of a Serial Wombat Chip that are not Pin Mode functionalities

*/
class SerialWombatChip
{
public:
	SerialWombatChip();
	/// \brief initialize a Serial Wombat chip to use a Serial Interface.
	/// 
	/// This call causes the Serial Wombat chip to be reset through its reset
	/// command as the first operation.  The Serial Wombat chips's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param serial Serial Interface to be used by the Serial Wombat library	
	int16_t begin(HardwareSerial &serial);
	/// \brief initialize a Serial Wombat chip to use a Serial Interface.
	/// 
	/// The reset parameter determines if the Serial Wombat chip is reset
	/// prior to other initialization operations.  If false,
	/// then any prior pin modes and configurations may still be in
	/// place.
	/// The Serial Wombat chips's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param serial Serial Interface to be used by the Serial Wombat library
	/// \param reset Whether or not to reset the Serial Wombat chip via command as the first initialization operation
	int16_t begin(HardwareSerial& serial, bool reset);

	/// \brief initialize a Serial Wombat chip to use a specified I2C Interface and address.
	/// 
	/// This call causes the Serial Wombat chip to be reset through its reset
	/// command as the first operation.  The Serial Wombat chip's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param wire I2C interface to be used by the Serial Wombat Library
	/// \param i2cAddress I2C Follower address of the Serial Wombat chip commanded by this instance
	int16_t begin(TwoWire &wire, uint8_t i2cAddress);
	
	/// \brief initialize a Serial Wombat chip to use the default Wire I2C Interface and specified address.
	/// 
	/// This call causes the Serial Wombat chip to be reset through its reset
	/// command as the first operation.  The Serial Wombat chip's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param i2cAddress I2C Follower address of the Serial Wombat chip commanded by this instance
	int16_t begin(uint8_t i2cAddress);

	/// \brief initialize a Serial Wombat chip to use a specified I2C Interface and address.
	/// 
	/// This call causes the Serial Wombat chip to optionally reset through its reset
	/// command as the first operation.  The Serial Wombat chip's source 
	/// voltage is then read as well as its version.
	/// 
	/// \param wire I2C interface to be used by the Serial Wombat Library
	/// \param i2cAddress I2C Follower address of the Serial Wombat chip commanded by this instance
	/// \param reset Whether or not to reset the Serial Wombat chip via command as the first initialization operation. If false,
	/// then any prior pin modes and configurations may still be in
	/// place.
	int16_t begin(TwoWire& wire, uint8_t i2cAddress, bool reset);
	~SerialWombatChip();

	/// \brief Send an 8 byte packet to the Serial Wombat chip and wait for 8 bytes back
	/// 
	/// This method sends 8 bytes via I2C or Serial and blocks until 8 bytes are receieved
	/// back
	/// 
	/// \param tx address of an array of 8 bytes to send
	/// \param rx address of an array of 8 bytes into which to put response.
	/// \return The number of bytes received as a response, or a negative value if an error was returned from the Serial Wombat chip
	int sendPacket( uint8_t tx[], uint8_t rx[]);

	/// \brief Send an 8 byte packet to the Serial Wombat chip.
	/// 
	/// This method sends 8 bytes and processes the response to check for errors.
	/// 
	/// 
	/// \param tx address of an array of 8 bytes to send
	/// \return The number of bytes received as a response, or a negative value if an error was returned from the Serial Wombat chip
	int sendPacket(uint8_t tx[]);

	/// \brief Send an 8 byte packet to the Serial Wombat chip, don't wait for a response.
		/// 
		/// This method sends 8 bytes.  Used for resetting the chip prior to bootloading
		/// 
		/// 
		/// \param tx address of an array of 8 bytes to send
		/// \return returns a non-error 0 or higher

	int sendPacketNoResponse(uint8_t tx[]);

	/// \brief Request version string (combined model and firmware) and return pointer to it
	/// 
	/// This queries the Serial Wombat chip for the 7 characters:   product line (1 character)
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

	/// \brief Read the 16 Bit public data associated with a Serial Wombat Pin Mode 
	/// 
	/// Reads and returns the 16 bit value associated with a Serial Wombat Pin Mode.
	/// Additionally, values of 65 and higher have special meanings.  See
	/// Serial Wombat firmware documentation for details.
	/// \return 16 bit public data for pin specified
	/// \param dataSource The pin (or special meaning value) for which to retreive data
	uint16_t readPublicData(SerialWombatDataSource dataSource);

	/// \brief Write a 16 bit value to a Serial Wombat pin Mode
	/// \param pin The pin number to which to write
	/// \param value The 16 bit value to write
	uint16_t writePublicData(uint8_t pin, uint16_t value);

	/// \brief Measure the Serial Wombat chip's Supply voltage
	/// 
	/// Causes the Serial Wombat chip to measure the counts for the 
	/// internal reference voltage.  The Arduino library
	/// then converts these counts to a Source votlage in mV
	/// 
	/// \return The Serial Wombat chip's source voltage in mV
	uint16_t readSupplyVoltage_mV(void);

	/// \brief Measure the Serial Wombat chip's internal temperature
	/// 
	/// This command is only supported by the SerialWombat 18 Series.
	/// The Arduino library will return 25 deg. C for other models
	/// 
	/// This value is low accuracy unless a calibration has been performed
	/// 
	/// \return The Serial Wombat chip's temperature in 100ths deg C
	int16_t readTemperature_100thsDegC(void);


	/// \brief Send a reset command to the Serial Wombat chip
	/// 
	/// Sends a reset command to the Serial Wombat chip.  The calling function
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
	/// It does not make use of advanced Serial Wombat chip's functionality such as averaging and
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


	/// \brief Send a version request to the Serial Wombat chip
	/// 
	/// This function queries the Serial Wombat chip for its model and version
	/// and stores the result in the public members model and fwVersion
	/// as zero terminated strings.  Returns true if the response is
	/// likely a proper version response and false otherwise.
	/// 
	/// \return TRUE if response was likely a valid version, FALSE otherwise
	bool queryVersion();

	/// \brief Get the number of 1mS frames that have been executed since Serial Wombat chip reset
	/// 
	/// This value should be roughly equal to the mS since reset.  It will vary based on the Serial Wombat chip's
	/// internal oscillator variation, and may run slow if Overflow frames are occuring.
	uint32_t readFramesExecuted();

	/// \brief Get the number of times an overflow Frame has occured.
	/// 
	/// This value increments each time the Serial Wombat firmware determines it is time to start a new 1 mS frame,
	/// but the previous frame is still executing.  Indicates processor loading over 100% of real-time.  Overflows
	/// back to 0 when incremented from 65535.

	uint16_t readOverflowFrames();

	/// \brief Jump to Bootloader and wait for a UART download of new firmware
	/// 
	/// This function causes a reset of the Serial Wombat chip and causes it to remain
	/// in the bootloader until a power-cycle occurs.  This allows loading new
	/// firmware via a UART connection to the bottom two pins (DIP pins 4 (RX) and 5(TX))
	/// on the SW4A/SW4B.  When jumping to boot the TX pin will go high.  All other 
	/// communication or functional pins will become inputs (i.e. PWMS, etc will stop).
	void jumpToBoot();

	/// \brief Read Address from RAM based on 16 bit address
	///
	/// Most Arduino users should not need this command.
	/// 
	/// This command can be used to read variables and registers within the Serial Wombat Chip
	/// Note that reading registers may have unintended side effects.  See the microcontroller datasheet
	/// for details.
	/// 
	/// Note that Note that the PIC16F15214 used in the SW4A and SW4B chips
	/// is a Microchip Enhanced Mid-Range chip with both a banked RAM area and a Linear RAM area at an offset address.
	/// See the datasheet for details.  It's wierd to people who are unfamilliar with it.  The same location
	/// can have two different addresses.
	/// 
	/// Addresses are not validated to be available in a given chip's address range.
	/// 
	/// \param address  A 16-bit address pointing to a location in the Serial Wombat Chip's memory map
	/// 
	/// \return An 8 bit value returned from the Serial Wombat chip.
	uint8_t readRamAddress(uint16_t address);

	/// \brief Write byte to Address in RAM based on 16 bit address
	///
	/// Most Arduino users should not need this command.
	/// 
	/// This command can be used to write variables and registers within the Serial Wombat Chip
	/// Note that write registers may have unintended side effects.  See the microcontroller datasheet
	/// for details.
	/// 
	/// Note that Note that the PIC16F15214 used in the SW4A and SW4B chips
	/// is a Microchip Enhanced Mid-Range chip with both a banked RAM area and a Linear RAM area at an offset address.
	/// See the datasheet for details.  It's wierd to people who are unfamilliar with it.  The same location
	/// can have two different addresses.
	/// 
	/// Addresses are not validated to be available in a given chip's address range.
	/// 
	/// \param address  A 16-bit address pointing to a location in the Serial Wombat Chip's memory map
	/// \param value An 8 bit value to be written to RAM

	int16_t writeRamAddress(uint16_t address, uint8_t value);

	/// \brief Read Address from Flash based on 32 bit address
	///
	/// Most Arduino users should not need this command.
	/// 
	/// This command can be used to read flash locations within the Serial Wombat Chip
	/// 
	/// 
	/// Addresses are not validated to be available in a given chip's address range.
	/// 
	/// \param address  A 32-bit address pointing to a location in the Serial Wombat Chip's memory map
	/// 
	/// \return An 32 bit value returned from the Serial Wombat chip.  32 bits are used to accomodate different chips.  The SW18 series has a 24 bit flash word, whereas the SW4A and SW4B have a 14 bit word.

	uint32_t readFlashAddress(uint32_t address);

	/// \brief Shuts down most functions of the Serial Wombat chip reducing power consumption
	/// 
	/// This command stops the Serial Wombat chip's internal clock, greatly reducing power consumption.
	/// The host is responsible for configuring outputs to a safe state prior to calling sleep.
	/// 
	/// \warning This command does not cause any sort of shutdown routine to run.  The chip just stops.  \
	/// Outputs, including PWM, Servo and Protected Outputs, may retain their logic levels \
	/// at the moment the sleep command is processed.  In other words, they may stay high or low as long as the chip is in sleep.
	void sleep();

	/// \brief Called to send a dummy packet to the Serial Wombat chip to wake it from sleep and ready it for other commands
	void wake();

	/// \brief Returns true if the instance received a model number corresponding to the Serial Wombat 18 series of chips at begin
	bool isSW18();

	/// \brief Erases a page in flash.  Intended for use with the Bootloader, not by end users outside of bootloading sketch
	int16_t eraseFlashPage(uint32_t address);
	/// \brief Writes a row in flash.  Intended for use with the Bootloader, not by end users outside of bootloading sketc
	int16_t writeFlashRow(uint32_t address);

	/// Stores the last value retreived by readSupplyVoltage_mV().  Used by SerialWombatAnalogInput 
	/// class to calculate mV outputs from retreived A/D counts.
	/// Don't access this member, as it may become private and SerialWombatAnalog input be made
	/// a friend of SerialWombat in the future.  Call readSupplyVoltage_mV instead.
	uint16_t _supplyVoltagemV = 0;

	/// Contains the last model retreived by queryVersion() as a zero-terminated string
	uint8_t model[4] = { 0 };

	/// Contains the last firmware Version retreived by queryVersion() as a zero-terminated string
	uint8_t fwVersion[4] = { 0 };

	uint8_t uniqueIdentifier[16];
	uint8_t uniqueIdentifierLength = 0;

	uint16_t deviceIdentifier;
	uint16_t deviceRevision;
	/// Incremented every time a communication or command error is detected.
	uint32_t errorCount = 0;

	/// Set to true if boot mode is indicated by a version query
	bool inBoot = false;

	/// \brief Set a pin to be a throughput monitoring pin. 
	///
	/// This pin goes high when pin processing begins in each 1mS frame, and goes low
	/// after pin processing is complete.  This allows the CPU utilization of the Serial
	/// Wombat chip to be measured using a logic analyzer.  This function can only be applied
	/// to one pin, and is only disabled by resetting the chip.  This function is supported on
	/// the SW18AB chip.  It is not supported on the SW4 series of chips.
	int16_t setThroughputPin(uint8_t pin);

	/// \brief Write bytes to the User Memory Buffer in the Serial Wombat chip
	/// \param index The index into the User Buffer array of bytes where the data should be loaded
	/// \param buffer a pointer to an array of bytes to be loaded into the User Buffer array
	/// \param number of bytes to load
	/// \return Number of bytes written or error code.
	int writeUserBuffer(uint16_t index, uint8_t* buffer, uint16_t count);

	/// \brief Enable UART command interface in addition to I2C (SW18AB Only)
	/// \param 2nd communication interface is enabled
	/// 
	/// \return 0 or positive for success or negative error code
	/// 
	int16_t enable2ndCommandInterface(bool enabled);


	/// \brief Search the I2C Bus addresses 0x68 to 0x6F for I2C devices, and test to see if they respond to
	/// Serial Wombat version commands.  Returns first address that responds properly or 0 if none found
	/// 
	/// \return I2C address of first found Seirla Wombat chip or 0 if none found
	static uint8_t find();

	/// \brief Search the I2C Bus addresses 0x68 to 0x6F for I2C devices, and test to see if they respond to
	/// Serial Wombat version commands.  Returns first address that responds properly or 0 if none found
	/// 
	/// \param keepTrying if True, go into a loop and do not exit until a Serial Wombat Chip is found
	/// 
	/// \return I2C address of first found Seirla Wombat chip or 0 if none found

	static uint8_t find(bool keepTrying);
	int16_t lastErrorCode = 0;

private:

	uint8_t address = 0;
	char version[8] = { 0 };
	HardwareSerial * Serial = NULL;
	TwoWire* i2cInterface = NULL;
	uint8_t _pinmode[WOMBAT_MAXIMUM_PINS]={}; // Includes Pullup
	bool _pullDown[WOMBAT_MAXIMUM_PINS]={};
	bool _openDrain[WOMBAT_MAXIMUM_PINS]={};
	bool _highLow[WOMBAT_MAXIMUM_PINS] = {};
	bool _asleep = false;
	void configureDigitalPin(uint8_t pin, uint8_t highLow);
	uint32_t sendReadyTime = 0;
	int16_t initialize();
	void readUniqueIdentifier();
	void readDeviceIdentifier();
};

/// \brief This class name is depricated.  Do not use for new development.  Use SerialWombatChip instead.
class SerialWombat : public SerialWombatChip {};  
/// Convert a uint16_t to two bytes in little endian format for array initialization
#define SW_LE16(_a)  (uint8_t)((_a) & 0xFF), (uint8_t)((_a) >>8)  

/// Convert a uint32_t to four bytes in little endian format for array initialization
#define SW_LE32(_a)  (uint8_t)((_a) & 0xFF), (uint8_t)((_a) >>8) , (uint8_t)((_a) >>16), (uint8_t)((_a) >>24)


class SerialWombatPin
{
public:

	SerialWombatPin(SerialWombatChip& serialWombatChip);

	/// \brief Read the 16 Bit public data associated with this pin 
	/// 
	/// Reads and returns the 16 bit value associated with this pin.
	/// \return 16 bit public data for this pin.
	uint16_t readPublicData() 
	{
		return _sw.readPublicData(_pin);
	};

	/// \brief Set pin to INPUT or OUTPUT
	/// 
	/// This method matches the Arduino Digital io pinMode command
	/// It should only be used on pins that have not been configured to a more
	/// sophisticated (e.g. debounce or servo) pin mode.  
	/// 
	/// \param mode Valid values are INPUT or OUTPUT as defined by arduino.  Do 
	/// not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	void pinMode(uint8_t mode);

	/// \brief Set pin to INPUT or OUTPUT, with options for pull Ups and open Drain settings
	/// 
	/// \param mode Valid values are INPUT, OUTPUT or INPUT_PULLUP as defined by arduino.  Do 
	/// not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	/// \param pullDown  If True, a weak pull down will be enabled on this pin (No effect on SW4A/SW4B)
	/// \param openDrain If True, output becomes openDrain output rather than push / pull
	void pinMode(uint8_t mode, bool pullDown, bool openDrain);

	/// \brief Set output pin High or Low
	/// 
	/// Before calling this function, the pin should be configured as an input or output with pinMode()
	/// \param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	/// \param val  Valid values are HIGH or LOW
	/// not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	void digitalWrite(uint8_t val);

	/// \brief Reads the state of the Pin
	/// 
	/// This function is based on the pin's public data, not a raw reading.
	/// 
	/// \return Returns LOW if pin is low or public data is 0.  Returns HIGH if pin is high or public data is > 0
	int digitalRead();


	/// \brief Write a 16 bit value to this pin
	/// \param value The 16 bit value to write
	uint16_t writePublicData(uint16_t value) 
	{ return _sw.writePublicData(_pin, value); }

	uint8_t pin() {return _pin;}
	uint8_t swPinModeNumber() { return _pinMode; }


protected:
	uint8_t _pin = 255;
	SerialWombatChip& _sw;
	uint8_t _pinMode = 0;
};
#include "SerialWombatQueue.h"
#include "SerialWombatAbstractButton.h"
#include "SerialWombatAbstractProcessedInput.h"
#include "SerialWombatAbstractScaledOutput.h"
#include "SerialWombat18CapTouch.h"
#include "SerialWombatAnalogInput.h"
#include "SerialWombatDebouncedInput.h"
#include "SerialWombatMatrixKeypad.h"
#include "SerialWombatProcessedInputPin.h"
#include "SerialWombatProtectedOutput.h"
#include "SerialWombatPulseTimer.h"
#include "SerialWombatPWM.h"
#include "SerialWombatQuadEnc.h"
#include "SerialWombatResistanceInput.h"
#include "SerialWombatServo.h"
#include "SerialWombatTM1637.h"
#include "SerialWombatUART.h"
#include "SerialWombatUltrasonicDistanceSensor.h"
#include "SerialWombatWatchdog.h"
#include "SerialWombatWS2812.h"
#include "SerialWombatThroughputConsumer.h"

#endif
