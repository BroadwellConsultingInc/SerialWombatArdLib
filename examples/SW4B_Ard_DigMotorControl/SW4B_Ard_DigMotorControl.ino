#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat

// This example is explained in a video tutorial at: https://youtu.be/p8CO04C1q_Y

void setup() {
  // put your setup code here, to run once:

  {//I2C Initialization
    Wire.begin();
    sw.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C.
  }

   sw.pinMode(3,OUTPUT);
   Serial.begin(115200);
}


int i;
void loop() {
      
     if (i & 0x01)
     {
        sw.digitalWrite(3,HIGH);
        Serial.println("On");
     }
     else
     {
      sw.digitalWrite(3,LOW);
      Serial.println("Off");
     }
   
      delay(3000);
     ++i;
     
}
