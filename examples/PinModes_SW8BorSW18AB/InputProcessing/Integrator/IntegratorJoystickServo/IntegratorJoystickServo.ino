#include "SerialWombat.h"
/*
This example shows how to integrate a joystick or other analog input to control the position 
of a servo by controlling the servo position based on the net result of the joystick position
over time, rather than directly correlating the servo's current position to the joystick's current
position.

This integration function can be applied to any class that inherits from the ProcessedInput class.

Servo is on pin 7.
Joystick / analog is on pin 1.

This example also outputs the current position on a TM1637 Display attached to pins 5 and 6.
A video demonstrating the use of input integration function is avaialble at:
TODO

Documentation for the  class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_processed_input.html

*/

SerialWombatChip sw;
SerialWombatAnalogInput_18AB joystick(sw);
SerialWombatServo_18AB servo(sw);
SerialWombatTM1637 display(sw);
  

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(115200);
  delay(500);

  //Find the Serial Wombat Chip on the I2C bus and display its firmware version
  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError();} // see showErrorNotFound tab
  
  sw.begin(Wire,i2cAddress); 
  sw.queryVersion();
  
  Serial.println();
  Serial.print("Version "); Serial.println((char*)sw.fwVersion);
  versionCheck(); //see showErrorNotFound tab
  Serial.println("SW18AB Found.");

  joystick.begin(1);

  joystick.configureIntegrator (500,//negativeMaxIndex, 
                              0x4000,// negativeMidIndex, 
                              0x7000,// negativeDeadZone
                              0x9000,//positiveDeadZone
                              0xC000,// positiveMidIndex, 
                              65035,//positiveMaxIndex, 
                              5,// midIncrement
                              20,// maxIncrement
                              32768 //initialValue
                              ); 
  joystick.writeProcessedInputEnable(true);


  servo.attach(7);
  servo.writeScalingEnabled(true,1);  //Position the servo based on Pin 19's output

  display.begin(5,//Clk Pin
                6,//Data pin
                6, // 6 digits
                 tm1637Decimal16,//Decimal 16 mode
                 19, // Get data from pin 19
                 4); // Brightness 4
   display.writeDigitOrder(2,1,0,5,4,3);
  
}



void loop() {
  //No action in loop.  Serial Wombat chip is driving the servo internally.  We can monitor the result:
  Serial.println(joystick.readPublicData());

}
