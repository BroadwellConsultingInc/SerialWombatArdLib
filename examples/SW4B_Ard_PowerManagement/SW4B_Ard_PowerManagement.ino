#include "SerialWombat.h"


// This example is explained in a video tutorial at: https://youtu.be/jVkQ1YoqcpI


SerialWombat SW6C;
SerialWombatPWM pwm(SW6C);
SerialWombatAnalogInput pot(SW6C);
#define PWMPIN 1
#define ANALOGPIN 2
#define POTHIGHPIN 3
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  SW6C.begin(Wire,0x6C);
  pwm.begin(PWMPIN,0x8000);
  pot.begin(ANALOGPIN);
  SW6C.pinMode(POTHIGHPIN,OUTPUT);
  delay(5000);

  Serial.begin(115200); // For debug output.
}

void loop() {
  // put your main code here, to run repeatedly:

  SW6C.digitalWrite(POTHIGHPIN,LOW);  //Turn off POT high-side before going to sleep.
  SW6C.sleep();
  delay(5000);
  
  SW6C.wake();
  SW6C.digitalWrite(POTHIGHPIN,HIGH); //Turn on POT High side
  delay(250); // Give the Serial Wombat chip time to make readings
  Serial.println(pot.readVoltage_mV());
  delay(5000);

}
