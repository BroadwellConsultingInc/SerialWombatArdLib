#include <SerialWombat.h>



PCB0048_Mux swMux;
SerialWombatAnalogInput_18AB swAnalog(swMux);
#define MUX_I2C_ADDRESS 0x60

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //Initialize I2C
  Serial.begin(115200);  //Initialize Serial
  while (!Serial); // Wait for initialization
  delay(200);
  swMux.begin(MUX_I2C_ADDRESS);

  swAnalog.begin(6); // Set up an analog input on pin 6, just to show how to use other pins for Serial Wombat functions.
  


}
void scanI2C(){

  for (int i2cAddress = 0x0E; i2cAddress <= 0x77; ++ i2cAddress)  // Scan through all valid addresses
  {
    Wire.beginTransmission(i2cAddress); // Look for an I2C Ack
    int error = Wire.endTransmission();


    if (error == 0)
    {
      //Got an Ack. Does it behave like a Serial Wombat Chip?
      Serial.print("I2C Device found at address 0x");
      Serial.println(i2cAddress, HEX);
      
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("All bus sections disabled:");
  swMux.bus1.writePublicData(0);
  swMux.bus2.writePublicData(0);
  swMux.bus3.writePublicData(0);
  swMux.bus7.writePublicData(0);
  scanI2C();

  Serial.println();
  Serial.println();
   Serial.println("Bus section 1:");
  swMux.bus1.writePublicData(0xFFFF);
  swMux.bus2.writePublicData(0);
  swMux.bus3.writePublicData(0);
  swMux.bus7.writePublicData(0);
  scanI2C();


 Serial.println();
  Serial.println();
   Serial.println("Bus section 2:");
  swMux.bus1.writePublicData(0);
  swMux.bus2.writePublicData(0xFFFF);
  swMux.bus3.writePublicData(0);
  swMux.bus7.writePublicData(0);
  scanI2C();


 Serial.println();
  Serial.println();
   Serial.println("Bus section 3:");
  swMux.bus1.writePublicData(0);
  swMux.bus2.writePublicData(0);
  swMux.bus3.writePublicData(0xFFFF);
  swMux.bus7.writePublicData(0);
  scanI2C();

  Serial.println();
  Serial.println();
   Serial.println("Bus section 7:");
  swMux.bus1.writePublicData(0);
  swMux.bus2.writePublicData(0);
  swMux.bus3.writePublicData(0);
  swMux.bus7.writePublicData(0xFFFF);
  scanI2C();

  Serial.println();
  Serial.println();
  delay(10000);  // Wait 10 seconds and do it again.
}
