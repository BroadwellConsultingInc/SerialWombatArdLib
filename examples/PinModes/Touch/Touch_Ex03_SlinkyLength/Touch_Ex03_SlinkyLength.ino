#include "SerialWombat.h"


/*
This example shows how to measure the change in a length of a Slinky by measuring the change in its
capacitance as it stretches.  The "Magic Values" in the countsToMoney function
were determined experimentally as shown in the example video.
SerialWombat18CapTouch class documentation can be found here:
https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat18_cap_touch.html#details

A demonstration video of this example can be found here:
https://youtu.be/wHsDJsw18b4

 */



#define SLINKY_PIN 0  //Must be an Analog capable pin:  0,1,2,3,4,16,17,18,19

SerialWombatChip sw;
SerialWombat18CapTouch slinky(sw);


int money = 0;  //Place to keep track of total money count

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

  slinky.begin(SLINKY_PIN,5000,10);  
 
                    slinky.writeAveragingNumberOfSamples(500);
//put this line in setup.  This should be the last of Processed Input Commands for this pin
                    slinky.configureOutputValue(SerialWombatAbstractProcessedInput::OutputValue::AVERAGE);
            slinky.writeProcessedInputEnable(true);


  delay(500);


  money = countsToMoney(slinky.readPublicData());
  Serial.println(money);
  

}

int countsToMoney(uint16_t counts)
{
 
  if (counts > 26788)
  {
    return 0;
  }
  if (counts > 26637)
  {
    return 25;
  }
  if (counts > 26450)
  {
    return 50;
  }
  if (counts > 26280)
  {
    return 75;
  }
    return 100;
}

void loop() {

  int newMoney = countsToMoney(slinky.readPublicData());
  if (newMoney != money)
  {
    money = newMoney;
    Serial.println(money);
  }
}
