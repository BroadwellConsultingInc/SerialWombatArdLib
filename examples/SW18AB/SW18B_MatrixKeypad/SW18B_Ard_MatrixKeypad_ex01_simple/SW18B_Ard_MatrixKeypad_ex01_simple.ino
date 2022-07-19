#include <SerialWombat.h>

/*
This example shows how to initialize a 16 key, 8 pin 4x4 matrix keypad using the 
Serial Wombat 18AB chip'sSerialWombatMatrixKeypad class.

This example shows how to treat the matrix keypad as a stream input 
so that it can be treated as if keypresses are Serial Input

Note that firmware versions prior to 2.0.7 have a bug that may cause slow recognition of
button presses.

This example assumes a 4x4 keypad attached with rows connected to pins 10,11,12,13 
and columns attached to pins 16,17,18,19 .  This can be changed in the keypad.begin 
statement to fit your circuit.

This example uses default modes for the SerialWombatMatrixKeypad.  The default values 
send ASCII to the queue assuming a standard 

123A
456B
789C
*0#D

keypad format.   See the pin mode documentation (link below) for more information on the 
possible buffer and queue modes It is assumed that the Serial Wombat chip is at I2C 
address 0x6B.


A video demonstrating the use of the SerialWombatMatrixKeypad class on the Serial Wombat 18AB chip is available at:
https://youtu.be/hxLda6lBWNg

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_w_s2812.html#details

*/
SerialWombatChip sw;
SerialWombatMatrixKeypad keypad(SW6B);

void setup() {

  Serial.begin(115200);
  Wire.begin();
  delay(100);
  sw.begin(Wire,0x6B);

  keypad.begin(10, // Command pin, typically the same as the row0 pin
  10, //row 0
  11, // row 1
  12, // row 2
  13, // row 3
  16, // column 0
  17, // column 1
  18, // column 2
  19); // column 3
}

void loop() {

  int i = keypad.read();  // returns a byte, or -1 if no value is avaialble
  if (i >0)
  {
    Serial.write((char)i);  // We got a keypress.  Dump it to the Serial Terminal
  }
  
}
