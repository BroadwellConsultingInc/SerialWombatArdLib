/*  This example shows how to use the Serial Wombat IR Receive (IRRx) pin mode.  It assumes an NEC compatible IR Transmitter   
 *   (most inexpensive Arduino kit remotes are compatible) and a 38kHz receiver module that goes low when a modulated IR signal is
 *   present.
 *   
 *   This example is compatible with the Serial Wombat 18AB and 8B chips.
 *   
 *   In this example command bytes sent by the transmitter are read from an Arduino Stream and printed to Serial.
 *   This example uses all of the default paramters for the pin mode.  When a repeat code is received from the tranmitter the
 *   last command byte is queued again.  No address filtering is done, so any command byte from any transmitter is queued.
 *   
 *   Video on IRRx pin mode:
 *   
 *   TODO coming soon
 * 
 *   SerialWombatIRRx pin mode documentation:
 *   
 *   TODO
 */

#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatIRRx irrx(sw);

#define IRRX_PIN 7
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("IR Read Example");


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
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_IRRX) ) )
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end
  irrx.begin(IRRX_PIN); // Default parameters:  NEC Mode, queue repeats, active low, 1000ms public data timeout, 0xFFFF public data default, no address filtering
  //irrx.enablePullup(true);   //Comment in this line if your receiver is open drain type without pullup 

}

void loop() {
  int receivedData = irrx.read();

  if (receivedData >=0)
  {
    Serial.println(receivedData);
  }
  
}
