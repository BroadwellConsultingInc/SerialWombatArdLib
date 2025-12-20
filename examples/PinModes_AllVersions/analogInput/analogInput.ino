#include <SerialWombat.h>

SerialWombatChip sw;    //Declare a Serial Wombat
SerialWombatAnalogInput leftPot(sw);  //5k linear Pot
SerialWombatAnalogInput rightPot(sw); //5k linear Pot      
SerialWombatAnalogInput temperatureSensor(sw);

// This example is explained in a video tutorial at: https://youtu.be/_EKlrEVaEhg

#define LEFT_POT_PIN 2
#define RIGHT_POT_PIN 1
#define TEMPERATURE_SENSOR_PIN 0 // Set this pin to 3 if using SW4B, as 0 doesn't have Analog

void setup() {
	// put your setup code here, to run once:

	Serial.begin(115200);  
	Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();

	sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port


  // Check for proper Serial Wombat version and pin mode, set error handler.  Not necessary, but useful for debugging problems
	errorChecking(); 
 
	leftPot.begin(LEFT_POT_PIN);
	rightPot.begin(RIGHT_POT_PIN);

	temperatureSensor.begin(TEMPERATURE_SENSOR_PIN,64,65417); //  average 64 samples, .5 Hz Low Pass filter.  
}


void loop() {

  Serial.print("Source V: ");
  uint16_t supplyVoltage = sw.readSupplyVoltage_mV();
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


