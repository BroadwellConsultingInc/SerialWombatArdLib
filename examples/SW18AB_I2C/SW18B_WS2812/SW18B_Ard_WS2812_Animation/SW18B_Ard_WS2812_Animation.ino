#include "SerialWombat.h"

/*
This example shows how to initialize an animation on a strip/board of WS2812b or equivalent LEDs.  This sketch uses
the SerialWombat18AB's SerialWombatWS2812 class to configure a pin to drive the LEDs.  The
selected pin must be an enhanced performance pin.

When executed this sketch will download 3 frames of 3 leds each to the Serial Wombat chip's user buffer area.  It will cycle the frames
out to the WS2812 LED array and delay between each frame based on a specified per-frame delay.  In this example a Green/Yellow/Red
traffic light will be simulated, with red and green on for 5 seconds each, and yellow on for 1 second.

Change the WS2812_PIN below to fit your circuit.

A video demonstrating the use of the WS2812b pin mode on the Serial Wombat 18AB chip is available at:
//TODO

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_w_s2812.html#details

*/


SerialWombat sw;
SerialWombatWS2812 ws2812(sw);

#define WS2812_PIN 15  // Must be an enhanced performance pin: 0,1,2,3,4,7,9,10-19
#define NUMBER_OF_LEDS 3
#define WS2812_USER_BUFFER_INDEX 0x0000  // Set this to an index into the on-chip user buffer.  Can't overlap with area used by other pins.


// Define colors.  prefix them with SW_ so we don't conflict with any other libraries, such as a graphic display library.
#define SW_RED 0x000F0000   // Red, changed from 0x00FF0000 to reduce power
#define SW_GREEN 0x0000F00
#define SW_WHITE 0x000F0F0F
#define SW_YELLOW 0x000F0F00
#define SW_BLUE 0x0000000F
#define SW_OFF 0x00000000
#define SW_PURPLE 0x000F000F

#define NUMBER_OF_FRAMES 3

uint32_t Frames[NUMBER_OF_LEDS][NUMBER_OF_FRAMES] =
{
    {SW_OFF,SW_OFF,SW_GREEN},
    {SW_OFF,SW_YELLOW,SW_OFF},
    {SW_RED,SW_OFF,SW_OFF},
};

  





void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(500);
  
  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) { showNotFoundError(); while(1){delay(100);}}
  
  sw.begin(Wire,i2cAddress);

  ws2812.begin(WS2812_PIN,  // The Pin connected to WS2812 array
    NUMBER_OF_LEDS,         // The number of LEDs being used
    WS2812_USER_BUFFER_INDEX);  // A location in the Serial Wombat chip's user RAM area where LED output signals will be buffered

    int16_t offset = ws2812.readBufferSize();  // We have a second location in the Serial Wombat chip's user buffer.  This is where
                                               // The animation frames are stored.  The readBufferSize() method gets the length of
                                               // buffer used by the configured number of LEDs.

    ws2812.writeAnimationUserBufferIndex(WS2812_USER_BUFFER_INDEX + offset,  // Location in memory to store the animation frames, after the main WS2812 buffer
                                        NUMBER_OF_FRAMES       // Number of frames
                                        );

    for (int i = 0; i < NUMBER_OF_FRAMES; ++i)
    {
       ws2812.writeAnimationFrame(i,Frames[i]);    // Transfer the frame to the animation buffer on the Serial Wombat chip
       ws2812.writeAnimationFrameDelay(i,5000);// Initalize All Frames 5000 mS delay
    }
    
    ws2812.writeAnimationFrameDelay(1,1000);//Make the yellow frame (index 1 )  only 1000 mS instead of 5000.

    ws2812.writeMode(ws2812ModeAnimation);
  
}


void loop() {
  // No code in here.  The Serial Wombat chip handles generating the LED sequence with no additional 
  // help from the Arduino.  In fact, you could unplug the I2C lines and it would continue working until
  // powered down.
}
