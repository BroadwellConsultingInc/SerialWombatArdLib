#include <SerialWombat.h>

SerialWombatChip sw;
SerialWombatServo BoringNormalServo(sw);
SerialWombatHighFrequencyServo ExcitingHighFrequencyServo(sw);

void setup() {
  // put your setup code here, to run once:
Wire.begin();

sw.begin(0x6B);

ExcitingHighFrequencyServo.attach(19,500,1000);
ExcitingHighFrequencyServo.writeFrequency_Hz(560);
BoringNormalServo.attach(18);

}

uint16_t position = 0;
void loop() {
  // put your main code here, to run repeatedly:
position += 1000;
ExcitingHighFrequencyServo.write16bit(position);
BoringNormalServo.write16bit(position);
delay(50);
}
