/*  This example shows how to use the Serial Wombat Charlieplex pin mode
 *   to display bitmap patterns on 56 LEDs.
 *
 *   This example is compatible with the Serial Wombat 18AB and 8B chips,
 *   when the Charlieplex pin mode is present in the firmware build.
 *
 *   Eight Serial Wombat pins are used to control up to 56 Charlieplexed
 *   LEDs.  The pins are assigned sequentially from pin 0 through pin 7.
 *
 *   The LED bitmap is stored in a seven-byte array.  Bit 0 of byte 0 controls
 *   logical LED 0, and bit 7 of byte 6 controls logical LED 55.
 *
 *   SerialWombatCharlieplex::writeLEDArray() sends the bitmap using the
 *   CONFIGURE_CHANNEL_MODE_2 and CONFIGURE_CHANNEL_MODE_3 commands.  These
 *   commands have command bytes 0xCA and 0xCB.
 *
 *   A new pattern is written every 200 ms.
 *
 *   Each LED must have an appropriate current-limiting resistor.  The LED
 *   wiring direction determines which logical LED index controls it.
 *
 *   SerialWombatCharlieplex pin mode documentation:
 *
 *   TODO coming soon
 *
 *   SerialWombatCharlieplex tutorial video:
 *
 *   TODO coming soon
 */

#include <SerialWombat.h>
#include <SerialWombatCharlieplex.h>

SerialWombatChip sw;
SerialWombatCharlieplex charlieplex(sw);

#define CHARLIEPLEX_PIN_0 0
#define CHARLIEPLEX_PIN_1 (CHARLIEPLEX_PIN_0 + 1)
#define CHARLIEPLEX_PIN_2 (CHARLIEPLEX_PIN_1 + 1)
#define CHARLIEPLEX_PIN_3 (CHARLIEPLEX_PIN_2 + 1)
#define CHARLIEPLEX_PIN_4 (CHARLIEPLEX_PIN_3 + 1)
#define CHARLIEPLEX_PIN_5 (CHARLIEPLEX_PIN_4 + 1)
#define CHARLIEPLEX_PIN_6 (CHARLIEPLEX_PIN_5 + 1)
#define CHARLIEPLEX_PIN_7 (CHARLIEPLEX_PIN_6 + 1)

#define PATTERN_DELAY_MS 200


const uint8_t ledPatterns[][SerialWombatCharlieplex::BITMAP_BYTES] =
{
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // All LEDs off
  {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, // Even-numbered LEDs
  {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, // Odd-numbered LEDs
  {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33}, // Two LEDs on, two LEDs off
  {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC}, // Inverse two-on, two-off pattern
  {0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00}, // LEDs 0 through 27
  {0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF}, // LEDs 28 through 55
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // All 56 LEDs on
};

#define NUMBER_OF_PATTERNS (sizeof(ledPatterns) / sizeof(ledPatterns[0]))

uint8_t currentPattern = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Charlieplex Example 1 - Bitmap Patterns");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (sw.isSW04())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_CHARLIEPLEX)))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  // A 56 ms minimum scan period provides consistent per-LED duty cycle as the
  // number of illuminated LEDs changes from one pattern to another.
  charlieplex.begin(CHARLIEPLEX_PIN_0,
                    CHARLIEPLEX_PIN_1,
                    CHARLIEPLEX_PIN_2,
                    CHARLIEPLEX_PIN_3,
                    CHARLIEPLEX_PIN_4,
                    CHARLIEPLEX_PIN_5,
                    CHARLIEPLEX_PIN_6,
                    CHARLIEPLEX_PIN_7,
                    SerialWombatCharlieplex::DISPLAY_MODE_BITMAP,
                    56);

  charlieplex.writeLEDArray(ledPatterns[currentPattern]); // Sends 0xCA and 0xCB commands
  Serial.println("Displaying a new 56-LED bitmap pattern every 200 ms.");
}

void loop() {
  delay(PATTERN_DELAY_MS);

  ++currentPattern;
  if (currentPattern >= NUMBER_OF_PATTERNS)
  {
    currentPattern = 0;
  }

  charlieplex.writeLEDArray(ledPatterns[currentPattern]); // Sends 0xCA and 0xCB commands
}
