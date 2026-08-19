/*  This example shows how to use the Serial Wombat Charlieplex pin mode as a
 *   12-LED bargraph controlled by a quadrature encoder.
 *
 *   This example is compatible with the Serial Wombat 18AB and 8B chips,
 *   when the Charlieplex and Quadrature Encoder pin modes are present in the
 *   firmware build.
 *
 *   Four Serial Wombat pins are used to control 12 Charlieplexed LEDs.  The
 *   Charlieplex pins are assigned sequentially from pin 0 through pin 3.
 *
 *   The quadrature encoder uses pins 6 and 7.  Its 16-bit public data value is
 *   read directly by the Charlieplex pin mode's scaled-output block.  No host
 *   transfer is needed to update the bargraph after configuration.
 *
 *   Encoder values from 0 through 11 are scaled to Charlieplex output values
 *   from 0 through 11.  Bargraph mode lights logical LEDs 0 through the scaled
 *   output value, so encoder position 0 lights one LED and position 11 lights
 *   all 12 LEDs.
 *
 *   The Arduino loop constrains the unsigned encoder counter to 0 through 11.
 *   This prevents decrementing below zero from wrapping to 65535 and prevents
 *   incrementing beyond the top of the bargraph.
 *
 *   Each LED must have an appropriate current-limiting resistor.  Connect the
 *   quadrature encoder common terminal to ground; the internal pull-up
 *   resistors are enabled on both encoder inputs.
 *
 *   SerialWombatCharlieplex pin mode documentation:
 *
 *   TODO coming soon
 *
 *   SerialWombatQuadEnc pin mode documentation:
 *
 *   https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_quad_enc.html
 */

#include <SerialWombat.h>
#include <SerialWombatCharlieplex.h>
#include <SerialWombatQuadEnc.h>

SerialWombatChip sw;
SerialWombatCharlieplex charlieplex(sw);
SerialWombatQuadEnc quadratureEncoder(sw);

#define CHARLIEPLEX_PIN_0 0
#define CHARLIEPLEX_PIN_1 (CHARLIEPLEX_PIN_0 + 1)
#define CHARLIEPLEX_PIN_2 (CHARLIEPLEX_PIN_1 + 1)
#define CHARLIEPLEX_PIN_3 (CHARLIEPLEX_PIN_2 + 1)

#define QUADRATURE_ENCODER_PIN_A 6
#define QUADRATURE_ENCODER_PIN_B 7

#define NUMBER_OF_LEDS 12
#define MAXIMUM_ENCODER_VALUE (NUMBER_OF_LEDS - 1)


uint16_t lastEncoderValue = 0xFFFF;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Charlieplex Example 4 - Quadrature Encoder Bargraph");


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
  if (sw.isSW08() &&
      (!(sw.isPinModeSupported(PIN_MODE_CHARLIEPLEX)) ||
       !(sw.isPinModeSupported(PIN_MODE_QUADRATUREENCODER))))
  {
    Serial.println("The required pin modes do not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  quadratureEncoder.begin(QUADRATURE_ENCODER_PIN_A,
                          QUADRATURE_ENCODER_PIN_B,
                          10,              // 10 ms debounce
                          true,            // Enable weak pull-ups
                          QE_ONLOW_POLL);   // One update on each high-to-low transition
  quadratureEncoder.write(0);

  charlieplex.begin(CHARLIEPLEX_PIN_0,
                    CHARLIEPLEX_PIN_1,
                    CHARLIEPLEX_PIN_2,
                    CHARLIEPLEX_PIN_3,
                    SerialWombatCharlieplex::UNUSED_PIN,
                    SerialWombatCharlieplex::UNUSED_PIN,
                    SerialWombatCharlieplex::UNUSED_PIN,
                    SerialWombatCharlieplex::UNUSED_PIN,
                    SerialWombatCharlieplex::DISPLAY_MODE_SCALED_BARGRAPH,
                    NUMBER_OF_LEDS);

  // Scale encoder public data values 0 through 11 to the full internal
  // 0 through 65535 scaled-output range.
  charlieplex.writeInputScaling(0, MAXIMUM_ENCODER_VALUE);

  // Scale the internal 0 through 65535 value back to logical LED indexes 0
  // through 11 for the Charlieplex bargraph display mode.
  charlieplex.writeOutputScaling(0, MAXIMUM_ENCODER_VALUE);

  // Read the quadrature encoder public data directly from encoder pin A.
  charlieplex.writeScalingEnabled(true, QUADRATURE_ENCODER_PIN_A);

  Serial.println("Turn the encoder to adjust the 12-LED Charlieplex bargraph.");
}

void loop() {
  uint16_t encoderValue = quadratureEncoder.read();

  // The quadrature encoder position is unsigned.  A value in the upper half
  // of the range indicates that the encoder was decremented below zero.
  if (encoderValue > 0x7FFF)
  {
    encoderValue = 0;
    quadratureEncoder.write(encoderValue);
  }
  else if (encoderValue > MAXIMUM_ENCODER_VALUE)
  {
    encoderValue = MAXIMUM_ENCODER_VALUE;
    quadratureEncoder.write(encoderValue);
  }

  if (encoderValue != lastEncoderValue)
  {
    Serial.print("Encoder position: ");
    Serial.print(encoderValue);
    Serial.print("  LEDs illuminated: ");
    Serial.println(encoderValue + 1);
    lastEncoderValue = encoderValue;
  }

  delay(10);
}
