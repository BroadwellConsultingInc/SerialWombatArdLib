#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat
SerialWombatWatchdog Watchdog(sw);  // Declare a Watchdog pin

// A video tutorial for this example is available at: https://youtu.be/fIObjmHmprY
void setup() {
 
  {//I2C Initialization
    Wire.begin();
    sw.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.
  }
  Watchdog.begin(2,                               // Start the watchdog on pin 2.
                 SW_INPUT,                        // Make the pin Input for normal operation
                 SW_LOW,                          // Make the pin go low on timeout
                 10000,                           // Timeout is 10 seconds
                 false);                          // The Serial Wombat won't self-reset on timeout
                                                   

  Serial.begin(115200);                             
  Serial.println();
  Serial.println("Setup Complete.");
}



// This flawed routine works well if A is a multiple of B, but 
// acts badly otherwise because quotient is unsigned and rolls
// back to a big number if the subtraction goes negative.
// Some values, such as 60 / 7 eventually end up returning a
// (wrong) result as the rollover(s) end up eventually
// giving a number that is a multiple of B.
// others such as 60 / 8 stay trapped in the loop forever.
uint8_t DivideAByB( uint8_t A, uint8_t B)
{
  uint8_t C = 0;

  while(A > 0)
  {
    A = A - B;
    ++C;
  }
  return C;
}


int x = 1;
void loop() {
  
  // put your main code here, to run repeatedly:

   Serial.println();
   Serial.print("60 / "); 
   Serial.print(x) ; 
   Serial.print(" = "); 
   Serial.println(DivideAByB(60,x));
   ++x;
   
   
   Watchdog.updateResetCountdown(10000); // Reset the watchdog clock to 10 seconds
   delay(1000);
  
}
