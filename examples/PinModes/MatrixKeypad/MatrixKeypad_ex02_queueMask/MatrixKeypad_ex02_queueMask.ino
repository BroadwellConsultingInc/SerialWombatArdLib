#include <SerialWombat.h>

/*
This example shows how to initialize a 16 key, 8 pin 4x4 matrix keypad using the 
Serial Wombat 18AB or 8B chip'sSerialWombatMatrixKeypad class, and add a queue mask
to allow only certain digits to be entered in the queue

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
possible buffer and queue modes

A video demonstrating the use of the SerialWombatMatrixKeypad class on the Serial Wombat 18AB chip is available at:
https://youtu.be/hxLda6lBWNg 

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_w_s2812.html#details

*/
SerialWombatChip sw;
SerialWombatMatrixKeypad keypad(sw);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Matrix Keypad With Queue Mask Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (sw.isSW04())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_MATRIX_KEYPAD))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end
  
  keypad.begin(10, // Command pin, typically the same as the row0 pin
  10, //row 0
  11, // row 1
  12, // row 2
  13, // row 3
  16, // column 0
  17, // column 1
  18, // column 2
  19); // column 3

  keypad.writeQueueMask(0x2777); // Bit mask with 1's for positions 0,1,2,4,5,6,8,9,0,13 corresponding to keypad marked positions 0-9.
}

void loop() {

  int i = keypad.read();  // returns a byte, or -1 if no value is avaialble
  if (i >0)
  {
    Serial.write((char)i);  // We got a keypress.  Dump it to the Serial Terminal
  }
  
}
