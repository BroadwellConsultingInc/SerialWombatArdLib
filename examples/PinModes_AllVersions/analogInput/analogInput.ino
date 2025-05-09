#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat
SerialWombatAnalogInput leftPot(sw);  //5k linear Pot
SerialWombatAnalogInput rightPot(sw); //5k linear Pot      
SerialWombatAnalogInput temperatureSensor(sw);

// This example is explained in a video tutorial at: https://youtu.be/_EKlrEVaEhg


void setup() {
  // put your setup code here, to run once:

    Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port, This SerialWombat's address is 6C.
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial

  leftPot.begin(3);
  rightPot.begin(1);
  temperatureSensor.begin(2,64,65417); // Wombat pin 2, average 64 samples, .5 Hz Low Pass filter.  Change pin 2 to pin 0 for the SW18AB, as pin 3 is the sda pin 
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
