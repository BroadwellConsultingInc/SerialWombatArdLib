/*  This example shows how to use the Serial Wombat SPI Pin mode to read ADC readings from a Microchip MCP3201 ADC.
 *   
 *   This example is compatible with the Serial Wombat 18AB and 8B chips.
 *   
  *   
 *   Video on SPI pin mode:
 *   
 *   TODO coming soon
 * 
 *   SerialWombatIRRx pin mode documentation:
 *   
 *   TODO
 */

#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatSPI spi(sw);

#define SPI_CLOCK_PIN 4
#define SPI_MISO_PIN 5
#define SPI_CS_PIN 6
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("SPI MCP3201 Read Example");


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
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_SPI) ) )
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end
  spi.begin(
            SPI_CLOCK_PIN,
            0, //Mode 0
            255, // 255 = None - No MOSI pin
            SPI_MISO_PIN,
            SPI_CS_PIN); 

}

void loop() {
  delay(250);
  uint16_t dummyData = 0; //for 16 bit transfer
  uint16_t rawdata = spi.transfer(dummyData);
  uint16_t result = (((rawdata & 0x1F) << 7) + ( rawdata >>9));  // The bits are not organized as a little-endian number.  Adjust.
  Serial.println(result);
  
}
