int showNotFoundError()
{
          Serial.println("ERROR:");
        Serial.print("No Serial Wombat Chip was detected.");
        Serial.println("Be sure that the address pin configuration you're using is unconnected or tied to ground with a 10k, 20k, or 30k resistor.");
        Serial.println("Ensure the Serial Wombat chip has stable power, and that the SCL and SDA lines are correctly connected.");
        Serial.println("Ensure the Arduino board and Serial Wombat chip share a common ground.");
        Serial.println("Ensure that all required capacitors are in place to support the Serial Wombat chip.");
        Serial.println("Ensure that I2C lines have pull-up resistors.  Consider trying a smaller value (e.g. 1k) resistor.");
        Serial.println("Reset the Arduino board to try again.");

        return(0);
}
