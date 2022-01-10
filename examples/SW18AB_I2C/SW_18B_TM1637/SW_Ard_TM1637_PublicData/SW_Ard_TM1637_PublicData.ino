#include "SerialWombat.h"

/*
This example shows how to display a number on a TM1637 display based on the public data of a Serial Wombat pin or other
data source within the Serial Wombat chip.

If you haven't already, run the SW_Ard_TM1637_012345 example to ensure your display displays digits in
the correct order.  If necessary, correct the call to writeDigitOrder below as described in that example.
For four digit displays, you'll likely want to use writeDigitOrder(2,3,4,5,0,1) in order to show the least
significant digits.

This sketch assumes a potentiometer output dividing ground and 3.3v  is connected to pin 0 as an analog input.

This sketch is designed to be experimented with.  Comment in #define's in the  CONFIG sections below to try out different options and how they affect the display

A video demonstrating the use of the TM1637 pin mode on the Serial Wombat 18AB chip is available at:
https://youtu.be/AwW12n6o_T0

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_t_m1637.html#details

*/

SerialWombat sw;
SerialWombatTM1637 myDisplay(sw);
#define DISPLAY_CLK_PIN 19  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Clock Pin
#define DISPLAY_DIN_PIN 18  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Data Pin

SerialWombatAnalogInput potentiometer(sw);

//CONFIG:  Which mode... Hex or decimal?  Comment in one...
#define TM1637_MODE tm1637Decimal16
//#define TM1637_MODE tm1637Hex16


//Config:  What data source?  Comment in one
//#define DATA_SOURCE SW_DATA_SOURCE_PIN_0
#define DATA_SOURCE SW_DATA_SOURCE_INCREMENTING_NUMBER
//#define DATA_SOURCE SW_DATA_SOURCE_TEMPERATURE


//Config:  blank leading zeros?   Make True or false
#define SURPRESS_LEADING_ZEROS true


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
  TM1637_MODE, // Mode enumeration
  DATA_SOURCE,
  4);   // Brightness 

  myDisplay.writeDigitOrder(0,1,2,3,4,5);

  myDisplay.suppressLeadingZeros(SURPRESS_LEADING_ZEROS);

  potentiometer.begin(0,64,0);
}

void loop() {
}
