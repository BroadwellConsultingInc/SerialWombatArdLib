#ifndef SERIAL_WOMBAT_H__
#define SERIAL_WOMBAT_H__

/*
Copyright 2020-2025 Broadwell Consulting Inc.

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
//#include "Serial.h" // Using "" rather than <> for compatibility with Visual C++ simulation project
#include "Wire.h"// Using "" rather than <> for compatibility with Visual C++ simulation project
#include "Arduino.h"
#ifndef INPUT 
#define INPUT 0x0
#endif


/*! \file SerialWombat.h
*/


#include "SerialWombatErrors.h"

/// Convert a uint16_t to two bytes in little endian format for array initialization
#define SW_LE16(_a)  (uint8_t)((_a) & 0xFF), (uint8_t)((_a) >>8)  

/// Convert a uint32_t to four bytes in little endian format for array initialization
#define SW_LE32(_a)  (uint8_t)((_a) & 0xFF), (uint8_t)((_a) >>8) , (uint8_t)((_a) >>16), (uint8_t)((_a) >>24)

#define ARRAY_UINT32(_array,_index) ((((uint32_t) _array[_index +3])<<24) + (((uint32_t) _array[_index +2])<<16) + (((uint32_t) _array[_index +1])<<8) + _array[_index])
#define SW18AB_LATEST_FIRMWARE 221
#define SW08B_LATEST_FIRMWARE 221
#define SW4B_LATEST_FIRMWARE 203

typedef enum
{
	SW_LOW = 0,
	SW_HIGH = 1,
	SW_INPUT = 2,
}SerialWombatPinState_t;


/// \brief A list of Serial Wombat public data sources
enum class SerialWombatDataSource{

	SW_DATA_SOURCE_PIN_0 = 0, ///< (0) 16 bit public data provided by Pin 0
	SW_DATA_SOURCE_PIN_1 = 1, ///< (1) 16 bit public data provided by Pin 1
	SW_DATA_SOURCE_PIN_2 = 2, ///< (2) 16 bit public data provided by Pin 2
	SW_DATA_SOURCE_PIN_3 = 3, ///< (3) 16 bit public data provided by Pin 3
	SW_DATA_SOURCE_PIN_4 = 4, ///< (4) 16 bit public data provided by Pin 4
	SW_DATA_SOURCE_PIN_5 = 5, ///< (5) 16 bit public data provided by Pin 5
	SW_DATA_SOURCE_PIN_6 = 6, ///< (6) 16 bit public data provided by Pin 6
	SW_DATA_SOURCE_PIN_7 = 7, ///< (7) 16 bit public data provided by Pin 7
	SW_DATA_SOURCE_PIN_8 = 8, ///< (8) 16 bit public data provided by Pin 8
	SW_DATA_SOURCE_PIN_9 = 9, ///< (9) 16 bit public data provided by Pin 9
	SW_DATA_SOURCE_PIN_10 = 10, ///< (10) 16 bit public data provided by Pin 10
	SW_DATA_SOURCE_PIN_11 = 11, ///< (11) 16 bit public data provided by Pin 11
	SW_DATA_SOURCE_PIN_12 = 12, ///< (12) 16 bit public data provided by Pin 12
	SW_DATA_SOURCE_PIN_13 = 13, ///< (13) 16 bit public data provided by Pin 13
	SW_DATA_SOURCE_PIN_14 = 14, ///< (14) 16 bit public data provided by Pin 14
	SW_DATA_SOURCE_PIN_15 = 15, ///< (15) 16 bit public data provided by Pin 15
	SW_DATA_SOURCE_PIN_16 = 16, ///< (16) 16 bit public data provided by Pin 16
	SW_DATA_SOURCE_PIN_17 = 17, ///< (17) 16 bit public data provided by Pin 17
	SW_DATA_SOURCE_PIN_18 = 18, ///< (18) 16 bit public data provided by Pin 18
	SW_DATA_SOURCE_PIN_19 = 19, ///< (19) 16 bit public data provided by Pin 19
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
	SW_DATA_SOURCE_INCREMENTING_NUMBER = 65, ///< (65) An number that increments each time it is accessed.
	SW_DATA_SOURCE_1024mvCounts = 66 , ///< (66) The number of ADC counts that result from a 1.024V reading
	SW_DATA_SOURCE_FRAMES_RUN_LSW = 67, ///< (67) The number of frames run since reset, least significant 16 bits
	SW_DATA_SOURCE_FRAMES_RUN_MSW = 68, ///< (68) The number of frames run since reset, most significant 16 bits
	SW_DATA_SOURCE_OVERRUN_FRAMES = 69, ///< (69) The number of frames that ran more than 1mS
	SW_DATA_SOURCE_TEMPERATURE = 70, ///< (70)The internal core temperature expressed in 100ths deg C
	SW_DATA_SOURCE_PACKETS_RECEIVED = 71, ///< (71) The nubmer of incoming command packets that have been processed since reset (rolls over at 65535)
	SW_DATA_SOURCE_ERRORS = 72, ///< (72)The number of incoming packets that have caused errors since reset (rolls over at 65535)  
	SW_DATA_SOURCE_DROPPED_FRAMES = 73, ///< (73) The number of times since reset that a frame ran so far behind that it crossed two subsequent 1ms boundaries, causing a permanent lost frame
	SW_DATA_SOURCE_SYSTEM_UTILIZATION = 74, ///< (74) A number between 0 and 65535 that scales to the average length of pin processing frames between 0 and 1000mS
	SW_DATA_SOURCE_VCC_mVOLTS = 75, ///< (75) The system source voltage in mV
	SW_DATA_SOURCE_VBG_COUNTS_VS_VREF = 76, ///< (76) A/D conversion of VBG against VRef .  Used for mfg calibration
						
 SW_DATA_SOURCE_RESET_REGISTER = 77, ///< Hardware dependent reset reason register contents
            SW_DATA_SOURCE_LFSR = 78, ///< A Linear FeedBack Shift register (32,7,5,3,2,1) based pseudo-random number generator
            SW_DATA_COM_ADDRESS_LOW = 79, ///< The Communications Address of the Device (Lower 16 bits) 
            SW_DATA_COM_ADDRESS_HIGH = 80, ///< The Communications Address of the Device (HIGHER 16 bits) Anticipated for CAN ID
	SW_DATA_SOURCE_0x55 = 85, ///< (85) 0x55 is a reserved value for resyncing.  Returns 0x55 0x55 
	SW_DATA_SOURCE_PIN_0_MV = 100, ///< (100) Pin 0 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_1_MV = 101, ///< (101) Pin 1 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_2_MV = 102, ///< (102) Pin 2 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_3_MV = 103, ///< (103) Pin 3 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_4_MV = 104, ///< (104) Pin 4 public output expressed in mV (for analog modes only)
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
	SW_DATA_SOURCE_PIN_16_MV = 116, ///< (116) Pin 16 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_17_MV = 117, ///< (117) Pin 17 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_18_MV = 118, ///< (118) Pin 18 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_PIN_19_MV = 119, ///< (119) Pin 19 public output expressed in mV (for analog modes only)
	SW_DATA_SOURCE_2HZ_SQUARE = 164, ///< (164) Square wave that alternates between 0 and 65535 every 256 frames
	SW_DATA_SOURCE_2HZ_SAW = 165,///< (165) Sawtooth wave that goes from  0 to 65535 to 0 every  512 frames
//            SW_DATA_SOURCE_2HZ_SIN = 166,
SW_DATA_SOURCE_1HZ_SQUARE = 167,///< (167) Square wave that alternates between 0 and 65535 every 512 frames
SW_DATA_SOURCE_1HZ_SAW = 168,///<  (168) Sawtooth wave that goes from  0 to 65535 to 0 every  1024 frames
//           SW_DATA_SOURCE_1HZ_SIN = 169,
SW_DATA_SOURCE_2SEC_SQUARE = 170,///< (170)Square wave that alternates between 0 and 65535 every 1024 frames
SW_DATA_SOURCE_2SEC_SAW = 171,///< (171)Sawtooth wave that goes from  0 to 65535 to 0 every  2048 frames
//          SW_DATA_SOURCE_2SEC_SIN = 172,
SW_DATA_SOURCE_8SEC_SQUARE = 173,///< (173)Square wave that alternates between 0 and 65535 every 4096 frames
SW_DATA_SOURCE_8SEC_SAW = 174,///< (174)Sawtooth wave that goes from  0 to 65535 to 0 every  8192 frames
//         SW_DATA_SOURCE_8SEC_SIN = 175,
SW_DATA_SOURCE_65SEC_SQUARE = 176,///< (176) Square wave that alternates between 0 and 65535 every 32768 frames
SW_DATA_SOURCE_65SEC_SAW = 177,///< (177 )Sawtooth wave that goes from  0 to 65535 to 0 every  65536 frames
//        SW_DATA_SOURCE_65SEC_SIN = 178,
         	SW_DATA_SOURCE_NONE = 255,///< (255 ) Used to mean "No Source Selected"
};

#define ERROR_HOST_INCORRECT_NUMBER_BYTES_WRITTEN 0x10000 ///< Write routine returned wrong number of bytes
#define ERROR_HOST_DATA_TOO_LONG 0x10001  ///< endTransmission returned data too long
#define ERROR_HOST_NACK_ADDRESS 0x10002  ///< endTransmission returned address NACK
#define ERROR_HOST_NACK_DATA 0x10003   ///< endTransmission returned data NACK
#define ERROR_HOST_OTHER_I2C_ERROR 0x10004 ///< endTransmission returned other error

#define WOMBAT_MAXIMUM_PINS 20

enum class SerialWombatCommands
{
	CMD_ECHO ='!', ///< ('!')
	CMD_READ_BUFFER_ASCII = 'G',///< ('G')
	CMD_ASCII_SET_PIN ='P', ///< ('P')
	CMD_RESET = 'R', ///< ('R')
	CMD_SET_BUFFER_ASCII = 'S',///< ('S')
	CMD_RESYNC = 'U',///< ('U')
	CMD_VERSION = 'V',///< ('V')
	CMD_SUPPLYVOLTAGE = 'v',///< ('v')
	COMMAND_BINARY_READ_PIN_BUFFFER = 0x81, ///< (0x81)
	COMMAND_BINARY_SET_PIN_BUFFFER = 0x82, ///< (0x82)
	COMMAND_BINARY_READ_USER_BUFFER = 0x83, ///< (0x83)
	COMMAND_BINARY_WRITE_USER_BUFFER = 0x84, ///< (0x84)
	COMMAND_BINARY_WRITE_USER_BUFFER_CONTINUE = 0x85, ///< (0x85)
	COMMAND_BINARY_PIN_POLL_THRESHOLD = 0x8F, ///< (0x8F)
	COMMAND_BINARY_QUEUE_INITIALIZE = 0x90, ///< (0x90)
	COMMAND_BINARY_QUEUE_ADD_BYTES = 0x91, ///< (0x91)
	COMMAND_BINARY_QUEUE_ADD_7BYTES = 0x92, ///< (0x92)
	COMMAND_BINARY_QUEUE_READ_BYTES = 0x93, ///< (0x93)
	COMMAND_BINARY_QUEUE_INFORMATION = 0x94, ///< (0x94)
	COMMAND_BINARY_QUEUE_CLONE = 0x95, ///< (0x95)
	COMMAND_BINARY_CONFIG_DATALOGGER = 0x96, ///< (0x96)
	COMMAND_BINARY_CONFIGURE = 0x9F, ///< (0x9F)
	COMMAND_BINARY_READ_RAM = 0xA0, ///< (0xA0)
	COMMAND_BINARY_READ_FLASH = 0xA1, ///< (0xA1)
	COMMAND_BINARY_READ_EEPROM = 0xA2, ///< (0xA2)
	COMMAND_BINARY_WRITE_RAM = 0xA3, ///< (0xA3)
	COMMAND_BINARY_WRITE_FLASH = 0xA4, ///< (0xA4)
	COMMAND_CALIBRATE_ANALOG = 0xA5, ///< (0xA5)
	COMMAND_ENABLE_2ND_UART = 0xA6, ///< (0xA6)
	COMMAND_READ_LAST_ERROR_PACKET = 0xA7, ///< (0xA7)
	COMMAND_UART0_TX_7BYTES = 0xB0, ///< (0xB0)
	COMMAND_UART0_RX_7BYTES = 0xB1, ///< (0xB1)
	COMMAND_UART1_TX_7BYTES = 0xB2, ///< (0xB2)
	COMMAND_UART1_RX_7BYTES = 0xB3, ///< (0xB3)
	COMMAND_BINARY_TEST_SEQUENCE = 0xB4,///< (0xB4)
	COMMAND_BINARY_RW_PIN_MEMORY = 0xB5,///< (0xB5)
	COMMAND_CAPTURE_STARTUP_SEQUENCE = 0xB6,///< (0xB6)
	COMMAND_ADJUST_FREQUENCY = 0xB7,///< (0xB7)
        COMMAND_SET_PIN_HW = 0xB8, ///< (0xB8)
        COMMAND_BINARY_SET_ADDRESS = 0xB9, ///< (0xB9)
	CONFIGURE_PIN_MODE0 = 200, ///< (200)
	CONFIGURE_PIN_MODE1 = 201, ///< (201)
	CONFIGURE_PIN_MODE2 = 202, ///< (202)
	CONFIGURE_PIN_MODE3 = 203, ///< (203)
	CONFIGURE_PIN_MODE4 = 204, ///< (204)
	CONFIGURE_PIN_MODE5 = 205, ///< (205)
	CONFIGURE_PIN_MODE6 = 206, ///< (206)
	CONFIGURE_PIN_MODE7 = 207, ///< (207)
	CONFIGURE_PIN_MODE8 = 208, ///< (208)
	CONFIGURE_PIN_MODE9 = 209, ///< (209)
	CONFIGURE_PIN_MODE10 = 210, ///< (210)
	CONFIGURE_PIN_OUTPUTSCALE = 210, ///< (210)
	CONFIGURE_PIN_MODE_DISABLE = 219, ///< (219)
	CONFIGURE_PIN_INPUTPROCESS = 211, ///< (211)
	CONFIGURE_CHANNEL_MODE_CHECK_MODE_SUPPORTED = 218, ///< (218)
	CONFIGURE_PIN_MODE_HW_0 = 220, ///< (220)
	CONFIGURE_CHANNEL_MODE_HW_1 = 221, ///< (221)
	CONFIGURE_CHANNEL_MODE_HW_2 = 222, ///< (222)
	CONFIGURE_CHANNEL_MODE_HW_3 = 223, ///< (223)

};


typedef enum {
	PIN_MODE_DIGITALIO = 0, ///< (0)
	PIN_MODE_CONTROLLED = 1, ///< (1)
	PIN_MODE_ANALOGINPUT = 2, ///< (2)
	PIN_MODE_SERVO = 3, ///< (3)
	PIN_MODE_THROUGHPUT_CONSUMER = 4, ///< (4)
	PIN_MODE_QUADRATUREENCODER = 5, ///< (5)
	PIN_MODE_HBRIDGE = 6, ///<(6)
	PIN_MODE_WATCHDOG = 7, ///< (7)
	PIN_MODE_PROTECTED_OUTPUT = 8, ///< (8)
	PIN_MODE_DEBOUNCE = 10, ///< (10)
	PIN_MODE_TM1637 = 11, ///< (11)
	PIN_MODE_WS2812 = 12, ///< (12)
	PIN_MODE_SW_UART = 13, ///< (13)
	PIN_MODE_INPUT_PROCESSOR = 14, ///< (14)
	PIN_MODE_MATRIX_KEYPAD = 15, ///< (15)
	PIN_MODE_PWM = 16, ///< (16)
	PIN_MODE_UART_RX_TX = 17, ///< (17)  
	PIN_MODE_PULSETIMER = 18, ///< (18)
	PIN_MODE_FRAME_TIMER = 21, ///< (21)
	PIN_MODE_SW18AB_CAPTOUCH = 22, ///< (22)
	PIN_MODE_UART1_RX_TX = 23, ///< (23)
	PIN_MODE_RESISTANCEINPUT = 24, ///< (24)
	PIN_MODE_PULSE_ON_CHANGE = 25, ///< (25)
	PIN_MODE_HS_SERVO = 26, ///< (26)
	PIN_MODE_ULTRASONIC_DISTANCE = 27, ///< (27)
	PIN_MODE_LIQUIDCRYSTAL = 28, ///< (28)
	PIN_MODE_HS_CLOCK = 29, /// < (29)
	PIN_MODE_HS_COUNTER = 30, ///< (30)
	PIN_MODE_VGA = 31, ///<(31)
	PIN_MODE_PS2KEYBOARD = 32, ///<(32)
	PIN_MODE_I2C_CONTROLLER = 33, ///<(33)
	PIN_MODE_QUEUED_PULSE_OUTPUT = 34, ///<(34)
	PIN_MODE_FREQUENCY_OUTPUT = 36, ///<(36)
	PIN_MODE_IRRX = 37, ///<(37)
	PIN_MODE_BLINK = 40, ///<(40)
	PIN_MODE_UNKNOWN = 255, ///< (0xFF)
}SerialWombatPinMode_t;

class SerialWombatChip;

typedef void  (*SerialWombatErrorHandler_t) (uint16_t errorNumber, SerialWombatChip* sw);

/*! \brief Class for a Serial Wombat chip.  Each Serial Wombat chip on a project should have its own instance.

This class describes the capabilties of a Serial Wombat Chip that are not Pin Mode functionalities.  This
class is used for all variety of Serial Wombat chips and is the parent class for Serial Wombat Boards
such as PCB0030, PCB0031, etc.

Certain features may not be available on all models of Serial Wombat chips. 

*/
class SerialWombatChip
{
private:

	char version[8] = { 0 };
	HardwareSerial * Serial = NULL;
	TwoWire* i2cInterface = NULL;
	uint8_t _pinmode[WOMBAT_MAXIMUM_PINS]={}; // Includes Pullup
	bool _pullDown[WOMBAT_MAXIMUM_PINS]={};
	bool _openDrain[WOMBAT_MAXIMUM_PINS]={};
	bool _highLow[WOMBAT_MAXIMUM_PINS] = {};
	bool _asleep = false;
	SerialWombatErrorHandler_t errorHandler = NULL;
	bool _currentlyCommunicating = false;
public:
/*!
	Stores the last value retreived by readSupplyVoltage_mV().  Used by SerialWombatAnalogInput 
	class to calculate mV outputs from retreived A/D counts.
	Don't access this member, as it may become private and SerialWombatAnalog input be made
	a friend of SerialWombat in the future.  Call readSupplyVoltage_mV instead.
*/
	uint16_t _supplyVoltagemV = 0;

	/// Contains the last model retreived by queryVersion() as a zero-terminated string
	uint8_t model[4] = { 0 };

	/// Contains the last firmware Version retreived by queryVersion() as a zero-terminated string
	uint8_t fwVersion[4] = { 0 };

/*!
	Contains the unique identifier stored in microcontroller at its manufacturing.  
	Length and format vary by model.  uniqueIdentifier holds the length in bytes.  
	Call queryVersion() to populate this value.
*/
	uint8_t uniqueIdentifier[16];

/*!
	Contains the unique identifier length in bytes.  This value varies by model. 
	Call queryVersion() to populate this value.
*/
	uint8_t uniqueIdentifierLength = 0;

/*!
	Contains a Microchip device identifier for Microchip based Serial Wombat Models.
	Call queryVersion() to populate this value.
*/
	uint16_t deviceIdentifier;

/*!
	Contains a Microchip device revision for Microchip based Serial Wombat Models.
	Call queryVersion() to populate this value.
*/
	uint16_t deviceRevision;
	/// Incremented every time a communication or command error is detected.
	uint16_t errorCount = 0;

	/// Set to true if boot mode is indicated by a version query
	bool inBoot = false;

	/// The last error code returned as part of a protocol error message expressed as a positive integer
	int16_t lastErrorCode = 0;

	/*!
	@brief Configure a pin to a digital state with the same parameters as Arduino's digitalWrite() function.

	This function is convenient if not using SerialWombatPin class which provides additonal funcitonalities.

	@param pin The pin number to configure	
	@param highLow The desired state of the pin.  INPUT, LOW or HIGH or INPUT_PULLUP
	*/
	
	void configureDigitalPin(uint8_t pin, uint8_t highLow)
	{
		uint8_t tx[8] = { 200,pin,0,0,0,0,0,0x55 };
		uint8_t rx[8];
		switch (_pinmode[pin])
		{
		case INPUT: // Arduino input
		{
			tx[3] = 2; //Input
		}
		break;
		case OUTPUT:
		{
			if (highLow == LOW)
			{
				tx[3] = 0;
			}
			else if (highLow == HIGH)
			{
				tx[3] = 1;
			}
			else 
			{ 
				return; 
			}
		}
		break;
		case INPUT_PULLUP:
		{
			tx[3] = 2; //Input
			tx[4] = 1; //Pullup on
		}
		break;
		default:
		{
			return;
		}
		}
		tx[6] = _openDrain[pin];
		tx[5] = _pullDown[pin];
		sendPacket(tx, rx, true);
	}

	/*!
	@brief Used as a countdown after a reset command is issued
	*/
	uint32_t sendReadyTime = 0;

	/*!
	@brief An internal function used to set up the Serial Wombat chip and this class.  Call begin() instead of this function.
	*/
	int16_t initialize() 
	{
		lastErrorCode = 0;
		readVersion();
		readSupplyVoltage_mV();
		readUniqueIdentifier();
		readDeviceIdentifier();
		return(lastErrorCode);
	}
		
	/*!
	@brief Read the unique identifier from the Serial Wombat chip and store it in uniqueIdentifier.

	Each Serial Wombat Chip has a unique identifier stored in its flash memory at manufacturing.
	The length of this identifier varies by model.   See the public value uniqueIdentifierLength to determine the length of the identifier.
	*/
	void readUniqueIdentifier()
	{
		uniqueIdentifierLength = 0;
		if (version[0] == 'S' && version[1] == '0' && version[2] == '4')
		{ //16F15214
			for (uint32_t address = 0x8100; address <= 0x8108; ++address)
			{
				uint32_t data = readFlashAddress(address);
				uniqueIdentifier[uniqueIdentifierLength] = (uint8_t)data;
				++uniqueIdentifierLength;
				/* Always zero... leave out
				uniqueIdentifier[uniqueIdentifierLength] = (uint8_t)(data>>8);
				++uniqueIdentifierLength;
				*/
			}
		}
		else if (isSW18())
		{
			for (uint32_t address = 0x801600; address <= 0x801608; address += 2)
			{
				uint32_t data = readFlashAddress(address);
				uniqueIdentifier[uniqueIdentifierLength] = data;
				++uniqueIdentifierLength;
				uniqueIdentifier[uniqueIdentifierLength] = data >> 8;
				++uniqueIdentifierLength;
				uniqueIdentifier[uniqueIdentifierLength] = data >> 16;
				++uniqueIdentifierLength;
			}
		}
		else if (isSW08())
		{
			//TODO
		}
		
	}

	/*!
	@brief Read the device identifier from the Serial Wombat chip and store it in deviceIdentifier.

	This function makes various chip identification (such as mask revision) available to the user.
	See the datasheet for the specific Serial Wombat chip's microcontroller for details on what this identifier means.
	*/

	void readDeviceIdentifier() 
	{
		if (version[0] == 'S' && version[1] == '0' && version[2] == '4')
		{ //16F15214
			
				uint32_t data = readFlashAddress(0x8006);
				deviceIdentifier = (uint16_t)data;
				data = readFlashAddress(0x8005);
				deviceRevision = (uint16_t)data;
		}
		else if (isSW18())
		{
			uint32_t data = readFlashAddress(0xFF0000);
			deviceIdentifier = (uint16_t)data;
			data = readFlashAddress(0xFF0002);
			deviceRevision = (uint16_t)data & 0xF;
		}
	}

	/*!
	@brief Convert an ASCII Error code from a received packet into an integer error code.
	@param rx The received packet
	@return The integer error code
	*/
	uint16_t returnErrorCode(uint8_t* rx)
	{
		uint16_t result = rx[1] - '0';
		result *= 10;
		result += rx[2] - '0';
		result *= 10;
		result += rx[3] - '0';
		result *= 10;
		result += rx[4] - '0';
		result *= 10;
		result += rx[5] - '0';
		return(result);
	}


	
public:
	SerialWombatChip();
/*!
	\brief initialize a Serial Wombat chip to use a Serial Interface.
	
	The reset parameter determines if the Serial Wombat chip is reset
	prior to other initialization operations.  If false,
	then any prior pin modes and configurations may still be in
	place.
	The Serial Wombat chips's source 
	voltage is then read as well as its version.
	
	\param serial Serial Interface to be used by the Serial Wombat library
	\param reset Whether or not to reset the Serial Wombat chip via command as the first initialization operation
*/
	int16_t begin(HardwareSerial& serial, bool reset = true)
	{
		Serial = &serial;
		Serial->begin(115200);
		Serial->setTimeout(2);
		Serial->write((uint8_t*)"UUUUUUUU", 8);
		delay(5);
		while (Serial->read() >= 0);
		if (reset)
		{
			hardwareReset();
			sendReadyTime = millis() + 1000;
			return(1);
		}
		else
		{
			return initialize();
		}
		
		
	}

/*!
	\brief initialize a Serial Wombat chip to use the default Wire I2C Interface and specified address.
	
	This call causes the Serial Wombat chip to be reset through its reset
	command as the first operation.  The Serial Wombat chip's source 
	voltage is then read as well as its version.
	
	\param i2cAddress I2C Follower address of the Serial Wombat chip commanded by this instance
*/
	int16_t begin(uint8_t i2cAddress);

/*!
	\brief initialize a Serial Wombat chip to use a specified I2C Interface and address.
	
	This call causes the Serial Wombat chip to optionally reset through its reset
	command as the first operation.  The Serial Wombat chip's source 
	voltage is then read as well as its version.
	
	\param wire I2C interface to be used by the Serial Wombat Library
	\param i2cAddress I2C Follower address of the Serial Wombat chip commanded by this instance
	\param reset Whether or not to reset the Serial Wombat chip via command as the first initialization operation. If false,
	then any prior pin modes and configurations may still be in
	place.
*/
	int16_t begin(TwoWire& wire, uint8_t i2cAddress, bool reset = true) 
	{	
			i2cInterface = &wire;
			address = i2cAddress;

			Wire.beginTransmission(i2cAddress);
			int error = Wire.endTransmission();


			if (error != 0)
			{
				return(-1);
			}

			if (reset)
			{
				hardwareReset();
				sendReadyTime = millis() + 250;
				return(1);
			}
			else
			{
				sendReadyTime = 0;
				return initialize();

			}
	}

	~SerialWombatChip();

/*!
	\brief Send an 8 byte packet to the Serial Wombat chip and wait for 8 bytes back
	
	This method sends 8 bytes via I2C or Serial and blocks until 8 bytes are receieved
	back
	
	\param tx address of an array of 8 bytes to send
	\param rx address of an array of 8 bytes into which to put response.
	\return The number of bytes received as a response, or a negative value if an error was returned from the Serial Wombat chip
*/
	int sendPacket( uint8_t tx[], uint8_t rx[]);

/*!
	\brief Send an 8 byte packet to the Serial Wombat chip.
	
	This method sends 8 bytes and processes the response to check for errors.
	
	
	\param tx address of an array of 8 bytes to send
	\return The number of bytes received as a response, or a negative value if an error was returned from the Serial Wombat chip
*/
	int sendPacket(uint8_t tx[]);

/*!
	\brief Send an 8 byte packet to the Serial Wombat chip and wait for 8 bytes back
	
	This method sends 8 bytes via I2C or Serial and blocks until 8 bytes are receieved
	back
	
	\param tx address of an array of 8 bytes to send
	\param rx address of an array of 8 bytes into which to put response.
	\param retryIfEchoDoesntMatch If true, the packet will be retried if the echo doesn't match
	\param beginningBytesToMatch The number of bytes at the beginning of the packet to check for a match
	\param endBytesToMatch The number of bytes at the end of the packet to check for a match
	\return The number of bytes received as a response, or a negative value if an error was returned from the Serial Wombat chip
*/
	int sendPacket(uint8_t tx[], uint8_t rx[], bool retryIfEchoDoesntMatch, uint8_t beginningBytesToMatch = 8, uint8_t endBytesToMatch = 0);
/*!
	\brief Send an 8 byte packet to the Serial Wombat chip.
	
	This method sends 8 bytes and processes the response to check for errors.
	
	
	\param tx address of an array of 8 bytes to send
	@param retryIfEchoDoesntMatch If true, the packet will be retried if the echo doesn't match
	\return The number of bytes received as a response, or a negative value if an error was returned from the Serial Wombat chip
*/
	int sendPacket(uint8_t tx[], bool retryIfEchoDoesntMatch);

/*!
	\brief Send an 8 byte packet to the Serial Wombat chip, don't wait for a response.
	
	This method sends 8 bytes.  Used for resetting the chip prior to bootloading
	
	\param tx address of an array of 8 bytes to send
	\return returns a non-error 0 or higher
*/

	int sendPacketNoResponse(uint8_t tx[]);

/*!
	\brief Request version string (combined model and firmware) and return pointer to it
	
	This queries the Serial Wombat chip for the 7 characters:   product line (1 character)
	Model (3 characters) and firmware version (3 characters)
	This is stored in a string in the Serial Wombat object.  A pointer to this string
	is returned.
*/
	char* readVersion(void) 
	{
		uint8_t tx[] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		memcpy(version, &rx[1], 7);
		version[7] = '\0';
		memcpy(model, &rx[1], 3);
		model[3] = '\0';
		fwVersion[0] = rx[5];
		fwVersion[1] = rx[6];
		fwVersion[2] = rx[7];
		return (version);
	}

/*!
	\brief Request version as a uint32
	
	This queries the Serial Wombat chip for its version information, and returns the
	firmware version as a uint32 0x0XYZ where X,Y,and Z represent firmwre version X.Y.Z
*/
	uint32_t readVersion_uint32(void) 
	{
		readVersion();
		return (
			((uint32_t)fwVersion[0] - '0') * 100 +
			((uint32_t)fwVersion[1] - '0') * 10 +
		(uint32_t)fwVersion[2] - '0');
	}

	/*!
	@brief Check if the firmware is the latest version (or more precisely, if the firmware matches this verison of the Library)
	@return true if the firmware matches the library.  False if the firmware is newer or older than the library
	*/
	bool isLatestFirmware(void)
	{
		uint32_t v = readVersion_uint32();
		if (isSW18())
		{
			return (v == SW18AB_LATEST_FIRMWARE);
		}
		else if (isSW08())
		{
			return (v == SW08B_LATEST_FIRMWARE);
		}
		else
		{
			return (v == SW4B_LATEST_FIRMWARE);

		}
	}

/*!
	\brief Read the 16 Bit public data associated with a Serial Wombat Pin Mode 
	
	Reads and returns the 16 bit value associated with a Serial Wombat Pin Mode.
	Additionally, values of 65 and higher have special meanings.  See
	Serial Wombat firmware documentation for details.
	\return 16 bit public data for pin specified
	\param pin The pin (or special meaning value) for which to retreive data
*/
	uint16_t readPublicData(uint8_t pin)
	{
		uint8_t tx[] = { 0x81,pin,255,255,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		return(rx[2] + (uint16_t)rx[3] * 256);
	}

/*!
	\brief Read the 16 Bit public data associated with a Serial Wombat Pin Mode 
	
	Reads and returns the 16 bit value associated with a Serial Wombat Pin Mode.
	Additionally, values of 65 and higher have special meanings.  See
	Serial Wombat firmware documentation for details.
	\return 16 bit public data for pin specified
	\param dataSource The pin (or special meaning value) for which to retreive data
*/
	uint16_t readPublicData(SerialWombatDataSource dataSource)
	{
		return (readPublicData((uint8_t)dataSource));
	}

/*!
	\brief Write a 16 bit value to a Serial Wombat pin Mode
	\param pin The pin number to which to write
	\param value The 16 bit value to write
*/
	uint16_t writePublicData(uint8_t pin, uint16_t value)
	{
		uint8_t tx[] = { 0x82,pin,(uint8_t)(value & 0xFF),(uint8_t)(value >> 8) ,255,0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		return (rx[2] + rx[3] * 256);
	}

	uint32_t comparePublicDataToThreshold(uint16_t threshold = 0)
	{
		uint8_t tx[] = { (uint8_t) SerialWombatCommands::COMMAND_BINARY_PIN_POLL_THRESHOLD,SW_LE16(threshold) ,0x55, 0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		return (ARRAY_UINT32(rx,1));
	}

/*!
	\brief Measure the Serial Wombat chip's Supply voltage
	
	Causes the Serial Wombat chip to measure the counts for the 
	internal reference voltage.  The Arduino library
	then converts these counts to a Source votlage in mV
	
	\return The Serial Wombat chip's source voltage in mV
*/

	uint16_t readSupplyVoltage_mV(void) 
	{
		if (isSW18() || isSW08())
		{
			_supplyVoltagemV = readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_VCC_mVOLTS);
		}
		else
		{
			int32_t counts = readPublicData(66); // Get FVR counts (1.024 v)
			if (counts > 0)
			{
				uint32_t mv = 1024 * 65536 / counts;
				_supplyVoltagemV = (uint16_t)mv;
			}
			else
			{
				_supplyVoltagemV = 0;
			}
		}
		return(_supplyVoltagemV);
	}


/*!
	\brief Measure the Serial Wombat chip's internal temperature
	
	This command is only supported by the SerialWombat 18 Series.
	The Arduino library will return 25 deg. C for other models
	
	This value is low accuracy unless a calibration has been performed
	
	\return The Serial Wombat chip's temperature in 100ths deg C
*/
	int16_t readTemperature_100thsDegC(void)
	{
		if (isSW18())
		{
			int32_t result = readPublicData(70);
			if (result >= 32768)
			{
				result = result - 65536;
			}
			return ((int16_t)result);
		}
		else
		{
			return 2500;
		}
	}

/*!
	\brief Send a reset command to the Serial Wombat chip
	
	Sends a reset command to the Serial Wombat chip.  The calling function
	should wait 500mS before sending additional commands.
*/
	void hardwareReset() 
	{
		uint8_t tx[9] = "ReSeT!#*";
		sendPacketNoResponse(tx);
	}

/*!
	\brief Set a pin to INPUT or OUTPUT, with options for pull Ups and open Drain settings
	
	\param pin The Serial Wombat pin to set
	\param mode Valid values are INPUT, OUTPUT or INPUT_PULLUP as defined by arduino.  Do not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
	\param pullDown  If True, a weak pull down will be enabled on this pin (No effect on SW4A/SW4B)
	\param openDrain If True, output becomes openDrain output rather than push / pull
*/
	void pinMode(uint8_t pin, uint8_t mode, bool pullDown = false, bool openDrain = false) 
	{
		if (pin >= WOMBAT_MAXIMUM_PINS)
		{
			return;
		}
		_pullDown[pin] = pullDown;
		_openDrain[pin] = openDrain;
		_pinmode[pin] = mode;
		configureDigitalPin(pin, mode);
	}

/*!
	\brief Set an output pin High or Low
	
	Before calling this function, the pin should be configured as an input or output with pinMode()
	\param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	\param val  Valid values are HIGH or LOW not use SW_INPUT, SW_HIGH or SW_LOW here, as these have different meanings
*/
	void digitalWrite(uint8_t pin, uint8_t val)
	{
		configureDigitalPin(pin, val);
	}

/*!
	@brief Reads the state of a Pin
	
	@return Returns LOW if pin is low or public data is 0.  Returns HIGH if pin is high or public data is > 0
*/
	int digitalRead(uint8_t pin) 
	{
		if (readPublicData(pin) > 0)
		{
			return (HIGH);
		}
		else
		{
			return (LOW);
		}
	}

/*!
	\brief Configures pin as analog input and does an immediate A/D conversion.  
	
	This function is compatible with the Arduino Uno analogRead function.  
	It does not make use of advanced Serial Wombat chip's functionality such as averaging and
	filtering.  Consider declaring a SerialWombatAnalogInput instead.
	\param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	\return An Analog to Digital conversion ranging from 0 to 1023 (10-bit)
*/
	int analogRead(uint8_t pin)
	{
		uint8_t tx[] = { 200,pin,PIN_MODE_ANALOGINPUT,0,0,0,0,0 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		return (readPublicData(pin) >> 6); // Scale from 16 bit value to 10 bit value.
	}

/*!
	\brief Set a pin to PWM output
	
	This function is compatible with the Arduino Uno analogWrite function, but will
	output a PWM with a different frequency.
	Consider declaring a SerialWombatPWM instead.  It has higher resolution and
	the ability to choose frequency.
	
	\param pin The Serial Wombat pin to set.  Valid values for SW4A: 0-3  SW4B: 1-3
	\param val A value from 0 (always low) to 255(always high) for the PWM duty cycle
*/
	void analogWrite(uint8_t pin, int val)
	{
		uint8_t dutyCycleLow = 0;
		if (val == 255)
		{
			dutyCycleLow = 255;
		}
		uint8_t tx[] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,pin,PIN_MODE_PWM,pin,dutyCycleLow,(uint8_t) val,false,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
	}

/*!
	\brief Send a version request to the Serial Wombat chip
	
	This function queries the Serial Wombat chip for its model and version
	and stores the result in the public members model and fwVersion
	as zero terminated strings.  Returns true if the response is
	likely a proper version response and false otherwise.
	
	\return TRUE if response was likely a valid version, FALSE otherwise
*/
	bool queryVersion()
	{
		uint8_t tx[8] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		if (rx[0] == 'V' && (rx[1] == 'S' || rx[1] == 'B'))
		{
			model[0] = rx[1];
			model[1] = rx[2];
			model[2] = rx[3];
			model[3] = 0;
			fwVersion[0] = rx[5];
			fwVersion[1] = rx[6];
			fwVersion[2] = rx[7];
			fwVersion[3] = 0;

			inBoot = (rx[1] == 'B');
			return (true);
		}
		return (false);
	}

/*!
	\brief Get the number of 1mS frames that have been executed since Serial Wombat chip reset
	
	This value should be roughly equal to the mS since reset.  It will vary based on the Serial Wombat chip's
	internal oscillator variation, and may run slow if Overflow frames are occuring.
*/
	uint32_t readFramesExecuted()
	{
		uint8_t tx[8] = { 0x81,67,68,0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		uint32_t returnval = rx[2] + (((uint32_t)rx[3]) << 8) + (((uint32_t)rx[4]) << 16) + (((uint32_t)rx[5]) << 24);
			return (returnval);
	}

/*!
	\brief Get the number of times an overflow Frame has occured.
	
	This value increments each time the Serial Wombat firmware determines it is time to start a new 1 mS frame,
	but the previous frame is still executing.  Indicates processor loading over 100% of real-time.  Overflows
	back to 0 when incremented from 65535.
*/

	uint16_t readOverflowFrames()
	{
		return readPublicData(69);
	}

/*!
	\brief Jump to Bootloader and wait for a UART download of new firmware
	
	This function causes a reset of the Serial Wombat chip and causes it to remain
	in the bootloader until a power-cycle occurs.  This allows loading new
	firmware via a UART connection to the bottom two pins (DIP pins 4 (RX) and 5(TX))
	on the SW4A/SW4B.  When jumping to boot the TX pin will go high.  All other 
	communication or functional pins will become inputs (i.e. PWMS, etc will stop).
*/
	void jumpToBoot()
	{
		uint8_t tx[] = "BoOtLoAd";
		sendPacket(tx);
	}

/*!
	\brief Read Address from RAM based on 16 bit address
	
	Most Arduino users should not need this command.
	
	This command can be used to read variables and registers within the Serial Wombat Chip
	Note that reading registers may have unintended side effects.  See the microcontroller datasheet
	for details.
	
	Note that Note that the PIC16F15214 used in the SW4A and SW4B chips
	is a Microchip Enhanced Mid-Range chip with both a banked RAM area and a Linear RAM area at an offset address.
	See the datasheet for details.  It's wierd to people who are unfamilliar with it.  The same location
	can have two different addresses.
	
	Addresses are not validated to be available in a given chip's address range.
	
	\param address  A 16-bit address pointing to a location in the Serial Wombat Chip's memory map
	
	\return An 8 bit value returned from the Serial Wombat chip.
*/
	uint8_t readRamAddress(uint16_t address)
	{
		uint8_t tx[8] = { 0xA0,SW_LE16(address),0x55,0x55,0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		return(rx[3]);
	}

/*!
	\brief Write byte to Address in RAM based on 16 bit address
	
	Most Arduino users should not need this command.
	
	This command can be used to write variables and registers within the Serial Wombat Chip
	Note that write registers may have unintended side effects.  See the microcontroller datasheet
	for details.
	
	Note that Note that the PIC16F15214 used in the SW4A and SW4B chips
	is a Microchip Enhanced Mid-Range chip with both a banked RAM area and a Linear RAM area at an offset address.
	See the datasheet for details.  It's wierd to people who are unfamilliar with it.  The same location
	can have two different addresses.
	
	Addresses are not validated to be available in a given chip's address range.
	
	\param address  A 16-bit address pointing to a location in the Serial Wombat Chip's memory map
	\param value An 8 bit value to be written to RAM
*/

	int16_t writeRamAddress(uint16_t address, uint8_t value)
	{
		uint8_t tx[8] = { 0xA3,SW_LE16(address),0,0,value,0x55,0x55};
		return sendPacket(tx);
	}

/*!
	\brief Read Address from Flash based on 32 bit address
	
	Most Arduino users should not need this command.
	
	This command can be used to read flash locations within the Serial Wombat Chip
	
	
	Addresses are not validated to be available in a given chip's address range.
	
	\param address  A 32-bit address pointing to a location in the Serial Wombat Chip's memory map
	
	\return An 32 bit value returned from the Serial Wombat chip.  32 bits are used to accomodate different chips.  The SW18 series has a 24 bit flash word, whereas the SW4A and SW4B have a 14 bit word.
*/

	uint32_t readFlashAddress(uint32_t address)
	{
		uint8_t tx[8] = { 0xA1,SW_LE32(address),0x55,0x55,0x55 };
		uint8_t rx[8];
		sendPacket(tx, rx);
		return(((uint32_t)rx[4]) + (((uint32_t)rx[5]) <<8) + (((uint32_t)rx[6]) <<16) + (((uint32_t)rx[7]) <<24));
	}

	/*!
	\brief Read data from the Serial Wombat 18AB's internal RAM buffer

	

	\param index  A 16-bit index into the Serial Wombat Chip's User RAM Buffer
	\param buffer a pointer to an array into which bytes will be written
	\param count Number of bytes to read

	\return Number of bytes read, or negative error code
*/
	 int16_t readUserBuffer(uint16_t index, uint8_t* buffer, uint16_t count)
	{
		uint16_t bytesRead = 0;
		while (bytesRead < count)
		{
			byte tx[] = {(byte)SerialWombatCommands::COMMAND_BINARY_READ_USER_BUFFER, (byte)(index & 0xFF), (byte)(index >> 8), 0x55, 0x55, 0x55, 0x55, 0x55};
			byte rx[8];
			int16_t result = sendPacket(tx,  rx);
			if (result >= 0)
			{
				for (int i = 1; i < 8; ++i)
				{
					buffer[bytesRead] = rx[i];
					++bytesRead;
					++index;
					if (bytesRead >= count)
					{
						break;
					}
				}
			}
			else
			{
				return (bytesRead);
			}
		}
		return (bytesRead);
	}
/*!
	\brief Shuts down most functions of the Serial Wombat chip reducing power consumption
	
	This command stops the Serial Wombat chip's internal clock, greatly reducing power consumption.
	The host is responsible for configuring outputs to a safe state prior to calling sleep.
	
	\warning This command does not cause any sort of shutdown routine to run.  The chip just stops.
	Outputs, including PWM, Servo and Protected Outputs, may retain their logic levels 
	at the moment the sleep command is processed.  In other words, they may stay high or low as long as the chip is in sleep.
*/
	void sleep()
	{
		uint8_t tx[8] = { 'S','l','E','e','P','!','#','*'};
		sendPacket(tx);
		_asleep = true;
	}

	/// \brief Called to send a dummy packet to the Serial Wombat chip to wake it from sleep and ready it for other commands
	void wake()
	{
		uint8_t tx[8] = { '!','!','!','!','!','!','!','!' };
		sendPacket(tx);
	}

	/// \brief Returns true if the instance received a model number corresponding to the Serial Wombat 18 series of chips at begin
	bool isSW04()
	{
		return ( model[1] == '0' && model[2] == '4');
	}


	/// \brief Returns true if the instance received a model number corresponding to the Serial Wombat 18 series of chips at begin
	bool isSW18()
	{
		return ( model[1] == '1' && model[2] == '8');
	}
	/// \brief Returns true if the instance received a model number corresponding to the Serial Wombat 08 series of chips at begin
	bool isSW08()
	{
		return ( model[1] == '0' && model[2] == '8');
	}

	/*!
	@brief Check if a specific pin mode is supported by the firmware in the Serial Wombat chip. (8B and 18AB only)
	@param pinMode The pin mode to check
	@return true if the pin mode is supported, false otherwise.
	*/
	bool isPinModeSupported(int pinMode)
	{
		return isPinModeSupported((SerialWombatPinMode_t)pinMode);
	}
	/*!
	@brief Check if a specific pin mode is supported by the firmware in the Serial Wombat chip. (8B and 18AB only)
	@param pinMode The pin mode to check
	@return true if the pin mode is supported, false otherwise.
	*/
	bool isPinModeSupported(SerialWombatPinMode_t pinMode)
	{
		if (isSW04())
		{
			switch (pinMode)
			{
				case PIN_MODE_DIGITALIO:
				case PIN_MODE_ANALOGINPUT:
				case PIN_MODE_CONTROLLED:
				case PIN_MODE_SERVO:
				case PIN_MODE_PWM:
				case PIN_MODE_DEBOUNCE:
				case PIN_MODE_QUADRATUREENCODER:
				case PIN_MODE_WATCHDOG:
				case PIN_MODE_PULSETIMER:
				case PIN_MODE_PROTECTED_OUTPUT:
					return true;

				default:
					return false;
			}
		}

		uint8_t tx[8] = {(uint8_t)SerialWombatCommands::CONFIGURE_CHANNEL_MODE_CHECK_MODE_SUPPORTED, 1, (uint8_t)pinMode, 0x55, 0x55, 0x55, 0x55, 0x55};
            int16_t returnVal = sendPacket(tx);
            returnVal *= -1;
            return (returnVal != SW_ERROR_UNKNOWN_PIN_MODE);
	}




	/// \brief Erases a page in flash.  Intended for use with the Bootloader, not by end users outside of bootloading sketch
	int16_t eraseFlashPage(uint32_t address)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_WRITE_FLASH,
			0, //Erase Page
			SW_LE32(address),
			0x55,0x55 };
		return sendPacket(tx);
	}


	/// \brief Writes a row in flash.  Intended for use with the Bootloader, not by end users outside of bootloading sketc
	int16_t writeFlashRow(uint32_t address)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_WRITE_FLASH,
			1, // Write entire row
			SW_LE32(address),0x55,0x55 };
		return sendPacket(tx);
	}




/*!
	\brief Set a pin to be a throughput monitoring pin. 
	
	This pin goes high when pin processing begins in each 1mS frame, and goes low
	after pin processing is complete.  This allows the CPU utilization of the Serial
	Wombat chip to be measured using a logic analyzer.  This function can only be applied
	to one pin, and is only disabled by resetting the chip.  This function is supported on
	the SW18AB chip.  It is not supported on the SW4 series of chips.
*/
	int16_t setThroughputPin(uint8_t pin)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,pin,PIN_MODE_FRAME_TIMER,0x55,0x55,0x55,0x55,0x55 };
		return sendPacket(tx);
	}

/*!
	\brief Set  this chip Address (SW8B only)
	
	Sets the i2c or other address of the chip.  This value is
	stored in flash and applies to both bootloader and application.
	Persistent across application bootloads.  Causes address selection
	pins to be ignored.  Set to 0xFF to restore address pins.
	Requires reset after calling to change I2C address.	

	\param address to set.  7 Bit value for I2C, defined as 32 bit for future applications like CAN
	\return Number of bytes written or error code.

*/
	int16_t setThroughputPin(uint32_t address)
	{
		uint8_t tx[8] = { (uint8_t)SerialWombatCommands::COMMAND_BINARY_SET_ADDRESS,SW_LE32(address),0x55,0x55,0x55 };
		return sendPacket(tx);
	}

/*!
	\brief Write bytes to the User Memory Buffer in the Serial Wombat chip
	\param index The index into the User Buffer array of bytes where the data should be loaded
	\param buffer a pointer to an array of bytes to be loaded into the User Buffer array
	\param count number of bytes to load
	\return Number of bytes written or error code.
*/
	int writeUserBuffer(uint16_t index, uint8_t* buffer, uint16_t count)
	{
		uint16_t bytesSent = 0;
		if (count == 0)
		{
			return 0;
		}

		{ // Send first packet of up to 4 bytes
			uint8_t bytesToSend = 4;
			if (count < 4)
			{
				bytesToSend = (uint8_t)count;
				count = 0;
			}
			else
			{
				count -= 4;
			}

			uint8_t tx[8] = { 0x84,SW_LE16(index), bytesToSend,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];

			uint8_t i;
			for (i = 0; i < bytesToSend; ++i)
			{
				tx[4 + i] = buffer[i];
			}
			int result = sendPacket(tx, rx);
			if (rx[0] == 'E')
			{
				return (result);
			}
			bytesSent = bytesToSend;
		}
		while (count >= 7)  // Continue sending
		{

			count -= 7;
			uint8_t tx[8] = { 0x85,0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
			uint8_t rx[8];
			uint8_t i;
			for (i = 0; i < 7; ++i)
			{
				tx[1 + i] = buffer[bytesSent + i];
			}
			int result = sendPacket(tx, rx);
			if (rx[0] == 'E')
			{
				return (result);
			}
			bytesSent += 7;
		}
		while (count > 0)
		{

			{ // Send first packet of up to 4 bytes
				uint8_t bytesToSend = 4;
				if (count < 4)
				{
					bytesToSend = (uint8_t)count;
					count = 0;
				}
				else
				{
					count -= 4;
				}

				uint8_t tx[8] = { 0x84,SW_LE16(index + bytesSent), bytesToSend,0x55,0x55,0x55,0x55 };
				uint8_t rx[8];

				uint8_t i;
				for (i = 0; i < bytesToSend; ++i)
				{
					tx[4 + i] = buffer[i + bytesSent];
				}
				int result = sendPacket(tx, rx);
				if (rx[0] == 'E')
				{
					return (result);
				}
				bytesSent += bytesToSend;
			}
		}
		return(bytesSent);
		

	}

/*!
	\brief Write bytes to the User Memory Buffer in the Serial Wombat chip
	\param index The index into the User Buffer array of bytes where the data should be loaded
	\param s string to convert to Ascii bytes and load
	\return Number of bytes written or error code.
*/
	int writeUserBuffer(uint16_t index, char* s)
	{
		return writeUserBuffer(index, (uint8_t*)s, (uint16_t)strlen(s));
	}

/*!
	\brief Write bytes to the User Memory Buffer in the Serial Wombat chip
	\param index The index into the User Buffer array of bytes where the data should be loaded
	\param s string to convert to Ascii bytes and load
	\return Number of bytes written or error code.
*/
	int writeUserBuffer(uint16_t index, const char s[])
	{
		return writeUserBuffer(index, (uint8_t*)s, (uint16_t)strlen(s));
	}



/*!
	\brief Set a pin to be a frame timer for system utilization (SW18AB Only)
	\return 0 or positive for success or negative error code
	
	This command configures a Serial Wombat 18AB Pin to be a frame timer.  This frame goes high at
	the beginning of pin processing, and low after all pins have been serviced.  The duty cycle of 
	this pin is an indicator of the Serial Wombat Chip's CPU utilization.  This pin has a frequency
	of 1kHz corresponding to the 1000 frames per second executive.  Most multimeters will filter this
	pin to a voltage, so the CPU utilization can be seen as a fraction of the system voltage.
	Only one pin can be the Frame Timer pin at a time.
*/
	int16_t writeFrameTimerPin(uint8_t pin)
{
	uint8_t tx[] = { 0xC8 ,pin,(uint8_t)PIN_MODE_FRAME_TIMER,0x55,0x55,0x55,0x55,0x55 };
	return sendPacket(tx);
}
	

/*!
	\brief Search the I2C Bus addresses 0x68 to 0x6F for I2C devices, and test to see if they respond to Serial Wombat version commands.  Returns first address that responds properly or 0 if none found
	
	\param keepTrying if True, go into a loop and do not exit until a Serial Wombat Chip is found
	
	\return I2C address of first found Seirla Wombat chip or 0 if none found
*/
	static uint8_t find(bool keepTrying = false)
	{
		do
		{
			for (int i2cAddress = 0x60; i2cAddress <= 0x6F; ++i2cAddress)
			{
				Wire.beginTransmission(i2cAddress);
				int error = Wire.endTransmission();


				if (error == 0)
				{
					uint8_t tx[8] = { 'V',0x55,0x55,0x55,0x55,0x55,0x55,0x55 };
					uint8_t rx[8];
					Wire.beginTransmission(i2cAddress);
					Wire.write(tx, 8);
					Wire.endTransmission();
					Wire.requestFrom((uint8_t)i2cAddress, (uint8_t)8);

					int count = 0;
					while (Wire.available() && count < 8)
					{
						rx[count] = Wire.read();
						++count;
					}
					if (count == 8)
					{
						if (rx[0] == 'V' && (rx[1] == 'S' || rx[1]=='B'))
						{
							return(i2cAddress); // Found one.
						}
					}
				}
			}
			delay(0);
		}while (keepTrying);
				return(0);  // Didn't find one.
	}


/*!
	\brief Returns the last Serial Wombat command that produced a protocol error
	
	\return Returns the error code associated with the command
	\param cmd pointer to a uint8_t [8] array into which the error command will be copied
*/
	int16_t readLastErrorCommand(uint8_t* cmd)
{

	uint8_t tx[8] = { (uint8_t)SerialWombatCommands::COMMAND_READ_LAST_ERROR_PACKET, 0,0x55,0x55,0x55,0x55,0x55,0x55 };
	uint8_t rx[8];
	if (sendPacket(tx, rx) >= 0)
	{
		for (int i = 1; i < 8; ++i)
		{
			cmd[i - 1] = rx[i];
		}
	}
	else
	{
		return (lastErrorCode);
	}
	tx[1] = 7;
	if (sendPacket(tx, rx) >= 0)
	{
		cmd[7] = rx[1];
	}
	return(lastErrorCode);
}

/*!
	@brief Registers an error handler that is called by the SerialWombatChip sendPacket() command when a protocol error is returned by the Serial Wombat
	@param handler A function pointer to a function of SerialWombatErrorHandler_t type.
*/
	void registerErrorHandler(SerialWombatErrorHandler_t handler)
	{
		errorHandler = handler;
	}

	/// \brief The I2C address of the SerialWombatChip instance
	uint8_t address = 0;

	///  \brief How many times to retry a packet if communcation bus (such as I2C) error
	uint8_t communicationErrorRetries = 5;

	/*!
	@brief Echo a byte array back to the host.  Used for testing and debugging
	@param data The byte array to echo back to the host
	@param count The number of bytes to echo.  Default is 7 bytes.  Maximum is 7 bytes.
	@return 0 for success or negative error code.  Success is successful transmit, not echo verification.
	*/
	int16_t echo(uint8_t data[], uint8_t count = 7)
	{
		uint8_t tx[] = "!UUUUUUU";
		for (int i = 0; i < 7 && i < count; ++i)
		{
			tx[i + 1] = (uint8_t)data[i];
		}
		return sendPacket(tx);
	}

	/*!
	@brief Echo a byte array back to the host.  Used for testing and debugging
	@param data The 7 byte array to echo back to the host
	@return 0 for success or negative error code.  Success is successful transmit, not echo verification.
	*/

	int16_t echo(char* data)
	{
		int length = strlen(data);
		uint8_t tx[] = "!UUUUUUU";
		for (int i = 0; i < 7 && i < length; ++i)
		{
			tx[i + 1] = (uint8_t)data[i];
		}
		return sendPacket(tx);
	}

	/*!
	@brief Read the birthday of the Serial Wombat 18AB chip
	\return The birthday as a 32 bit value, or 0 if not a SW18AB chip
	*/
	uint32_t readBirthday()
	{
		if (isSW18())
		{
			uint32_t birthday = (readFlashAddress(0x2A00C) >> 8) & 0xFF;
			birthday *= 100;
			birthday += (readFlashAddress(0x2A00C)) & 0xFF;
			birthday *= 100;
			birthday += readFlashAddress(0x2A00E) & 0xFF;
			birthday *= 100;
			birthday += readFlashAddress(0x2A010) & 0xFF;
			return (birthday);
		}
		return 0;
	}

	/*!
	@brief Read the brand of the Serial Wombat 18AB chip
	\return The brand as a string, or empty string if not a SW18AB chip
	*/
	int16_t readBrand(char* data)
	{
		uint8_t length = 0;
		if (isSW18())
		{
			for (int i = 0; i < 32; ++i)
			{
				uint32_t val = readFlashAddress(0x2A020 + i * 2) ;
				if ((val & 0xFF) != 0xFF)
				{
					data[i ] = (char)(val & 0xFF);
					++length;
				}		
				else
				{
					data[length] = 0;
					return (length);
				}
			}
			data[length] = 0;
			return (length);
		}

		data[0] = 0;
		return 0 ;
	}

};

/*! 
	@brief A class which tunes the oscillator on a Serial Wombat 18AB chip
	
	This class is designed to be called periodically in the program main loop.  It compares
	the 1mS execution frame count to the millis() funciton provided by the host.  When
	at least 10 seconds of execution have occured the class compares the counts and
	issues a command to tune the Serial Wombat Chip's oscillator slightly slower or faster.
	This can reduce the error in the Serial Wombat's 32MHz nominal clock to less than +/- 0.1%
	vs. the +/- 1.5% limit in the datasheet.   Simply call update() periodically and the class
	will take care of the rest.  Allow up to 10 calls at least 10 seconds apart each to reach
	optimal timing.
	See the example sketch for an example.
	*/
class SerialWombat18ABOscillatorTuner
{
private:
	SerialWombatChip& _sw;
	uint32_t lastMillis = 0;
	uint32_t lastFrames = 0;
public:
	/*!
	@brief Class constructor for SerialWombat18OscillatorTuner
	@param serialWombatChip The Serial Wombat chip on which the Oscillator will be tuned;
	*/
	SerialWombat18ABOscillatorTuner(SerialWombatChip& serialWombatChip) : _sw(serialWombatChip) { }

	/*!
	@brief   Call periodically to tune the SW18AB oscillator to reported millis
	*/
	void update() {
		uint32_t m = millis();
		if (lastMillis == 0)
		{
			lastMillis = m;
			uint32_t frames = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
			uint16_t frameslsb = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
			if (frames != _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW))
			{
				frameslsb = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
				frames = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
			}
			frames <<= 16;
			frames += frameslsb;
			lastFrames = frames;

		}
		else if ((m - lastMillis) < 10000)
		{
			//Do nothing
		}
		else if (m < lastMillis)
		{
			//Has it been 47 days already?
			lastMillis = 0;
		}
		else
		{
			uint32_t diff = m - lastMillis;

			uint32_t frames = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
			uint16_t frameslsb = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
			
			if (frames != _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW))
			{
				frameslsb = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW);
				frames = _sw.readPublicData(SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_MSW);
			}
			frames <<= 16;
			frames += frameslsb;
			uint32_t framesDif = frames - lastFrames;

			if (diff > framesDif )
			{
				// Running  slow
				uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_ADJUST_FREQUENCY,
					SW_LE16(1),//Counts to increment
					SW_LE16(0), // Counts to decrement
					0x55,0x55,0x55};
				_sw.sendPacket(tx);
			}
			else if (diff < framesDif)
			{
				// Running mfast
				uint8_t tx[] = { (uint8_t)SerialWombatCommands::COMMAND_ADJUST_FREQUENCY,
					SW_LE16(0),//Counts to increment
					SW_LE16(1), // Counts to decrement
					0x55,0x55,0x55 };
				_sw.sendPacket(tx);

			}

			lastMillis = m;
			lastFrames = frames;

		}
		
	}
	
};

/*
End of cross platform code synchronization.  Random string to help the compare tool sync lines:
asdkj38vjn1nasdnvuwlamafdjiivnowalskive
*/

/// \brief This class name is depricated.  Do not use for new development.  Use SerialWombatChip instead.
class SerialWombat : public SerialWombatChip {};  





/*!
@brief a sample error handler that can be registered with registerErrorHandler to report protocol errors on Serial.  

Do not use this when using Serial to control the Serial Wombat Chip
@param error The error number that was reported
@param sw to the Serial Wombat Chip instance that generated the error
*/
void SerialWombatSerialErrorHandlerBrief(uint16_t error, SerialWombatChip* sw);

/*!
@brief a sample error handler that can be registered with registerErrorHandler to report protocol errors on Serial.  

Do not use this when using Serial to control the Serial Wombat Chip
@param error The error number that was reported
@param sw to the Serial Wombat Chip instance that generated the error
*/
void SerialWombatSerialErrorHandlerVerbose(uint16_t error, SerialWombatChip* sw);

#include "SerialWombatPin.h"
#include "SerialWombatQueue.h"
#include "SerialWombat18ABDataLogger.h"
#include "SerialWombatAbstractButton.h"
#include "SerialWombatAbstractProcessedInput.h"
#include "SerialWombatAbstractScaledOutput.h"
#include "SerialWombatBlink.h"
#include "SerialWombat18CapTouch.h"
#include "SerialWombat18ABVGA.h"
#include "SerialWombatAnalogInput.h"
#include "SerialWombatDebouncedInput.h"
#include "SerialWombatFrequencyOutput.h"
#include "SerialWombatHBridge.h"
#include "SerialWombatHSClock.h"
#include "SerialWombatHSCounter.h"
#include "SerialWombatIRRx.h"
#include "SerialWombatLiquidCrystal.h"
#include "SerialWombatMatrixKeypad.h"
#include "SerialWombatProcessedInputPin.h"
#include "SerialWombatProtectedOutput.h"
#include "SerialWombatPS2Keyboard.h"
#include "SerialWombatPulseOnChange.h"
#include "SerialWombatPulseTimer.h"
#include "SerialWombatPWM.h"
#include "SerialWombatQuadEnc.h"
#include "SerialWombatQueuedPulseOutput.h"
#include "SerialWombatResistanceInput.h"
#include "SerialWombatServo.h"
#include "SerialWombatTM1637.h"
#include "SerialWombatUART.h"
#include "SerialWombatUltrasonicDistanceSensor.h"
#include "SerialWombatWatchdog.h"
#include "SerialWombatWS2812.h"
#include "SerialWombatThroughputConsumer.h"
#include "PCB0030_Bridge.h"
#include "PCB0031_Grip.h"

#endif
