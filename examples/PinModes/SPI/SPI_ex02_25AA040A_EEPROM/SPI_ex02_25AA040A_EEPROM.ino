/*  This example shows how to use the Serial Wombat SPI Pin mode to read And write data from a Microchip 25AA040A 
 *   512 byte EEPROM.
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
#define SPI_MOSI_PIN 7
#define SPI_CS_PIN 3
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("SPI 25AA040A EEPROM Example");


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
  spi.transfer((uint8_t)0x06); // Enable Writes on EEPROM.  Be sure to cast single byte constants so compiler interprets them that way.

{
  uint8_t buf[] ={0x02, 0x10, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,0x39, 0x61, 0x62,0x63,0x64,0x65,0x66}; // Write "0123456789ABCDEF" to addres 0x010
  spi.transfer(buf,18);  //Transfer 18 bytes 
}
  delay(2000);  // Allow time for write to complete

 spi.transfer((uint8_t)0x06); // Enable Writes on EEPROM.  Be sure to cast single byte constants so compiler interprets them that way.

{
  uint8_t buf[] ={0x02, 0xFF, 0xD7}; // Write 0x D7 to addres 0x1FF
  spi.transfer(buf,3);  //Transfer 3 bytes
}
delay(10); // Allow time for write to complete
spi.transfer((uint8_t)0x06); // Enable Writes on EEPROM.  Be sure to cast single byte constants so compiler interprets them that way.

{
  uint8_t buf[] ={0x02, 0x00, 0x67}; // Write 0x67 to addres 0x000
  spi.transfer(buf,3);  //Transfer 3 bytes
}

}

uint8_t readBuf[514];
void loop() {

  readBuf[0] = 0x03; //Read command
  readBuf[1] = 0x00; // Start at address 0x00
  spi.transfer(readBuf,514);

  for (int row = 0; row < 32; ++row)
  {
    char r[10];
    sprintf(r, "%03X: ",row * 16);
    Serial.print(r);
    for (int column = 0; column < 16; ++ column)
    {
      char s[10];
      sprintf(s,"%02X ",readBuf[row * 16 + column + 2]);
      Serial.print(s);
    }
    Serial.println();
  }
  Serial.println();
  Serial.println();
  Serial.println();
    delay(5000);

}
