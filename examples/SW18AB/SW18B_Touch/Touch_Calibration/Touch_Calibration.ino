#include "SerialWombat.h"

/*
This example shows how to configure a Serial Wombat 18AB pin to Touch input and determine working
calibration constants for the touch sensor.

SerialWombat18CapTouch class documentation can be found here:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat18_cap_touch.html#details


NOTE!   VERSION 2.0.5 of the SW18AB firmware has a bug which prevents configuration of digital mode for the
Touch input.  You must upgrade if you have version 2.0.5 and want to use this feature.


A demonstration video of this class can be found here:
https://youtu.be/c4B0_DRVHs0

*/

#define TOUCH_PIN 17      //<<<<<< MODIFY THIS BASED ON WHICH PIN YOUR TOUCH SENSOR IS RUNNING ON
SerialWombatChip sw;
SerialWombat18CapTouch capTouch(sw);

uint16_t lastDigitalRead;
void setup() {
	delay(5000);
  // Set up the Serial port and I2C
  Wire.begin();
  Serial.begin(115200);


  //Find and initialize the Serial Wombat chip
  uint8_t i2cAddress = sw.find();
  if (i2cAddress == 0) {showNotFoundError(); while(1){delay(100);}}
  sw.begin(Wire,i2cAddress,false);

  
  delay(1000);

   sw.queryVersion();

   if (sw.fwVersion[0] == '2' && sw.fwVersion[1] == '0' && sw.fwVersion[2] == '5')
   {
	   Serial.println("Firmware Version 2.0.5 detected.  This version has a bug which prevents transition from analog to digital touch reporting.  Update firmware to latest version (Tutorial video: https://youtu.be/q7ls-lMaL80  )");
	   while (1)
	   {
		   delay(100);
	   }
   }

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
