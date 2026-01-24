#include "SerialWombat.h"
/*
This example shows how to configure two pins to work together to connect to an IBM PS2 Keyboard.

This example assumes a Serial Wombat 18AB chip is attached to the Arduino board via I2C.

The goal of this example is to read keystrokes in ASCII from the Keyboard / Serial Wombat 18AB chip and output it 
to serial.

Keyboard data and clock lines should be pulled up to +5v using a 2k resistor.  5V tollerant pins (9-12, 14, 15) should 
be used.

A video demonstrating the use of the PS2 Keyboard pin mode on the Serial Wombat 18AB chip is available at:
https://www.youtube.com/watch?v=YV00GfyxFJU

Documentation for the SerialWombatTM1637 Arduino class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_p_s2_keyboard.html
*/

SerialWombatChip sw;
SerialWombatPS2Keyboard myKeyboard(sw);
#define PS2_CLK_PIN 10  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Keyboard Clock Pin
#define PS2_DATA_PIN 11  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Keyboard Data Pin

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("PS2 Keyboard Queueing Example");


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
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_PS2KEYBOARD))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end


  Serial.print ("Connect Clock line to pin ");
  Serial.println(PS2_CLK_PIN);
  Serial.print (" and data pin to pin ");
  Serial.println (PS2_DATA_PIN);
  
 
  myKeyboard.begin(PS2_CLK_PIN,  //Clk Pin
  PS2_DATA_PIN);   // Data Pin 

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
