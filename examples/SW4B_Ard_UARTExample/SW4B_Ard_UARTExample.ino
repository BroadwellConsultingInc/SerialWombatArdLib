#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat
SerialWombatUART SWUart(sw);  // Declare a Serial Wombat UART  Only one UART can be assigned on the SerialWombat 4B.  This pin mode does not work on Serial Wombat 4A.

// There is a video tutorial to go with this example at:  https://youtu.be/C1FjcaiBYZs
void setup() {

  {//I2C Initialization
    Wire.begin();
    Wire.setClock(400000);
    sw.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.
  }
  SWUart.begin(115200,1,0,1);                               // Start the UART at 115200, assigned to pin 1, receive on Pin 0, Transmit on Pin 1.
               
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
