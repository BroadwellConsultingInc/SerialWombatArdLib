#include <SerialWombat.h>

SerialWombat sw;    //Declare a Serial Wombat

SerialWombatDebouncedInput blueButton(sw);

SerialWombatDebouncedInput redButton(sw);
SerialWombatButtonCounter redCounter(&redButton);

SerialWombatDebouncedInput greenButton(sw);
SerialWombatButtonCounter greenCounter(&greenButton);

SerialWombatServo servo(sw);

// This example is explained in a video tutorial at: https://youtu.be/R1KM0J2Ug-M

long int servoPosition = 90;
void setup() {
  // put your setup code here, to run once:

  {//I2C Initialization
    Wire.begin();
    sw.begin(Wire,0x6C);  //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C
  }
  
   redButton.begin(0);
   greenButton.begin(1);
   blueButton.begin(3);

   redCounter.begin(&servoPosition, 1,500,2000,5,250,5000,10,100);
   redCounter.lowLimit = 0;
   redCounter.highLimit = 180;

   greenCounter.begin(&servoPosition, -1, 500,2000,-5,250,5000,-10,100);
   greenCounter.lowLimit = 0;
   greenCounter.highLimit = 180;

   servo.attach(2,true);  //Servo on serial wombat pin 2, reverse direction
    
   Serial.begin(115200);
}

void loop() {
  
     redCounter.update();
     greenCounter.update();
    if (blueButton.digitalRead())
    {
      servoPosition = 90;
    }
     servo.write(servoPosition);
    Serial.println(servoPosition);

   
    delay(50);
 
}
