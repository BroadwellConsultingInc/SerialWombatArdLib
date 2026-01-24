#include "SerialWombat.h"

/*
  This example shows how to configure two pins to work together to drive a TM1637 seven-segment
  LED display with a Serial Wombat 8B or 18AB chip.

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

SerialWombatChip sw;
SerialWombatTM1637 myDisplay(sw);
#define DISPLAY_CLK_PIN 19  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Clock Pin
#define DISPLAY_DIN_PIN 18  // <<<<<<<<<   Set this to the Serial Wombat pin connected to your Display Data Pin

void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("TM1637 123456 Example");


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
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_TM1637))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

  myDisplay.begin(DISPLAY_CLK_PIN,  //Clk Pin
                  DISPLAY_DIN_PIN, // Data Pin
                  6, // Number of digits
                  tm1637CharArray, // Mode enumeration
                  0x55, // Source pin Not used in tm1637CharArray mode
                  4);   // Brightness
  // myDisplay.writeDigitOrder(2,3,4,5,0,1);


  uint8_t test[7] = "012345";
  myDisplay.writeArray(test);

}

void loop() {

}
