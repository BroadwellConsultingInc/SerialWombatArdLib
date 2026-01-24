#include "SerialWombat.h"
SerialWombat sw;
SerialWombatWS2812 ws2812(sw);
SerialWombatAnalogInput pot(sw);
#define WS2812_PIN 15  // Must be an enhanced performance pin: 0,1,2,3,4,7,9,10-19
#define NUMBER_OF_LEDS 16
#define WS2812_USER_BUFFER_INDEX 0x0000  // Set this to an index into the on-chip user buffer.  Can't overlap with area used by other pins.


//Config:  What data source?  Comment in one
#define DATA_SOURCE SerialWombatDataSource::SW_DATA_SOURCE_PIN_0
//#define DATA_SOURCE SerialWombatDataSource::SW_DATA_SOURCE_FRAMES_RUN_LSW // Increments every 1ms
//#define DATA_SOURCE SerialWombatDataSource::SW_DATA_SOURCE_TEMPERATURE


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
 
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end
  
  pot.begin(0,64,0);
  ws2812.begin(WS2812_PIN,
    NUMBER_OF_LEDS,
    WS2812_USER_BUFFER_INDEX);

  ws2812.barGraph(DATA_SOURCE,
    (uint32_t)0x00050000,   // Off value = dim red
    (uint32_t)0x00000020,   // on value = blue
    (uint16_t)1000,         // Min value 1000
    (uint16_t)64000);       // Max value 64000
}


void loop() {

  
}
