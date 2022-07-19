#include <SerialWombat.h>


// A video tutorial on this example is available at:
SerialWombatChip sw;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //Initialize I2C
  Serial.begin(115200);  //Initialize Serial
  while (!Serial); // Wait for initialization



}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Serial Wombat chip Finder");
  Serial.println();

  Serial.println();
  Serial.println("=======================================================");
  Serial.println();
  for (int i2cAddress = 0x0E; i2cAddress <= 0x77; ++ i2cAddress)  // Scan through all valid addresses
  {
    Wire.beginTransmission(i2cAddress); // Look for an I2C Ack
    int error = Wire.endTransmission();


    if (error == 0)
    {
      //Got an Ack. Does it behave like a Serial Wombat Chip?
      Serial.print("I2C Device found at address 0x");
      Serial.println(i2cAddress, HEX);
      sw.begin(Wire, i2cAddress, false); //Try to initialize it as a Serial Wombat Chip
      if (sw.queryVersion())  // Does the response to a version query look reasonable?
      {
        //Yes
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
          Serial.printf("%X ", sw.uniqueIdentifier[i]);
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
        //No
        Serial.println("Device did not respond properly to Serial Wombat version information inquiry");
      }
      Serial.println();
      Serial.println();
    }
  }
  delay(30000);  // Wait 30 seconds and do it again.
}
