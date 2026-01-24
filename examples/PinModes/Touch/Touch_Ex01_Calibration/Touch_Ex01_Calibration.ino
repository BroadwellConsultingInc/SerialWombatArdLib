#include "SerialWombat.h"

/*
This example shows how to configure a Serial Wombat 18AB pin to Touch input and determine working
calibration constants for the touch sensor.

SerialWombat18CapTouch class documentation can be found here:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat18_cap_touch.html#details


A demonstration video of this class can be found here:
https://youtu.be/c4B0_DRVHs0

*/

#define TOUCH_PIN 17      //<<<<<< MODIFY THIS BASED ON WHICH PIN YOUR TOUCH SENSOR IS RUNNING ON
SerialWombatChip sw;
SerialWombat18CapTouch capTouch(sw);

uint16_t lastDigitalRead;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();   //Initialize the I2C Bus on default pins

  Serial.begin(115200);
  delay(3000);
  Serial.println("High Speed Clock Example");


  sw.begin(Wire, sw.find(true));  // Scan the bus for Serial Wombat chips, and initialize the first one found

  //Optional Error handling code begin:
  if (!sw.isSW18())
  {
    Serial.println("This Example is not supported on the Serial Wombat 4B or 8B chip.  An  18AB chip is required.");
    while (1) {
      delay(100);
    }
  }
  if (!sw.isLatestFirmware()) {
    Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");
  }
 
  sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
  //Optional Error handling code end


  //Iterate through increasing amounts of charge until we find a value that 90% saturates the sensor.
  Serial.println("Determining charge time.  Do not touch the sensor.");

  uint16_t noTouchReading = 0;
  uint16_t chargeTime = 250;
    capTouch.begin(TOUCH_PIN,chargeTime,0);
    delay(500);
    noTouchReading = sw.readPublicData(TOUCH_PIN);

  while (noTouchReading < 60000)
  {
    if (noTouchReading < 30000)
    {
      chargeTime += 250;
    }
    else
    {
      chargeTime += 250;
    }
    capTouch.begin(TOUCH_PIN,chargeTime,20);
    delay(500);
    noTouchReading = sw.readPublicData(TOUCH_PIN);
    Serial.print(chargeTime);
    Serial.print(": ");
    Serial.println(noTouchReading);
    
  }
  uint16_t recommendedChargeTime = chargeTime;

  Serial.print("Recommended charge time: ");
  Serial.println(recommendedChargeTime);
  Serial.println();


  // Now take a bunch of samples at that charge to see how much varation you get.  Find the
  // smallest returned value over 5 seconds.
  Serial.println("Calibrating High Limit...");
  uint16_t HighLimit = 65535;
  uint32_t start = millis();
  while (start + 5000 > millis())
  {
    uint16_t result = sw.readPublicData(TOUCH_PIN);

    if (result < HighLimit)
    {
      HighLimit = result;
      Serial.println(HighLimit);

    }
    delay(0);
  }



  Serial.println("Lightly Hold finger on sensor until told to remove...");
  // Wait for the user to touch the sensor  
  while (sw.readPublicData(TOUCH_PIN) > HighLimit - 1500)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println();


  // Now take 5 seconds worth of samples to determine the maximum value you're likely to see
  // while touched.
  uint16_t LowLimit = 0;
  start = millis();
  while (start + 5000 > millis())
  {
    uint16_t result = sw.readPublicData(TOUCH_PIN);

    if (result > LowLimit)
    {
      LowLimit = result;
      Serial.println(LowLimit);
    }
  }

  Serial.println("Remove Finger.");
  Serial.print("Recommended charge time: ");
  Serial.println(recommendedChargeTime);
  Serial.print("Recommended High Limit: ");
  Serial.println (LowLimit + (HighLimit - LowLimit)*3 / 4);   
  Serial.print("Recommended Low Limit: ");
  Serial.println (LowLimit + (HighLimit - LowLimit) / 4);   

  Serial.println("Done.");

  Serial.println();
  Serial.println("Configuring Touch in digital mode using calibrations.  Code is:");
  Serial.println();
  Serial.print(" capTouch.begin(TOUCH_PIN,");Serial.print(recommendedChargeTime);Serial.println(",0);");
  Serial.print("capTouch.makeDigital("); Serial.print(LowLimit);Serial.print(",");Serial.print(HighLimit);Serial.println(",1,0,0,0);");
  Serial.println();

  capTouch.makeDigital(LowLimit,HighLimit,1,0,0,0);
  delay(250);
  lastDigitalRead = sw.readPublicData(TOUCH_PIN);
  
}


//In the loop look for a change in state on the sensor and print a 0 or 1
void loop() {
  static uint8_t count = 0;
  uint16_t newValue = sw.readPublicData(TOUCH_PIN);

  if (newValue != lastDigitalRead)
  {
    Serial.print(newValue); Serial.print(" ");
    lastDigitalRead = newValue;
    ++ count;
    if (count > 20)
    {
      count = 0;
      Serial.println();
    }
  }


}
