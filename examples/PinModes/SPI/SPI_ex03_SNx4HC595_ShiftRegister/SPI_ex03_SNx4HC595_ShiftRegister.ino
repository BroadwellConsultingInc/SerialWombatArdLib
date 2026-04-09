/*  This example shows how to use the Serial Wombat SPI Pin mode to output digital
 *   signals on a SNx4HC595 shift register.
 *   
 *   If you're considering this, I'd consider just buying more SW8B boards.  They're
 *   cheap, much more versatile, and likely make your design adaptable for the future.  
 *   
 *   This will cycle through a variety of output values to show patterns on an LED array
 *   
 *   SNxHC595 Pinout:
                 +----\/----+
 Qb      1  ---- |1      16| ---- VCC 
 Qc      2  ---- |2      15| ---- Qa
 Qd      3  ---- |3      14| ---- SER (DS)  <-- Serial Wombat MOSI Pin
 Qe      4  ---- |4      13| ---- OE        <-- GND (enable outputs)
 Qf      5  ---- |5      12| ---- RCLK      <-- Serial Wombat CS pin
 Qg      6  ---- |6      11| ---- SRCLK     <-- Serial Wombat SPI Clock pin
 Qh      7  ---- |7      10| ---- SRCLR     <-- VCC (no reset)
 GND     8  ---- |8       9| ---- Qh' (cascade out)
                 +-----------+ *   
 *   This example is compatible with the Serial Wombat 18AB and 8B chips.
 *   
 *   The calls to spi transfer on this example are compatible with other spi.transfer 
 *   calls using the normal Arduino SPI library.  However, calls to beginTransation()
 *   and end() are unnecessary, and bit order and clock frequency are not configurable.
 *   
 *   Video on SPI pin mode:
 *   
 *   TODO coming soon
 * 
 *   SerialWombatSPI pin mode documentation:
 *   
 *   TODO
 */

#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatSPI spi(sw);

#define SPI_CLOCK_PIN 4
#define SPI_MISO_PIN 5
#define SPI_MOSI_PIN 7
#define SPI_CS_PIN 6
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("SPI SNx4HC595 Shfit Register Example");


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
            SPI_MOSI_PIN, 
            SPI_MISO_PIN,
            SPI_CS_PIN); 
 
}

const uint8_t outputArray[] = 
{
  0xFF,
  0x55,
  0xAA,
  0xF0,
  0x0F,
  0xC3,
  0x3C,
  0x00,
};

int outputIndex = 0;
void loop() {

  spi.transfer(outputArray[outputIndex]);
  ++outputIndex;
  if (outputIndex >= sizeof(outputArray))
  {
    outputIndex = 0;
  }

  delay(300);

}
