#include <SerialWombat.h>

/*

This example returns the key pressed based on its index position (0..15), rather than they key's ASCII value.

This example shows how to initialize a 16 key, 8 pin 4x4 matrix keypad using the 
Serial Wombat 18AB chip'sSerialWombatMatrixKeypad class.

This example shows how to treat the matrix keypad as a stream input 
so that it can be treated as if keypresses are Serial Input.

Note that firmware versions prior to 2.0.7 have a bug that may cause slow recognition of
button presses.

This example assumes a 4x4 keypad attached with rows connected to pins 10,11,12,13 
and columns attached to pins 16,17,18,19 .  This can be changed in the keypad.begin 
statement to fit your circuit.

Using the index position of the key pressed may be useful if your keypad has a non-standard layout, or you want to represent your keys in a custom way.

By default, SerialWombatMatrixKeypad will return the ASCII value of the key pressed - '0', '1', '*' etc.
In some cases, you may want the index (position) of the key pressed - 0,1,2..15.

You can use the index itself (0..15), or create a simple character mapping.

Instead of the default layout:

123A
456B
789C
*0#D

You might have a unique layout:

0123
4567
89AB
CDEF

or even one that is unique to your application:

0123
4567
89<>
AMSC

Using the index of the key pressed, it is simple to map it to its desired character equivalent.

See the pin mode documentation (link below) for more information on the 
possible buffer and queue modes It is assumed that the Serial Wombat chip is at I2C 
address 0x6B.

A video demonstrating the use of the SerialWombatMatrixKeypad class on the Serial Wombat 18AB chip is available at:
https://youtu.be/hxLda6lBWNg

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_w_s2812.html#details

*/

SerialWombatChip sw;
SerialWombatMatrixKeypad keypad(sw);

/*
Map index to keys using an example layout:

0123
4567
89AB
CDEF

*/

char keymap[] = "0123456789ABCDEF";

void setup() {

  Serial.begin(115200);
  Wire.begin();
  delay(100);
  sw.begin(Wire,0x6B);

  keypad.begin(10, // Command pin, typically the same as the row0 pin
  10, // row 0
  11, // row 1
  12, // row 2
  13, // row 3
  16, // column 0
  17, // column 1
  18, // column 2
  19, // column 3
  1,  // BufferMode - 1 Last Key Index
  0); // QueueMode - 0 Index (vs ASCII)
}

void loop() {

  int index = keypad.read();  // returns a byte, or -1 if no value is avaialble
  // If a key is pressed, the index position 0..15 is returned
  // Special Note: Check for any value greater than or equal to zero
  if (index >= 0)
  {
    // Optionally map the index to a custom key value
    char key = keymap[index];
    // Print the index and the mapped character
    Serial.printf("\nIndex: %d, Key: %c", index, (char) key);
  }
  
}
