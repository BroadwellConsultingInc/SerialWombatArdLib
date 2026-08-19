/*  This example shows how to use the Serial Wombat Charlieplex pin mode
 *   set-LED and clear-LED commands.
 *
 *   This example is compatible with the Serial Wombat 18AB and 8B chips,
 *   when the Charlieplex pin mode is present in the firmware build.
 *
 *   Eight Serial Wombat pins are used to control up to 56 Charlieplexed
 *   LEDs.  The pins are assigned sequentially from pin 0 through pin 7.
 *
 *   One logical LED moves from LED 0 through LED 55.  The previous LED is
 *   cleared with CONFIGURE_CHANNEL_MODE_6, command byte 0xCE.  The next LED
 *   is set with CONFIGURE_CHANNEL_MODE_5, command byte 0xCD.
 *
 *   The 0xCD and 0xCE commands can each set or clear as many as five specified
 *   logical LEDs without maintaining a bitmap on the Arduino host.
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

#define NUMBER_OF_LEDS 56
#define LED_MOVE_DELAY_MS 200


uint8_t currentLED = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Charlieplex Example 2 - Moving LED");


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
                    56);

  charlieplex.clearLEDs();       // Clear the complete bitmap with 0xCA and 0xCB
  charlieplex.setLEDs(currentLED); // Set logical LED 0 with command 0xCD

  Serial.println("A single LED is moving across logical LEDs 0 through 55.");
}

void loop() {
  delay(LED_MOVE_DELAY_MS);

  charlieplex.clearLEDs(currentLED); // Clear the previous LED with command 0xCE

  ++currentLED;
  if (currentLED >= NUMBER_OF_LEDS)
  {
    currentLED = 0;
  }

  charlieplex.setLEDs(currentLED); // Set the next LED with command 0xCD
}
