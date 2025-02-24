#include "SerialWombat.h"

/*
  This example shows how to control 18 relays using WS2811 breakout boards.  This example assumes that the first relay is
  attached to R, the 2nd to G, the 3rd to B, and so on for 6 LEDs.  The swapRG member of ws2812 is set to true since
  the R and G data positions on the WS2811 silicon are swapped compared to the WS2812 silicon.

  The WS2811 does not drive the Relays directly (WS2811s aren't designed for inductive loads), but rather through
  a relay driver on the relay boards.  Note that the WS2811 is designed to switch the low side of an LED, so it's an open drain
  style output (active low on, high-impedance off) so the relay board needs to be low-input active.

  Try to keep the power lines running the relays away from the signal line for the WS2811's, as the noise created by the 
  relays' coils may induce glitches if the wires are near the signal line, and this can cause the WS2811's to output unexpected
  data.

  Relays are binary devices, so we set each LED driver "color" to either 0  (inactive) or 0xFF (active).

  

  Change the WS2812_PIN below to fit your circuit.

  A video demonstrating the use of the WS2812b pin mode on the Serial Wombat 18AB chip is available at:
  //TODO

  Documentation for the SerialWombatTM1637 Arduino class is available at:
  https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_w_s2812.html#details

*/


SerialWombat sw;
SerialWombatWS2812 ws2812(sw);

#define WS2812_PIN 19  // Must be an enhanced performance pin: 0,1,2,3,4,7,9,10-19
#define NUMBER_OF_LEDS 6
#define WS2812_USER_BUFFER_INDEX 0x0000  // Set this to an index into the on-chip user buffer.  Can't overlap with area used by other pins.

uint32_t ledArray[6] = {0,0,0,0,0,0};

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
  ws2812.swapRG = true; //WS2811 swaps R and G vs WS2812

}


void loop() {
  for (int x = 0; x < 6; ++ x)  // Cycle through 6 LEDs (3 relays for each LED)
  {
    for (int i = 0; i < 6; ++i)
    {
      ledArray[i] = 0;  // Set the color array to all off
    }

    ledArray[x] = 0xFF0000; // Red
    ws2812.write(0,6,ledArray);
    delay(1000);

    ledArray[x] = 0xFF00; // Green
    ws2812.write(0,6,ledArray);
    delay(1000);

      ledArray[x] = 0xFF; // Blue
    ws2812.write(0,6,ledArray);
    delay(1000);

  }
  for (int i = 0; i < 6; ++i)
    {
      ledArray[i] = 0;
    }
    ws2812.write(0,6,ledArray);
    delay(1000);

    //All LEDs on
    for (int i = 0; i < 6; ++i)
    {
      ledArray[i] = 0xFFFFFF;
    }
    ws2812.write(0,6,ledArray);
    delay(1000);

    //All LEDs off
    for (int i = 0; i < 6; ++i)
    {
      ledArray[i] = 0;
    }
    ws2812.write(0,6,ledArray);
    delay(1000);
}
