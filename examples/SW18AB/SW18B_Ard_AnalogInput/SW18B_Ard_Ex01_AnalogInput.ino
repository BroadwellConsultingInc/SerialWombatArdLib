#include <SerialWombat.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


SerialWombatChip sw;
SerialWombatAnalogInput swAn0(sw);
SerialWombatServo swServo(sw);
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  //Turn off pull ups
  sw.begin(Wire,0x6B);
  swAn0.begin(0);
  swServo.attach(9,1000,15000);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t pot = swAn0.readCounts();
  Serial.println(pot);
  swServo.write16bit(pot);
  delay(100);
}
