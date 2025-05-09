#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat chip
SerialWombatUART SWUart(sw);  // Declare a Serial Wombat UART  Only one UART can be assigned on the SerialWombat 4B.  This pin mode does not work on Serial Wombat 4A.  Pins 5 and 4 must be used as Rx and TX on the SW8B.  Enhanced digital performance pins must be used on the SW18AB

// There is a video tutorial to go with this example at:  https://youtu.be/C1FjcaiBYZs
void setup() {

Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial


  SWUart.begin(57600,1,0,1);                               // Start the UART at 115200, assigned to pin 1, receive on Pin 0, Transmit on Pin 1.
               
  SWUart.println();
  SWUart.println("Setup complete");              // Notify that we finished startup.
}



void loop() {
  
  // put your main code here, to run repeatedly:
   
   int c = SWUart.read();

      switch (c)
      {
        case 'A':
        {
          SWUart.println("A is for AARDVARK!");
        }
        break;

        case 'B':
        {
          SWUart.println("B is for BUTTERFLY!");
        }
        break;

         case 'C':
        {
          SWUart.println("C is for CAT!");
        }
        break;
        case 'X':
        {
          // Print out all printable ASCII characters
          uint8_t array[0x7F-0x20];
          for (int i = 0x21; i <= 0x7F; ++i)
          {
            array[i-0x21] = i;
          }
          SWUart.write(array,0x7F-0x20);
          
        }
      }
}
