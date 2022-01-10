int VersionCheck()
{
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
        return(1);
      }

      else

      {
        char addr[10];
        sprintf(addr,"0x%X",I2C_ADDRESS);
        Serial.println();
        Serial.println();
        Serial.println("ERROR:");
        Serial.print("No Serial Wombat Chip was detected at address ");
        Serial.println(addr);
        Serial.println("Be sure to set #define I2C_ADDRESS to the address for the address pin configuration you're using.");
        Serial.println("Ensure the Serial Wombat chip has stable power, and that the SCL and SDA lines are correctly connected.");
        Serial.println("Ensure the Arduino board and Serial Wombat chip share a common ground.");
        Serial.println("Ensure that all required capacitors are in place to support the Serial Wombat chip.");
        Serial.println("Ensure that I2C lines have pull-up resistors.  Consider trying a smaller value (e.g. 1k) resistor.");
        
      }
      while (1)
      {
        delay(100);
      }    
      return(-1);
}
