#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatUART SerWomUart(sw);  // Declare a Serial Wombat UART  Only one UART can be assigned on the SerialWombat 4B.  This pin mode does not work on Serial Wombat 4A.  Pins 5 and 4 must be used as Rx and TX on the SW8B.  Enhanced digital performance pins must be used on the SW18AB

// There is a video tutorial to go with this example at:  https://youtu.be/C1FjcaiBYZs
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Serial Wombat HW UART Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:

  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !sw.isPinModeSupported(PIN_MODE_UART_RX_TX))
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end


  SerWomUart.begin(57600, 1, 0, 1);                            // Start the UART at 57600, assigned to pin 1, receive on Pin 0, Transmit on Pin 1.

  SerWomUart.println();
  SerWomUart.println("Setup complete");              // Notify that we finished startup.
}



void loop() {

  // put your main code here, to run repeatedly:

  int c = SerWomUart.read();

  switch (c)
  {
    case 'A':
      {
        SerWomUart.println("A is for AARDVARK!");
      }
      break;

    case 'B':
      {
        SerWomUart.println("B is for BUTTERFLY!");
      }
      break;

    case 'C':
      {
        SerWomUart.println("C is for CAT!");
      }
      break;
    case 'X':
      {
        // Print out all printable ASCII characters
        uint8_t array[0x7F - 0x20];
        for (int i = 0x21; i <= 0x7F; ++i)
        {
          array[i - 0x21] = i;
        }
        SerWomUart.write(array, 0x7F - 0x20);

      }
  }
}
