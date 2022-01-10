#include "SerialWombat.h"
SerialWombat sw;
SerialWombatWS2812 ws2812(sw);
SerialWombatAnalogInput pot(sw);
#define WS2812_PIN 15  // Must be an enhanced performance pin: 0,1,2,3,4,7,9,10-19
#define NUMBER_OF_LEDS 16
#define WS2812_USER_BUFFER_INDEX 0x0000  // Set this to an index into the on-chip user buffer.  Can't overlap with area used by other pins.


//Config:  What data source?  Comment in one
#define DATA_SOURCE SW_DATA_SOURCE_PIN_0
//#define DATA_SOURCE SW_DATA_SOURCE_FRAMES_RUN_LSW // Increments every 1ms
//#define DATA_SOURCE SW_DATA_SOURCE_TEMPERATURE


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(500);
  
  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) { showNotFoundError(); while(1){delay(100);}}
  sw.begin(Wire,i2cAddress,false);

  pot.begin(0,64,0);
  ws2812.begin(WS2812_PIN,
    NUMBER_OF_LEDS,
    WS2812_USER_BUFFER_INDEX);

  ws2812.barGraph(DATA_SOURCE,
    0x00050000,   // Off value = dim red
    0x00000020,   // on value = blue
    1000,         // Min value 1000
    64000);       // Max value 64000
}


void loop() {

  
}
