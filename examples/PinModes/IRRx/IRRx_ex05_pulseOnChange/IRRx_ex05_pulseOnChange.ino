/*  This example shows how to use the Serial Wombat IR Receive (IRRx) pin mode.  It assumes an NEC compatible IR Transmitter
     (most inexpensive Arduino kit remotes are compatible) and a 38kHz receiver module that goes low when a modulated IR signal is
     present.

     This example is compatible with the Serial Wombat 18AB and 8B chips.


     In this example commands are received from the pin mode and printed to Serial,

     By default the pin mode provides a command byte as its 16 bit public data value.  However, it can be reconfigured
     so that the 16 bit public data value increments each time a command (or a repeat code) is received.  A Blink
     pin mode can then read this value to chirp a buzzer or blink an LED when IR commands are received.  Note that this is
     more sophisticated than the visible LED on some IR Receivers.  The Serial Wombat Blink pin will only pulse
     when data is successfully received and decoded.  An LED on an IR Receivers blinks to indicate presense of an IR signal,
     not successful receiption of an IR packet.


     Video on IRRx pin mode:

     TODO coming soon

     SerialWombatIRRx pin mode documentation:

     TODO
*/

#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatIRRx irrx(sw);
SerialWombatBlink swBlink(sw);

#define IRRX_PIN 7
#define BLINK_PIN 1


void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("IR Address Filtering Example");


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
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_IRRX) &&  sw.isPinModeSupported(PIN_MODE_BLINK)) )
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end
  irrx.begin(IRRX_PIN,
             0,//Mode 0 : NEC
             true, // Use repeat
             SW_LOW,// Active Low
             1000, // 1000 ms Public Data Timeout
             0xFFFF, // Default public data
             false,   // Use Address Filtering
             0x0000, // Transmitter Address (not used)
             SerialWombatIRRx::publicDataOutput::DATACOUNT); // Make data count the output

  //irrx.enablePullup(true);   //Comment in this line if your receiver is open drain type without pullup


  swBlink.begin  (BLINK_PIN, IRRX_PIN);

  delay(1000);
}

void loop() {
  int receivedData = irrx.read();

  if (receivedData >= 0)
  {
    Serial.print(irrx.readPublicData());
    Serial.print(":");
    Serial.println(receivedData);
  }

}
