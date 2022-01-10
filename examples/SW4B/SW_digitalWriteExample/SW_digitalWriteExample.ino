// There is an tutorial video to go with this example at https://youtu.be/uFLlIoolQ_M

#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat


#define GREEN_LED_SW_PIN 2
void setup() {
 
  {//I2C Initialization
    Wire.begin();
    sw.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.
  }

  sw.pinMode(GREEN_LED_SW_PIN,OUTPUT);

}

void loop() {
  
  // put your main code here, to run repeatedly:

  sw.digitalWrite(GREEN_LED_SW_PIN,HIGH);
  delay(1000);
  sw.digitalWrite(GREEN_LED_SW_PIN,LOW);
  delay(1000);
  
}
