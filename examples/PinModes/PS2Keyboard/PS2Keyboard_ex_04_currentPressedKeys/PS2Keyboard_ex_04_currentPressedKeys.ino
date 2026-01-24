#include "SerialWombat.h"
/*
This example shows how to configure two pins to work together to connect to an IBM PS2 Keyboard.

This example assumes a Serial Wombat 18AB chip is attached to the Arduino board via I2C.

The goal of this example is to monitor if certain keys are currently pressed.  This could be used to use
a PS2 Keyboard as a real-time control keypad.  

This example will print U/D/L/R and S to the Serial Console while Up / Down / Left / Right or Space are held
on the keyboard.  This could be used to add or subtract from Servo or PWM outputs, or other real time control

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
  
  Serial.println ("Serial Wombat 18AB PS2 Keyboard Example.");
  Serial.print ("Connect Clock line to pin ");
  Serial.println(PS2_CLK_PIN);
  Serial.print (" and data pin to pin ");
  Serial.println (PS2_DATA_PIN);
  
  myKeyboard.begin(PS2_CLK_PIN,  //Clk Pin
    PS2_DATA_PIN,    // Data Pin 
    3,   // CREATE Bitfield of held keys
    2); // Increment public data on each keypress

  buzzer.begin( BUZZER_PIN);  // Set a pulse on change for pin 19.  Default values for other parameters are fine
  buzzer.setEntryOnChange(0, PS2_CLK_PIN); // Pulse when the PS2 Keyboard pin mode's public data changes
                
}
  
void loop() {

  int x;
  bool newline = false;
  if (myKeyboard.isKeyPressed(SCANCODE_UPARROW)) { Serial.print("U"); newline = true;}
  if (myKeyboard.isKeyPressed(SCANCODE_DOWNARROW)) { Serial.print("D"); newline = true;}
  if (myKeyboard.isKeyPressed(SCANCODE_RIGHTARROW)) { Serial.print("R"); newline = true;}
  if (myKeyboard.isKeyPressed(SCANCODE_LEFTARROW)) { Serial.print("L"); newline = true;}
  if (myKeyboard.isKeyPressed(SCANCODE_1)) { Serial.print("1"); newline = true;}
  if (myKeyboard.isKeyPressed(SCANCODE_SPACEBAR)) { Serial.print("S"); newline = true;}
  if (newline) Serial.println();
  
}
