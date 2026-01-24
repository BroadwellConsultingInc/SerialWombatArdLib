#include "SerialWombat.h"
/*
This example shows how to integrate a rotary encoder to control the position 
of a servo by controlling the servo position based on the net result of the rotary encoder position
over time, rather than directly correlating the servo's current position to the encoders's current
position.

The encoder will have 5 positions, fast left, slow left, stop, slow right, and fast right

This integration function can be applied to any class that inherits from the ProcessedInput class.

Servo is on pin 1.
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
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("Integrator Quadrature Encoder Servo Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (sw.isSW04())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B chip.  An 8B or 18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
  if (sw.isSW08() && !(sw.isPinModeSupported(PIN_MODE_QUADRATUREENCODER) && sw.isPinModeSupported(PIN_MODE_SERVO)) )
  {
    Serial.println("The required pin mode does not appear to be supported in this firmware build.  Do you need to download a different firmware?");
    while (1) {
      delay(100);
    }
  }
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end

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


  servo.attach(1);
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
