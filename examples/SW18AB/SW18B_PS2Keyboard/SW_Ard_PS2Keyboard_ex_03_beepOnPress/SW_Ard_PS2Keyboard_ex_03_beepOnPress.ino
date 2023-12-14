#include "SerialWombat.h"
/*
This example shows how to configure two pins to work together to connect to an IBM PS2 Keyboard.

This example assumes a Serial Wombat 18AB chip is attached to the Arduino board via I2C.

The goal of this example is to read keystrokes in ASCII from the Keyboard / Serial Wombat 18AB chip and output it 
to serial, providing audible feedback from a buzzer on pin 19.

Keyboard data and clock lines should be pulled up to +5v using a 2k resistor.  5V tollerant pins (9-12, 14, 15) should 
be used.

This example uses the SerialWombatPulseOnChange class as well as the SerialWombatPS2Keyboard class.  In addition
to storing key presses for retreival by the host, the SerialWombatPS2Keyboard pin mode will be configured to
increment its 16 bit public data each time a make-code is detected.  The SerialWombatPulseOnChange class will
then pulse high for a short period of time on each increment, creating an audible tone.

A video demonstrating the use of the PS2 Keyboard pin mode on the Serial Wombat 18AB chip is available at:
https://www.youtube.com/watch?v=YV00GfyxFJU

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_p_s2_keyboard.html

*/

SerialWombatChip sw;
SerialWombatPS2Keyboard myKeyboard(sw);
SerialWombatPulseOnChange buzzer(sw);

#define PS2_CLK_PIN 10  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Keyboard Clock Pin
#define PS2_DATA_PIN 11  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Keyboard Data Pin
#define BUZZER_PIN 19  // <<<<<<<<<<<   Set this to the Serial Wombat pin connected to a buzzer
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
    PS2_DATA_PIN,    // Data Pin 
    0,   // Queue ASCII Data
    2); // Increment public data on each keypress

  buzzer.begin( BUZZER_PIN);  // Set a pulse on change for pin 19.  Default values for other parameters are fine
  buzzer.setEntryOnChange(0, PS2_CLK_PIN); // Pulse when the PS2 Keyboard pin mode's public data changes
                
}
  
void loop() {

  int x;
  x = myKeyboard.read();  // Read the keyboard queue.  Returns -1 if no characters available
  while (x > 0)
  {
    char c = (char) x;  // Convert the value to a character
    Serial.print(c);    // Send the character to the serial interface
    x = myKeyboard.read();  // Read the keyboard queue.  Returns -1 if no characters available
  }

}
