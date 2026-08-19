/*  This example shows how to use the Serial Wombat Random Blink pin mode.
 *
 *   This example is compatible with the Serial Wombat 18AB and 8B chips,
 *   when the RandomBlink pin mode is present in the firmware build.
 *
 *   In this example pin 1 is configured as a Random Blink output.  The pin
 *   randomly alternates between on and off.  Each on time and off time is
 *   randomly selected between 0 and 2000 ms.
 *
 *   This example assumes an LED and current limiting resistor with anode connected
 *   to Pin 1, and cathode attached to ground (high side drive)
 *
 *   The on PWM value is fixed at 65535, fully on.  The off PWM value is fixed
 *   at 0, fully off.
 *
 *   SerialWombatRandomBlink pin mode documentation:
 *
 *   TODO coming soon
 *   
 *   SerialWombatRandomBlink tutorial video:
 *   
 *   TODO coming soon
 */

#include <SerialWombat.h>


SerialWombatChip sw;
SerialWombatRandomBlink randomBlink(sw);

#define RANDOM_BLINK_PIN 1

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Random Blink Example");


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
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_RANDOMBLINK) ) )
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  randomBlink.begin(RANDOM_BLINK_PIN, 2000, 2000); // Pin 1, on time up to 2 seconds, off time up to 2 seconds

  Serial.println("Pin 1 is now blinking randomly with on and off times each up to 2 seconds.");
}

void loop() {
  // The Serial Wombat chip controls the random blinking without any additional host activity.
}
