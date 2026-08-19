/*  This example shows how to customize the Serial Wombat Charlieplex logical
 *   LED lookup table.  By customizing this table you can change what is the 
 *   1st, 2nd, 3rd, etc LED in the sequence.  This can be highly advantageous
 *   in allowing convenient PCB layout, while abstracting the LEDs into a
 *   logical order.
 *
 *   This example is compatible with the Serial Wombat 18AB and 8B chips,
 *   when the Charlieplex pin mode is present in the firmware build.
 *
 *   Eight Serial Wombat pins are used to control up to 56 Charlieplexed
 *   LEDs.  The pins are assigned sequentially from pin 0 through pin 7.
 *
 *   Each lookup entry stores the logical high pin in the upper nibble and the
 *   logical low pin in the lower nibble.  This example changes the lookup
 *   order to:
 *
 *   0x01,
 *   0x02, 0x12,
 *   0x03, 0x13, 0x23,
 *   ...
 *   0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67,
 *
 *   followed by the LEDs in the reverse electrical direction:
 *
 *   0x10,
 *   0x20, 0x21,
 *   0x30, 0x31, 0x32,
 *   ...
 *   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76.
 *
 *   Note that the entries in this table are an index of the pin numbers provided
 *   in the begin() call, not explicit Serial Wombat pin numbers.
 *   
 *   SerialWombatCharlieplex::writeLookupTable() writes four entries per
 *   CONFIGURE_CHANNEL_MODE_4 packet, command byte 0xCC.
 *
 *   After configuring the table, one logical LED moves through all 56 lookup
 *   entries so the new physical order can be observed.
 *
 *   Each LED must have an appropriate current-limiting resistor.
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

#define NUMBER_OF_LEDS 56
#define LED_MOVE_DELAY_MS 200


uint8_t ledLookupTable[SerialWombatCharlieplex::MAX_LEDS] =
{
  // Forward electrical direction
  0x01,
  0x02, 0x12,
  0x03, 0x13, 0x23,
  0x04, 0x14, 0x24, 0x34,
  0x05, 0x15, 0x25, 0x35, 0x45,
  0x06, 0x16, 0x26, 0x36, 0x46, 0x56,
  0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67,

  // Reverse electrical direction
  0x10,
  0x20, 0x21,
  0x30, 0x31, 0x32,
  0x40, 0x41, 0x42, 0x43,
  0x50, 0x51, 0x52, 0x53, 0x54,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76
};

uint8_t currentLED = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Charlieplex Example 3 - Custom Lookup Order");


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

  charlieplex.begin(CHARLIEPLEX_PIN_0,
                    CHARLIEPLEX_PIN_1,
                    CHARLIEPLEX_PIN_2,
                    CHARLIEPLEX_PIN_3,
                    CHARLIEPLEX_PIN_4,
                    CHARLIEPLEX_PIN_5,
                    CHARLIEPLEX_PIN_6,
                    CHARLIEPLEX_PIN_7,
                    SerialWombatCharlieplex::DISPLAY_MODE_BITMAP,
                    1);

  // Write the initialized RAM lookup array to the Charlieplex pin mode.
  // writeLookupTable() sends fourteen 0xCC packets with four entries each.
  charlieplex.writeLookupTable(ledLookupTable);

  charlieplex.clearLEDs();
  charlieplex.setLEDs(currentLED);

  Serial.println("The custom 56-entry lookup table has been written.");
  Serial.println("A single LED is moving through the reordered logical LED sequence.");
}

void loop() {
  delay(LED_MOVE_DELAY_MS);

  charlieplex.clearLEDs(currentLED);

  ++currentLED;
  if (currentLED >= NUMBER_OF_LEDS)
  {
    currentLED = 0;
  }

  charlieplex.setLEDs(currentLED);
}
