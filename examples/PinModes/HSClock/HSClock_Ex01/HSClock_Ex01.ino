#include "SerialWombat.h"
/*
  This example shows how to generate a high speed signal using the Serial Wombat 18AB chip's high speed clock generator.

  Only one pin on the Serial Wombat 18AB chip can be configured to this mode at a time.

  A video demonstrating the use of the High Speed Clock Generator is available at:
  TODO

  Documentation for the SerialWombatHighSpeedClock class is available at:
  https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_h_s_clock.htmll
*/

SerialWombatChip sw;
SerialWombat18ABOscillatorTuner oscTun(sw);
SerialWombatHSClock hsClock(sw);

#define HSCLOCK_PIN 10  //Must be an enhanced digital capability pin.

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("High Speed Clock Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (!sw.isSW18())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B or 8B chip.  An  18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_HS_CLOCK))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  hsClock.begin(10, 2000000); // 2MHz output.  Note that only integer divisors of 32MHz are possible.  See documentation


}



void loop() {
  oscTun.update(); // Call this periodically to improve oscillator frequency accuracy.  See https://youtu.be/T2uBQM3s_JM

}
