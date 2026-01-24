#include <SerialWombat.h>
/*
This example shows how to initialize a 16 key, 8 pin 4x4 matrix keypad using the 
Serial Wombat 18AB or 8B chip's SerialWombatMatrixKeypad class.

Note that firmware versions prior to 2.0.7 have a bug that may cause slow recognition of
button presses.

This example shows how to treat the matrix keypad as if it were 16 separate digital
inputs by creating 16 instances of SerialWombatMatrixButton from a single instance of
SerialWombatMatrixKeypad.  The SerialWombatMatrixKeypad instance scans the keys and
the SerialWombatMatrixButton class abstracts each one into a single digital input.

After initialization the SerialWombatMatrixButton class has the same interfaces and 
is conceptually interchangable with instances of SerialWombatDebouncedInput and 
digitally configured SerialWombat18CapTouch instances.

This example assumes a 4x4 keypad attached with rows connected to pins 10,11,12,13 
and columns attached to pins 16,17,18,19 .  This can be changed in the keypad.begin 
statement to fit your circuit.

A video demonstrating the use of the SerialWombatMatrixKeypad class on the Serial Wombat 18AB chip is available at:
https://youtu.be/hxLda6lBWNg 

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_w_s2812.html#details

*/

SerialWombatChip sw;
SerialWombatMatrixKeypad keypad(sw);
SerialWombatMatrixButton button0(keypad,0);
SerialWombatMatrixButton button1(keypad,1);
SerialWombatMatrixButton button2(keypad,2);
SerialWombatMatrixButton button3(keypad,3);
SerialWombatMatrixButton button4(keypad,4);
SerialWombatMatrixButton button5(keypad,5);
SerialWombatMatrixButton button6(keypad,6);
SerialWombatMatrixButton button7(keypad,7);
SerialWombatMatrixButton button8(keypad,8);
SerialWombatMatrixButton button9(keypad,9);
SerialWombatMatrixButton button10(keypad,10);
SerialWombatMatrixButton button11(keypad,11);
SerialWombatMatrixButton button12(keypad,12);
SerialWombatMatrixButton button13(keypad,13);
SerialWombatMatrixButton button14(keypad,14);
SerialWombatMatrixButton button15(keypad,15);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Matrix Keypad as Individual Buttons Example");


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

  keypad.begin(10,10,11,12,13,16,17,18,19);
}

void loop() {
  // put your main code here, to run repeatedly:

  // If any of the 16 keys is pressed, print its index number.
  if (button0.digitalRead())Serial.print("0 ");
  if (button1.digitalRead())Serial.print("1 ");
  if (button2.digitalRead())Serial.print("2 ");
  if (button3.digitalRead())Serial.print("3 ");
  if (button4.digitalRead())Serial.print("4 ");
  if (button5.digitalRead())Serial.print("5 ");
  if (button6.digitalRead())Serial.print("6 ");
  if (button7.digitalRead())Serial.print("7 ");
  if (button8.digitalRead())Serial.print("8 ");
  if (button9.digitalRead())Serial.print("9 ");
  if (button10.digitalRead())Serial.print("10 ");
  if (button11.digitalRead())Serial.print("11 ");
  if (button12.digitalRead())Serial.print("12 ");
  if (button13.digitalRead())Serial.print("13 ");
  if (button14.digitalRead())Serial.print("14 ");
  if (button15.digitalRead())Serial.print("15 ");

  //Print how many times the lower right key has been pressed or released
  Serial.print(button15.transitions);  Serial.print (" ");

  // Print how long the lower right key has been held down (0 if not pressed)
  Serial.print(button15.readDurationInTrueState_mS());  Serial.print (" ");
    
  Serial.println();
  

}
