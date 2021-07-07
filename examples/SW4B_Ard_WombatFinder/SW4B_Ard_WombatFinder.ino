#include <SerialWombat.h>


// A video tutorial on this example is available at: https://youtu.be/UZOnq2FdrvU 
SerialWombat sw;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Wait for initialization

  Serial.println("Serial Wombat Finder");
  Serial.println();

  for (int i2cAddress = 0x0E; i2cAddress <= 0x77; ++ i2cAddress)
  {
    Wire.beginTransmission(i2cAddress);
    int error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C Device found at address 0x");
      Serial.println(i2cAddress,HEX);
      sw.begin(Wire,i2cAddress);
      if (sw.queryVersion())
      {
        Serial.println("Serial Wombat Found.");
        Serial.print("Model: ");
        Serial.println((char*)sw.model);
        Serial.print("FW Version: ");
        Serial.println((char*)sw.fwVersion);
        Serial.print("Source voltage (mV):  ");
        Serial.println(sw.readSupplyVoltage_mV());
      }
      else
      {
        Serial.println("Device did not respond properly to Serial Wombat version information inquiry");
      }
        Serial.println();
        Serial.println();
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
