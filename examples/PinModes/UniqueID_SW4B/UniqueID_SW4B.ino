#include <SerialWombat.h>

// See https://youtu.be/IHTcKyXT_2Q for a tutorial on how to use Unique ID

SerialWombatChip sw;
void setup() {
   Serial.begin(115200);
    Wire.begin();
	delay(500);
	uint8_t i2cAddress = sw.find();
    sw.begin(Wire,i2cAddress);  //Initialize the Serial Wombat library to use the primary I2C port
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Version: ");
  Serial.println(sw.readVersion()); // readVersion() must be called prior to using the data below
  
  Serial.print("UniqueIDLength: ");
  Serial.println(sw.uniqueIdentifierLength);
  Serial.print("0x6C UniqueID: "); 
  for (int i = 0; i < sw.uniqueIdentifierLength; ++i)
  Serial.printf("%X ",sw.uniqueIdentifier[i]);
  Serial.println();
  
  Serial.print("0x6C DeviceId: ");
  Serial.println(sw.deviceIdentifier);
  
  Serial.print("0x6C DeviceRevision: ");
  Serial.println(sw.deviceRevision);
  delay(2000);
}
