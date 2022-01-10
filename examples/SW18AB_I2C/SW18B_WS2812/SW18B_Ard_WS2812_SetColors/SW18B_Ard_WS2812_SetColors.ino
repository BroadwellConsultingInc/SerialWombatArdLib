#include "SerialWombat.h"

/*
  This example shows how to initialize an animation on a strip/board of WS2812b or equivalent LEDs and use the write
  command to write colors to the LEDs using the SerialWombat18AB's SerialWombatWS2812 class to configure a pin to drive the LEDs.  The
  selected pin must be an enhanced performance pin.

  When executed this sketch will set 16 LEDs to
  Red
  White
  Green
  Blue
  Yellow
  Off,
  Purple
  Blue,
  Blue,
  Blue,
  Red,
  Green
  White
  White
  Yellow
  Green


  Change the WS2812_PIN below to fit your circuit.

  A video demonstrating the use of the WS2812b pin mode on the Serial Wombat 18AB chip is available at:
  //TODO

  Documentation for the SerialWombatTM1637 Arduino class is available at:
  https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_w_s2812.html#details

*/


SerialWombat sw;
SerialWombatWS2812 ws2812(sw);

#define WS2812_PIN 15  // Must be an enhanced performance pin: 0,1,2,3,4,7,9,10-19
#define NUMBER_OF_LEDS 16
#define WS2812_USER_BUFFER_INDEX 0x0000  // Set this to an index into the on-chip user buffer.  Can't overlap with area used by other pins.


// Define colors.  prefix them with SW_ so we don't conflict with any other libraries, such as a graphic display library.
#define SW_RED 0x000F0000   // Red, changed from 0x00FF0000 to reduce power
#define SW_GREEN 0x0000F00
#define SW_WHITE 0x000F0F0F
#define SW_YELLOW 0x000F0F00
#define SW_BLUE 0x0000000F
#define SW_OFF 0x00000000
#define SW_PURPLE 0x000F000F







void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(500);

  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {
    showNotFoundError();
    while (1) {
      delay(100);
    }
  }

  sw.begin(Wire, i2cAddress);

  ws2812.begin(WS2812_PIN,  // The Pin connected to WS2812 array
               NUMBER_OF_LEDS,         // The number of LEDs being used
               WS2812_USER_BUFFER_INDEX);  // A location in the Serial Wombat chip's user RAM area where LED output signals will be buffered

  ws2812.write(0, SW_RED);
  ws2812.write(1, SW_WHITE);
  ws2812.write(2, SW_GREEN);
  ws2812.write(3, SW_BLUE);
  ws2812.write(4, SW_YELLOW);
  ws2812.write(5, SW_OFF);
  ws2812.write(6, SW_PURPLE);
  ws2812.write(7, SW_BLUE);
  ws2812.write(8, SW_BLUE);
  ws2812.write(9, SW_BLUE);
  ws2812.write(10, SW_RED);
  ws2812.write(11, SW_GREEN);
  ws2812.write(12, SW_WHITE);
  ws2812.write(13, SW_WHITE);
  ws2812.write(14, SW_YELLOW);
  ws2812.write(15, SW_GREEN);

}


void loop() {

}
