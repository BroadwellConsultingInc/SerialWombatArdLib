#pragma once
/*
Copyright 2026 Broadwell Consulting Inc.

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
#include "SerialWombat.h"

/*! \file SerialWombatCharlieplex.h
*/

/*!
\brief A class representing a Serial Wombat Charlieplexed LED display.

The Charlieplex pin mode uses between 2 and 8 Serial Wombat pins to control
up to N * (N - 1) LEDs, for a maximum of 56 LEDs with 8 pins.  One LED is
driven during each approximately 1 ms Serial Wombat execution frame.

The first pin in the pins array is the controlling pin.  Its public data is
used by display modes that consume public data.  The second pin is also used
by the firmware to store the customizable 56-byte logical LED lookup table.

Each lookup-table byte contains two logical pin indexes:
 - Bits 7:4: logical pin index driven high
 - Bits 3:0: logical pin index driven low

The class inherits SerialWombatAbstractScaledOutput.  Scaled-output functions
are useful with DISPLAY_MODE_SCALED_SINGLE_LED and
DISPLAY_MODE_SCALED_BARGRAPH.
*/
class SerialWombatCharlieplex : public SerialWombatPin, public SerialWombatAbstractScaledOutput
{
public:
    enum displayMode_t
    {
        DISPLAY_MODE_BITMAP = 0,              ///< Seven-byte protocol bitmap controls up to 56 LEDs.
        DISPLAY_MODE_PUBLIC_DATA_BITMAP = 1,  ///< Controlling pin public data controls LEDs 0 through 15 as a bitfield.
        DISPLAY_MODE_SCALED_SINGLE_LED = 2,   ///< Scaled-output value selects one logical LED index.
        DISPLAY_MODE_SCALED_BARGRAPH = 3      ///< Scaled-output value lights LEDs 0 through the selected index.
    };

    static const uint8_t MAX_PINS = 8;
    static const uint8_t MAX_LEDS = 56;
    static const uint8_t BITMAP_BYTES = 7;
    static const uint8_t UNUSED_PIN = 0xFF;
    static const uint8_t UNUSED_LED = 0xFF;

    /*!
    \brief Constructor for the SerialWombatCharlieplex class.
    \param serialWombat Serial Wombat chip on which the pin mode will run.
    */
    SerialWombatCharlieplex(SerialWombatChip& serialWombat) :
        SerialWombatPin(serialWombat),
        SerialWombatAbstractScaledOutput(serialWombat)
    {
    }

    /*!
    \brief Initialize a Charlieplexed LED display.

    \param pin0 First physical Serial Wombat pin.  This is the controlling pin.
    \param pin1 Second physical Serial Wombat pin.  The firmware uses this pin's
    pin memory to store the customizable logical LED lookup table.
    \param pin2 Third physical Serial Wombat pin, or UNUSED_PIN.
    \param pin3 Fourth physical Serial Wombat pin, or UNUSED_PIN.
    \param pin4 Fifth physical Serial Wombat pin, or UNUSED_PIN.
    \param pin5 Sixth physical Serial Wombat pin, or UNUSED_PIN.  Defaults to
    UNUSED_PIN.
    \param pin6 Seventh physical Serial Wombat pin, or UNUSED_PIN.  Defaults to
    UNUSED_PIN.
    \param pin7 Eighth physical Serial Wombat pin, or UNUSED_PIN.  Defaults to
    UNUSED_PIN.
    \param displayMode Selects bitmap, public-data bitmap, single-LED, or
    bargraph operation.
    \param scanPeriod_mS Minimum number of approximately 1 ms execution frames
    between the starts of consecutive scans.  Zero adds no blank padding.
    The documented useful range is 0 through 56.
    \return 0 or a positive value if successful, otherwise a negative error
    code.

    The number of Charlieplex pins is calculated by counting pin parameters
    from pin0 through the first UNUSED_PIN value.  Pin parameters after the
    first UNUSED_PIN are ignored by the firmware.
    */
    int16_t begin(uint8_t pin0,
        uint8_t pin1,
        uint8_t pin2,
        uint8_t pin3,
        uint8_t pin4,
        uint8_t pin5 = UNUSED_PIN,
        uint8_t pin6 = UNUSED_PIN,
        uint8_t pin7 = UNUSED_PIN,
        displayMode_t displayMode = DISPLAY_MODE_BITMAP,
        uint8_t scanPeriod_mS = 0)
    {
        const uint8_t pins[MAX_PINS] = {
            pin0,
            pin1,
            pin2,
            pin3,
            pin4,
            pin5,
            pin6,
            pin7
        };

        uint8_t numberOfPins = MAX_PINS;
        for (uint8_t i = 0; i < MAX_PINS; ++i)
        {
            if (pins[i] == UNUSED_PIN)
            {
                numberOfPins = i;
                break;
            }
        }

        _pin = pin0;
        _pinMode = (uint8_t)PIN_MODE_CHARLIEPLEX;

        uint8_t tx0[] = {
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE0,
            _pin,
            _pinMode,
            numberOfPins,
            (uint8_t)displayMode,
            scanPeriod_mS,
            pin1,
            pin2
        };

        int16_t result = _sw.sendPacket(tx0);
        if (result < 0)
        {
            return result;
        }

        uint8_t tx1[] = {
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE1,
            _pin,
            _pinMode,
            pin3,
            pin4,
            pin5,
            pin6,
            pin7
        };

        return _sw.sendPacket(tx1);
    }

    /*!
    \brief Write the complete LED bitmap from an array of bytes.

    \param ledArray Array of seven bytes in little-endian bit order.  Bit 0 of
    byte 0 controls logical LED 0; bit 7 of byte 6 controls logical LED 55.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t writeLEDArray(const uint8_t ledArray[BITMAP_BYTES])
    {
        uint8_t tx2[] = {
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE2,
            _pin,
            _pinMode,
            ledArray[0],
            ledArray[1],
            ledArray[2],
            ledArray[3],
            ledArray[4]
        };

        int16_t result = _sw.sendPacket(tx2);
        if (result < 0)
        {
            return result;
        }

        uint8_t tx3[] = {
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE3,
            _pin,
            _pinMode,
            ledArray[5],
            ledArray[6],
            0x55,
            0x55,
            0x55
        };

        return _sw.sendPacket(tx3);
    }

    /*!
    \brief Set up to five logical LEDs in bitmap display mode.

    Each argument is a logical LED index from 0 through 55.  Use UNUSED_LED
    for an unused argument.  The firmware modifies its LED bitmap directly;
    no host-side bitmap copy is maintained.

    \param led0 First logical LED index to set.
    \param led1 Second logical LED index, or UNUSED_LED.
    \param led2 Third logical LED index, or UNUSED_LED.
    \param led3 Fourth logical LED index, or UNUSED_LED.
    \param led4 Fifth logical LED index, or UNUSED_LED.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t setLEDs(uint8_t led0,
        uint8_t led1 = UNUSED_LED,
        uint8_t led2 = UNUSED_LED,
        uint8_t led3 = UNUSED_LED,
        uint8_t led4 = UNUSED_LED)
    {
        return writeLEDCommand(
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE5,
            led0, led1, led2, led3, led4);
    }

    /*!
    \brief Set up to five logical LEDs from an array.

    \param ledIndexes Array containing count logical LED indexes.
    \param count Number of entries to send.  Valid range is 0 through 5.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t setLEDs(const uint8_t ledIndexes[], uint8_t count)
    {
        return writeLEDCommand(
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE5,
            ledIndexes,
            count);
    }

    /*!
    \brief Clear up to five logical LEDs in bitmap display mode.

    Each argument is a logical LED index from 0 through 55.  Use UNUSED_LED
    for an unused argument.  The firmware modifies its LED bitmap directly;
    no host-side bitmap copy is maintained.

    \param led0 First logical LED index to clear.
    \param led1 Second logical LED index, or UNUSED_LED.
    \param led2 Third logical LED index, or UNUSED_LED.
    \param led3 Fourth logical LED index, or UNUSED_LED.
    \param led4 Fifth logical LED index, or UNUSED_LED.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t clearLEDs(uint8_t led0,
        uint8_t led1 = UNUSED_LED,
        uint8_t led2 = UNUSED_LED,
        uint8_t led3 = UNUSED_LED,
        uint8_t led4 = UNUSED_LED)
    {
        return writeLEDCommand(
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
            led0, led1, led2, led3, led4);
    }

    /*!
    \brief Clear up to five logical LEDs from an array.

    \param ledIndexes Array containing count logical LED indexes.
    \param count Number of entries to send.  Valid range is 0 through 5.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t clearLEDs(const uint8_t ledIndexes[], uint8_t count)
    {
        return writeLEDCommand(
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE6,
            ledIndexes,
            count);
    }

    /*!
    \brief Turn off all LEDs in bitmap display mode.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t clearLEDs()
    {
        uint8_t ledArray[BITMAP_BYTES] = {0, 0, 0, 0, 0, 0, 0};
        return writeLEDArray(ledArray);
    }

    /*!
    \brief Write four consecutive logical LED lookup entries.

    Each entry is encoded as high logical pin index in bits 7:4 and low logical
    pin index in bits 3:0.  The firmware writes four entries per command.

    \param firstLEDIndex Logical LED index for entry0.
    \param entry0 Encoded mapping for firstLEDIndex.
    \param entry1 Encoded mapping for firstLEDIndex + 1.
    \param entry2 Encoded mapping for firstLEDIndex + 2.
    \param entry3 Encoded mapping for firstLEDIndex + 3.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t writeLookupEntries(uint8_t firstLEDIndex,
        uint8_t entry0,
        uint8_t entry1,
        uint8_t entry2,
        uint8_t entry3)
    {
        uint8_t tx[] = {
            (uint8_t)SerialWombatCommands::CONFIGURE_PIN_MODE4,
            _pin,
            _pinMode,
            firstLEDIndex,
            entry0,
            entry1,
            entry2,
            entry3
        };
        return _sw.sendPacket(tx);
    }

    /*!
    \brief Write all 56 logical LED lookup entries.

    \param lookupTable Array of 56 encoded high/low logical pin pairs.
    \return 0 or a positive value if successful, otherwise a negative error
    code.
    */
    int16_t writeLookupTable(const uint8_t lookupTable[MAX_LEDS])
    {
        for (uint8_t firstLEDIndex = 0;
            firstLEDIndex < MAX_LEDS;
            firstLEDIndex += 4)
        {
            int16_t result = writeLookupEntries(
                firstLEDIndex,
                lookupTable[firstLEDIndex],
                lookupTable[firstLEDIndex + 1],
                lookupTable[firstLEDIndex + 2],
                lookupTable[firstLEDIndex + 3]);

            if (result < 0)
            {
                return result;
            }
        }
        return 0;
    }

    /*!
    \brief Encode a logical high pin and logical low pin into one lookup byte.
    \param highPinIndex Logical Charlieplex pin index driven high.
    \param lowPinIndex Logical Charlieplex pin index driven low.
    \return Encoded lookup-table byte.
    */
    static uint8_t encodePinPair(uint8_t highPinIndex, uint8_t lowPinIndex)
    {
        return (uint8_t)((highPinIndex << 4) | (lowPinIndex & 0x0F));
    }

    /*!
    \brief Fulfills a virtual function requirement of SerialWombatAbstractScaledOutput.
    \return Current controlling pin number.
    */
    uint8_t pin()
    {
        return SerialWombatPin::_pin;
    }

    /*!
    \brief Fulfills a virtual function requirement of SerialWombatAbstractScaledOutput.
    \return Current pin mode number.
    */
    uint8_t swPinModeNumber()
    {
        return SerialWombatPin::_pinMode;
    }

private:
    int16_t writeLEDCommand(uint8_t command,
        uint8_t led0,
        uint8_t led1,
        uint8_t led2,
        uint8_t led3,
        uint8_t led4)
    {
        const uint8_t ledIndexes[5] = {led0, led1, led2, led3, led4};

        for (uint8_t i = 0; i < 5; ++i)
        {
            if (ledIndexes[i] != UNUSED_LED && ledIndexes[i] >= MAX_LEDS)
            {
                return -1;
            }
        }

        uint8_t tx[] = {
            command,
            _pin,
            _pinMode,
            led0,
            led1,
            led2,
            led3,
            led4
        };

        return _sw.sendPacket(tx);
    }

    int16_t writeLEDCommand(uint8_t command,
        const uint8_t ledIndexes[],
        uint8_t count)
    {
        if (count > 5)
        {
            return -1;
        }

        uint8_t leds[5] = {
            UNUSED_LED,
            UNUSED_LED,
            UNUSED_LED,
            UNUSED_LED,
            UNUSED_LED
        };

        for (uint8_t i = 0; i < count; ++i)
        {
            if (ledIndexes[i] >= MAX_LEDS)
            {
                return -1;
            }
            leds[i] = ledIndexes[i];
        }

        return writeLEDCommand(
            command,
            leds[0],
            leds[1],
            leds[2],
            leds[3],
            leds[4]);
    }

};
