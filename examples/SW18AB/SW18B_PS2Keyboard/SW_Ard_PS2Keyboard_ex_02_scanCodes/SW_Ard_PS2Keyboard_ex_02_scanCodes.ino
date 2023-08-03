#include "SerialWombat.h"
/*
This example shows how to configure two pins to work together to connect to an IBM PS2 Keyboard
and read raw scan codes.

This example assumes a Serial Wombat 18AB chip is attached to the Arduino board via I2C.


Keyboard data and clock lines should be pulled up to +5v using a 2k resistor.  5V tollerant pins (9-12, 14, 15) should 
be used.

A video demonstrating the use of the PS2 Keyboard pin mode on the Serial Wombat 18AB chip is available at:
TODO

Documentation for the SerialWombatPS2Keyboard Arduino class is available at:
TODO
*/

SerialWombatChip sw;
SerialWombatPS2Keyboard myKeyboard(sw);
#define PS2_CLK_PIN 10  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Keyboard Clock Pin
#define PS2_DATA_PIN 11  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Keyboard Data Pin

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  delay(500);
  Serial.println ("Serial Wombat 18AB PS2 Keyboard Example.");
  Serial.print ("Connect Clock line to pin ");
  Serial.println(PS2_CLK_PIN);
  Serial.print (" and data pin to pin ");
  Serial.println (PS2_DATA_PIN);
  
  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  versionCheck(); //see showErrorNotFound tab
  
  sw.begin(Wire,i2cAddress);

  myKeyboard.begin(PS2_CLK_PIN,  //Clk Pin
  PS2_DATA_PIN, // Data Pin 
  2);   // All scan codes

}

int count = 0; // How many on the line?

uint32_t lastCodeTimestamp = 0;
void loop() {

  int x;
  x = myKeyboard.read();  // Read the keyboard queue.  Returns -1 if no characters available
  while (x > 0)
  {

   
    if (count > 25 or millis() > lastCodeTimestamp + 1000)
    {
      // start a new line if we've printed 25 codes or if it's been more than 1 second since the last code.
      Serial.println();
      count = 0;
    }
    
    Serial.print(x, HEX);    // Send the code to the serial interface
    Serial.print(' ');
    ++ count;
    lastCodeTimestamp = millis();
    
    x = myKeyboard.read();  // Read the keyboard queue.  Returns -1 if no characters available
  }

}
