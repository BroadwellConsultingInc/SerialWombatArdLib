#include "SerialWombat.h"

#define I2C_ADDRESS 0x69

SerialWombat sw;
SerialWombatAnalogInput  anWP0(sw);
SerialWombat18CapTouch capTouch(sw);
SerialWombatTM1637 redDisplay(sw);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  sw.begin(Wire,I2C_ADDRESS,false);
  anWP0.begin(0);
  Serial.begin(115200);

  capTouch.begin(16,0x2400,20);
//  capTouch.makeDigital(52000,57000,10000,2000,0,0);
 {
  /*
  uint8_t tx[8] = {200,16,22,0x00,0x24,0x55,0x55};
 
    uint8_t rx[8];
    sw.sendPacket(tx,rx);

    uint8_t tx1[8] = {201,16,22,SW_LE16(52000),SW_LE16(57000),0x55};
    sw.sendPacket(tx1);
    uint8_t tx2[8] = {202,16,22,SW_LE16(10000),SW_LE16(2000),0x55};
    sw.sendPacket(tx2);

    uint8_t tx3[8] = {203,16,22,1,0,SW_LE16(1),0x55};
    sw.sendPacket(tx3);
    */
    
    
 }
 {
  uint8_t tx[8] = {200,17,22,0x00,0x24,0x55,0x55};
 
    uint8_t rx[8];
   // sw.sendPacket(tx,rx);
 }

 redDisplay.begin(19,18,6,tm1637Decimal16,16,4);
  redDisplay.orderDigits(2,1,0,5,4,3);
  
    
}


void loop() {

  {
    Serial.print("Touch 16:");
    Serial.println(sw.readPublicData(16));
    Serial.print("Touch 17:");
    Serial.println(sw.readPublicData(17));
    
    
    Serial.print("WP0:");
    Serial.println(anWP0.readCounts());
    {
      uint8_t tx[8] = {204,16,22,0,0x55,0x55,0x55,0x55};
      uint8_t rx[8];
      sw.sendPacket(tx,rx);
      Serial.print("Reported Level: ");
      Serial.println(rx[3]);
      Serial.print("Transitions: ");
      Serial.println(rx[4] + 256 * rx[5]);
      Serial.print("TimeInState: ");
      Serial.println(rx[6] + 256 * rx[7]);
      
    }
    
    Serial.println();
  }

  delay(1000);
  

}
