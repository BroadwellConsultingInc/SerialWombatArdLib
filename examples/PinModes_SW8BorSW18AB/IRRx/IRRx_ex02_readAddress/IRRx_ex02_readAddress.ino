/*  This example shows how to use the Serial Wombat IR Receive (IRRx) pin mode.  It assumes an NEC compatible IR Transmitter   
 *   (most inexpensive Arduino kit remotes are compatible) and a 38kHz receiver module that goes low when a modulated IR signal is
 *   present.
 *   
 *   This example is compatible with the Serial Wombat 18AB and 8B chips.
 *   
 *   In this example a 16 bit address is received from the transmitter and printed to Serial in hex format.
 *   
 *   This example is useful to determine the address used by a generic NEC transmitter.  This can then be used in
 *   the address filtering example (Example 3) to allow only a specific address to be received.
 *   
 *   NEC compatible transmitters may either use and 8 bit address and then send the complement of the 
 *   address in the alternative byte, or may use all 16 bits for one address.  
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
#ifdef ARDUINO_ESP8266_GENERIC
  Wire.begin(2, 0); // ESP-01 - SDA GPIO2, SCL GPIO0
#else
  Wire.begin();
#endif

  Serial.begin(115200);

  delay(500);

  sw.begin(Wire, sw.find(true));
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}


  irrx.begin(IRRX_PIN); // Default parameters:  NEC Mode, queue repeats, active low, 1000ms public data timeout, 0xFFFF public data default, no address filtering
//irrx.enablePullup(true);   //Comment in this line if your receiver is open drain type without pullup 
}

void loop() {
  int receivedData = irrx.read();

  if (receivedData >=0)
  {
    char s[10];
    sprintf(s,"0x%X",irrx.readAddress());  // Read the 16 bit address of the transmitter, and convert it to a hex string
    Serial.println(s);
  }
  
}
