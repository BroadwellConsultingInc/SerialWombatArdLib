void showNotFoundError()
{
          Serial.println("ERROR:");
        Serial.print("No Serial Wombat Chip was detected.");
        Serial.println("Be sure that the address pin configuration you're using is unconnected or tied to ground with a 10k, 20k, or 30k resistor.");
        Serial.println("Ensure the Serial Wombat chip has stable power, and that the SCL and SDA lines are correctly connected.");
        Serial.println("Ensure the Arduino board and Serial Wombat chip share a common ground.");
        Serial.println("Ensure that all required capacitors are in place to support the Serial Wombat chip.");
        Serial.println("Ensure that I2C lines have pull-up resistors.  Consider trying a smaller value (e.g. 1k) resistor.");
        Serial.println("Reset the Arduino board to try again.");

        while(1)
        {
          delay(100);
        }
}

void versionCheck()
{
  sw.readVersion();
  if (!sw.isSW18())
  {
    Serial.println("ERROR: This chip does not appear to be a Serial Wombat 18AB chip"); 
    Serial.print((char)sw.model[0]);
    Serial.print((char)sw.model[1]);
    Serial.print((char)sw.model[2]);
    while (1) delay(100);
    
  }
  if (!sw.isLatestFirmware())
  {
    Serial.print("ERROR: Version ");
	Serial.print(SW18AB_LATEST_FIRMWARE);
      Serial.println(" or higher of the firmware is required to use this example");
    while (1) delay(100);
  }

}
