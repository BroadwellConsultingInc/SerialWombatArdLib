#include "SerialWombat.h"

/*
This example shows how to display an animation on a TM1637 display.  The animation is loaded to the Serial Wombat 18AB chip
from the Arduino board.  The Serial Wombat chip then outputs the animation to the display without any intervention from
the Arduino board.

If you haven't already, run the SW_Ard_TM1637_012345 example to ensure your display displays digits in
the correct order.  If necessary, correct the call to writeDigitOrder below as described in that example.
4 digit displays should use settings to display 0123 in that test to work properly with this sketch.

You can choose an animation to show by commenting in one of the options below //CONFIG:

A video demonstrating the use of the TM1637 pin mode on the Serial Wombat 18AB chip is available at:
https://youtu.be/AwW12n6o_T0

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_t_m1637.html#details

Serial Wombat is a registered trademark in the United States of Broadwell Consulting Inc.
*/

SerialWombat sw;
SerialWombatTM1637 myDisplay(sw);
#define DISPLAY_CLK_PIN 19  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Clock Pin
#define DISPLAY_DIN_PIN 18  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Data Pin


// CONFIG: pick one
#define ANIMATION_ARRAY snake_6_digit
//#define ANIMATION_ARRAY snake_4_digit
// #define ANIMATION_ARRAY leftToRight



//CONFIG:
//#define SPEED 1000  // Slow -  delay 1000mS after updates
#define SPEED 100  // Medium - delay 100mS after updates
//#define SPEED 10  // Fast -   delay 10ms after updates


#define SEG_A 0x1 //TOP 
#define SEG_B 0x2 // UPPER RIGHT
#define SEG_C 0x4 // BOTTOM RIGHT
#define SEG_D 0x8 // BOTTOM
#define SEG_E 0x10 //BOTTOM LEFT
#define SEG_F 0x20 //TOP LEFT
#define SEG_G 0x40 // CENTER
#define SEG_POINT 0x80
#define OFF__ 0



#define VERTRIGHT (SEG_B | SEG_C)
#define VERTLEFT  (SEG_E | SEG_F)
#define TOP (SEG_A)
#define MID (SEG_G)
#define BOT (SEG_D)

uint8_t LeftToRight[18][6] = {  // All arrays are 6 bytes wide, regardless of display width.
  {VERTLEFT, 0,0,0,0,0},
  {VERTRIGHT, 0,0,0,0,0},
  {0,VERTLEFT, 0,0,0,0},
  {0,VERTRIGHT, 0,0,0,0},
  {0,0,VERTLEFT, 0,0,0},
  {0,0,VERTRIGHT, 0,0,0},
  {0,0,0,VERTLEFT, 0,0},
  {0,0,0,VERTRIGHT, 0,0},
  {0,0,0,0,VERTLEFT, 0},
  {0,0,0,0,VERTRIGHT, 0},
  {0,0,0,0,0,VERTLEFT },
  {0,0,0,0,0,VERTRIGHT},
  {0,0,0,0,0,0},            // putting multiple frames the same in looks like a delay
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
   
};
uint8_t snake_6_digit[][6] = {
  
  {TOP,0,0,0,0,0},
  {0,TOP,0,0,0,0},
  {0,0,TOP,0,0,0},
  {0,0,0,TOP,0,0},
  {0,0,0,0,TOP,0},
  {0,0,0,0,0,TOP},

  {0,0,0,0,0,SEG_B},

  {0,0,0,0,0,MID},
  {0,0,0,0,MID,0},
  {0,0,0,MID,0,0},
  {0,0,MID,0,0,0},
  {0,MID,0,0,0,0},
  {MID,0,0,0,0,0},

  {SEG_E,0,0,0,0,0},

  {BOT,0,0,0,0,0},
  {0,BOT,0,0,0,0},
  {0,0,BOT,0,0,0},
  {0,0,0,BOT,0,0},
  {0,0,0,0,BOT,0},
  {0,0,0,0,0,BOT},

  {0,0,0,0,0,SEG_C},

  {0,0,0,0,0,MID},
  {0,0,0,0,MID,0},
  {0,0,0,MID,0,0},
  {0,0,MID,0,0,0},
  {0,MID,0,0,0,0},
  {MID,0,0,0,0,0},

   {SEG_F,0,0,0,0,0},
};


uint8_t snake_4_digit[][6] = {
  
  {TOP,0,0,0,0,0},
  {0,TOP,0,0,0,0},
  {0,0,TOP,0,0,0},
  {0,0,0,TOP,0,0},

  {0,0,0,SEG_B,0,0},

  {0,0,0,MID,0,0},
  {0,0,MID,0,0,0},
  {0,MID,0,0,0,0},
  {MID,0,0,0,0,0},

  {SEG_E,0,0,0,0,0},

  {BOT,0,0,0,0,0},
  {0,BOT,0,0,0,0},
  {0,0,BOT,0,0,0},
  {0,0,0,BOT,0,0},

  {0,0,0,SEG_C,0,0},

  {0,0,0,MID,0,0},
  {0,0,MID,0,0,0},
  {0,MID,0,0,0,0},
  {MID,0,0,0,0,0},

   {SEG_F,0,0,0,0,0},
};

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(500);

  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError(); while(1){delay(100);}}
  
  sw.begin(Wire,i2cAddress,false);

  myDisplay.begin(19,  //Clk Pin
  18, // Data Pin
  6, // Number of digits
  tm1637Animation, // Mode enumeration
  0, // source pin Not used in Animation mode
  4);   // Brightness 
  
  myDisplay.writeDigitOrder(0,1,2,3,4,5);

  
  myDisplay.writeAnimation(0x180, // Place array at index 0x180 in the user buffer
        SPEED,
        sizeof(ANIMATION_ARRAY)/6,  //Number of frames.  Suggest using sizeof like this to calculate.
        ANIMATION_ARRAY);  // Array to load.
}

void loop() {
}
