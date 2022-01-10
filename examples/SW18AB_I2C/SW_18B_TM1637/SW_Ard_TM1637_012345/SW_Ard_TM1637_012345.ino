#include "SerialWombat.h"

/*
This example shows how to configure two pins to work together to drive a TM1637 seven-segment
LED display with a Serial Wombat 18AB chip.

The goal of this example is to display "012345" on the display.  Ideally, "012345" will be displayed
on a 6-segment display, and "0123" will be displayed on a 4 digit display.

However, some displays may have the digits connected to the display in a different order than expected.
For instance, the 6 segment diymore display I bought on Amazon in September of 2021 displayed "210543"
indicating that each of the 3-segment LED displays used to make up the 6 digits was wired backwards.

In the example below, note the call to writeDigitOrder().  This function is used to correct digit order.
Simply enter the number displayed on the display by default as parameters and the display will show
correctly.  For example for the diymore display:
writeDigitalOrder(2,1,0,5,4,3);

A video demonstrating the use of the TM1637 pin mode on the Serial Wombat 18AB chip is available at:
https://youtu.be/AwW12n6o_T0

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_t_m1637.html#details

*/

SerialWombat sw;
SerialWombatTM1637 myDisplay(sw);
#define DISPLAY_CLK_PIN 19  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Clock Pin
#define DISPLAY_DIN_PIN 18  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Data Pin

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(500);

  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();}
  
  sw.begin(Wire,i2cAddress,false);

  myDisplay.begin(DISPLAY_CLK_PIN,  //Clk Pin
  DISPLAY_DIN_PIN, // Data Pin
  6, // Number of digits
  tm1637CharArray, // Mode enumeration
  0x55, // Source pin Not used in tm1637CharArray mode
  4);   // Brightness 
  myDisplay.writeDigitOrder(2,3,4,5,0,1);


  uint8_t test[7] = "012345";
  myDisplay.writeArray(test);

}
  
void loop() {

}
