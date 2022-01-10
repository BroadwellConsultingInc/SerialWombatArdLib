#include <SerialWombat.h>


// A video tutorial on this example is available at:
SerialWombat sw;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Wait for initialization

  
  
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println("Serial Wombat chip Finder");
  Serial.println();

   Serial.println();
    Serial.println("=======================================================");
    Serial.println();
  for (int i2cAddress = 0x0E; i2cAddress <= 0x77; ++ i2cAddress)
  {
    Wire.beginTransmission(i2cAddress);
    int error = Wire.endTransmission();

    
    if (error == 0)
    {
      Serial.print("I2C Device found at address 0x");
      Serial.println(i2cAddress,HEX);
      sw.begin(Wire,i2cAddress,false);
      if (sw.queryVersion())
      {
        Serial.println("Serial Wombat chip Found.");
        if (sw.inBoot)
        {
          Serial.println("Serial Wombat chip is in boot mode.");
        }
        Serial.print("Model: ");
        Serial.println((char*)sw.model);
         if (sw.inBoot)
        {
          Serial.print ("Boot ");
        }
        Serial.print("FW Version: ");
        Serial.println((char*)sw.fwVersion);

  
        Serial.print("UniqueID: "); 
        for (int i = 0; i < sw.uniqueIdentifierLength; ++i)
        Serial.printf("%X ",sw.uniqueIdentifier[i]);
        Serial.println();
  
        Serial.print("Microcontroller DeviceId: ");
        Serial.println(sw.deviceIdentifier);
  
        Serial.print("Microcontroller DeviceRevision: ");
        Serial.println(sw.deviceRevision);
        Serial.print("Source voltage (mV):  ");
        Serial.println(sw.readSupplyVoltage_mV());

        if (sw.isSW18())
        {
          Serial.print("Low Accuracy Temperature (deg C):  ");
          uint16_t temperature = sw.readTemperature_100thsDegC();
          Serial.print (temperature / 100);
          Serial.print(".");
          Serial.println(temperature % 100);
        }
      }
      else
      {
        Serial.println("Device did not respond properly to Serial Wombat version information inquiry");
      }
        Serial.println();
        Serial.println();
    }
  }
  delay(30000);
}
