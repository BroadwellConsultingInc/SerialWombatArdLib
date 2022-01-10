#include "SerialWombat.h"

// See https://youtu.be/IHTcKyXT_2Q for a tutorial on how to use Unique ID

SerialWombat SW6C;
void setup() {
  // put your setup code here, to run once:
  delay(3000);
  Serial.begin(115200);
  Wire.begin();
  SW6C.begin(Wire,0x6C);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Version: ");
  Serial.println(SW6C.readVersion()); // readVersion() must be called prior to using the data below
  
  Serial.print("UniqueIDLength: ");
  Serial.println(SW6C.uniqueIdentifierLength);
  Serial.print("0x6C UniqueID: "); 
  for (int i = 0; i < SW6C.uniqueIdentifierLength; ++i)
  Serial.printf("%X ",SW6C.uniqueIdentifier[i]);
  Serial.println();
  
  Serial.print("0x6C DeviceId: ");
  Serial.println(SW6C.deviceIdentifier);
  
  Serial.print("0x6C DeviceRevision: ");
  Serial.println(SW6C.deviceRevision);
  delay(2000);
}
