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


#define I2C_ADDRESS 0x6B
#define SLINKY_PIN 0  //Must be an Analog capable pin:  0,1,2,3,4,16,17,18,19

SerialWombatChip sw;
SerialWombat18CapTouch slinky(sw);


int money = 0;  //Place to keep track of total money count

void setup() {
  Wire.begin();  // Initialize I2C
  
  sw.begin(Wire,I2C_ADDRESS,false); //Initialize the Serial Wombat Chip
  
  Serial.begin(115200); //Initialize the UART

  delay(1000);
  
  VersionCheck();  //Check to ensure the Serial Wombat chip is responding (see other tab)

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
