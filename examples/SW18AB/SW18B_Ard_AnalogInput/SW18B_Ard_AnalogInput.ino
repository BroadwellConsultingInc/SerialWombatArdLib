#include <SerialWombat.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(128, 64);

SerialWombat SW6C;
SerialWombatAnalogInput swAn0(SW6C);
SerialWombatServo swServo(SW6C);
SerialWombatPulseTimer swPulseTimer(SW6C);
SerialWombatDebouncedInput swDebounced(SW6C);
void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  Wire.begin();
  //Turn off pull ups
  digitalWrite(SCL,LOW);
  digitalWrite(SDA,LOW);

  
  Wire.setClock(100000);
  SW6C.begin(Wire,0x6C,false);
  swAn0.begin(0);
  swServo.attach(9,1000,15000);
  swPulseTimer.begin(10, SW_PULSETIMER_mS, false);
  swDebounced.begin(11,1,false,false);
  Serial.begin(115200);

  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.println("SW4B Unit Test");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t pot = swAn0.readCounts();
  //Serial.println(pot);
  swServo.write16bit(pot);
  swDebounced.readTransitionsState();
  Serial.println(swDebounced.transitions);
  delay(100);
}
