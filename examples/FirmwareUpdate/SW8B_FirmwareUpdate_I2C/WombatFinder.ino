 void WombatFinder()
 {
 for (int i2cAddress = 0x0E; i2cAddress <= 0x77; ++ i2cAddress)
  {
    Wire.beginTransmission(i2cAddress);
    int error = Wire.endTransmission();

    
    if (error == 0)
    {
      Serial.print("I2C Device found at address 0x");
      Serial.println(i2cAddress,HEX);
      delay(50);
      sw.begin(Wire,i2cAddress,false);
      if (sw.queryVersion())
      {
        Serial.println("Serial Wombat chip Found.");
        delay(50);
        if (sw.inBoot)
        {
          Serial.println("Serial Wombat chip is in boot mode.");
          delay(50);
        }
        Serial.print("Model: ");
        Serial.println((char*)sw.model);
         if (sw.inBoot)
        {
          Serial.print ("Boot ");
          delay(50);
        }
        Serial.print("FW Version: ");
        delay(50);
        Serial.println((char*)sw.fwVersion);
        delay(50);

  
        Serial.print("UniqueID: "); 
        for (int i = 0; i < sw.uniqueIdentifierLength; ++i)
        {
          char s[8];
          sprintf(s,"%X ",sw.uniqueIdentifier[i]);
          Serial.print(s);
        }
        delay(50);
        Serial.println();
  
        Serial.print("Microcontroller DeviceId: ");
        Serial.println(sw.deviceIdentifier);
        delay(50);
        Serial.print("Microcontroller DeviceRevision: ");
        Serial.println(sw.deviceRevision);
        Serial.print("Source voltage (mV):  ");
        delay(50);
        Serial.println(sw.readSupplyVoltage_mV());

        if (sw.isSW18())
        {
          Serial.print("Low Accuracy Temperature (deg C):  ");
          uint16_t temperature = sw.readTemperature_100thsDegC();
          Serial.print (temperature / 100);
          Serial.print(".");
          Serial.println(temperature % 100);
          delay(50);
         Serial.print("Birthday: ");
          Serial.println(sw.readBirthday());
          char brand[33];
          sw.readBrand(brand);
          Serial.print("Brand: ");
          Serial.println(brand);
        }
      }
      else
      {
        Serial.println("Device did not respond properly to Serial Wombat version information inquiry");
        delay(50);
      }
        Serial.println();
        Serial.println();
    }
  }
  }
