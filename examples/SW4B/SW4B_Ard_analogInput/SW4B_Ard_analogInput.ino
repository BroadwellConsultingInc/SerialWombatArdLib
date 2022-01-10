#include <SerialWombat.h>

SerialWombat sw6C;    //Declare a Serial Wombat
SerialWombatAnalogInput leftPot(sw6C);  //5k linear Pot
SerialWombatAnalogInput rightPot(sw6C); //5k linear Pot      
SerialWombatAnalogInput temperatureSensor(sw6C);

// This example is explained in a video tutorial at: https://youtu.be/_EKlrEVaEhg


void setup() {
  // put your setup code here, to run once:

  { //I2C Initialization
    Wire.begin();
    digitalWrite(A5,LOW);  //Arduino Uno Specific.  Turn off I2C pull up
    digitalWrite(A4,LOW);  //Arduino Uno Specific.  Turn off I2C pull up
    sw6C.begin(Wire, 0x6C); //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C
  }
  leftPot.begin(3);
  rightPot.begin(1);
  temperatureSensor.begin(2,64,65417); // Wombat pin 2, average 64 samples, .5 Hz Low Pass filter
  Serial.begin(115200);
}


void loop() {

  Serial.print("Source V: ");
  uint16_t supplyVoltage = sw6C.readSupplyVoltage_mV();
  Serial.print(supplyVoltage );
  Serial.print("mV      Left Pot: ");
  Serial.print(leftPot.readCounts());
  Serial.print(" ");

  uint16_t leftVoltage = leftPot.readVoltage_mV();
  Serial.print(leftVoltage);
  Serial.print("mV      Right Pot:");
 
  Serial.print(rightPot.readCounts());
  Serial.print(" ");
   uint16_t rightVoltage = rightPot.readVoltage_mV();
   Serial.print(rightVoltage);
   
  Serial.print("mV      T:");

  Serial.print(temperatureSensor.readCounts());
  Serial.print(" ");
  Serial.print(temperatureSensor.readVoltage_mV());
  Serial.print("mV ");

  
  float tempSensor_mV = temperatureSensor.readAveraged_mV();

  //See datasheet for TMP36 Temperature sensor for conversion
   float temperature = (tempSensor_mV - 750) / 10.0 + 25;
  
  Serial.print(temperature);
  Serial.print(" deg C ");

  
  Serial.println();
  delay(200);
}
