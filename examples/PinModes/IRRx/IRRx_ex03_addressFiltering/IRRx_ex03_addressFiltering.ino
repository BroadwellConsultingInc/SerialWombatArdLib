/*  This example shows how to use the Serial Wombat IR Receive (IRRx) pin mode.  It assumes an NEC compatible IR Transmitter   
 *   (most inexpensive Arduino kit remotes are compatible) and a 38kHz receiver module that goes low when a modulated IR signal is
 *   present.
 *   
 *   This example is compatible with the Serial Wombat 18AB and 8B chips.
 *   
 *   In this example commands are received from the pin mode and printed to Serial.  Address filtering is turned on 
 *   so that only a single transmitter address is processed (other addresses are ignored).  The address is set
 *   with the #define TRANSMITTER_ADDRESS .  Example 2 can help find the address of a given transmitter.
 *   
 *   
 *   NEC compatible transmitters may either use and 8 bit address and then send the complement of the 
 *   address in the alternative byte, or may use all 16 bits for one address.  The Serial Wombat IRRx pin mode
 *   treats both of these cases as a single 16 bit address.
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
#define TRANSMITTER_ADDRESS 0xEF00

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
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_IRRX) ) )
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
              true,   // Use Address Filtering
              TRANSMITTER_ADDRESS); // Filter to address TRANSMITTER_ADDRESS  (This value was determined for a given remote using example 2
              
//irrx.enablePullup(true);   //Comment in this line if your receiver is open drain type without pullup 

}

void loop() {
  int receivedData = irrx.read();

  if (receivedData >=0)
  {
    Serial.println(receivedData);
  }
  
}
