#include "SerialWombat.h"
/*
This example shows how to integrate a rotary encoder to control the position 
of a servo by controlling the servo position based on the net result of the rotary encoder position
over time, rather than directly correlating the servo's current position to the encoders's current
position.

The encoder will have 5 positions, fast left, slow left, stop, slow right, and fast right

This integration function can be applied to any class that inherits from the ProcessedInput class.

Servo is on pin 15.
The rotary encoder is on pins 5 and 6.

This example also outputs the current position on a TM1637 Display attached to pins 16 and 17.
A video demonstrating the use of input integration function is avaialble at:
TODO

Documentation for the  class is available at:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_abstract_processed_input.html

*/

SerialWombatChip sw;
SerialWombatQuadEnc_18AB rotary(sw);
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

  rotary.begin(5,6);
  rotary.writeMinMaxIncrementTargetPin(0,6);

  rotary.configureIntegrator (0,//negativeMaxIndex, 
                              2,// negativeMidIndex, 
                              3,// negativeDeadZone
                              3,//positiveDeadZone
                              4,// positiveMidIndex, 
                              6,//positiveMaxIndex, 
                              5,// midIncrement 5 counts per ms, 5000 counts per second
                              40,// maxIncrement 20 counts per ms, 20000 counts per second
                              32768 //initialValue
                              ); 
  rotary.writeProcessedInputEnable(true);


  servo.attach(15);
  servo.writeScalingEnabled(true,5);  //Position the servo based on Pin 5's output

  display.begin(16,//Clk Pin
                17,//Data pin
                6, // 6 digits
                 tm1637Decimal16,//Decimal 16 mode
                 5, // Get data from pin 5
                 4); // Brightness 4
   display.writeDigitOrder(2,1,0,5,4,3);
  
}



void loop() {
  //No action in loop.  Serial Wombat chip is driving the servo internally.  We can monitor the result:
  Serial.println(rotary.readPublicData());

}
