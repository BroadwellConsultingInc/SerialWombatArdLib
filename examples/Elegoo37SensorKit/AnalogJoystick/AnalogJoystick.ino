#include <SerialWombat.h>

SerialWombatChip swc;
SerialWombatAnalogInput joyX(swc),joyY(swc);
SerialWombatDebouncedInput button(swc);
SerialWombatServo servoX(swc),servoY(swc);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  swc.begin(Wire,0x6B);
  joyY.begin(1);
  joyX.begin(2);
  servoX.attach(18);
  servoY.attach(19);
  button.begin(0); //Default paramters used, 30mS debounce, inverted, pulled up
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t trueDuration = button.readDurationInTrueState_mS();
  if (trueDuration > 0)
  {
    Serial.println(trueDuration);
  }
  // The following two lines could be eliminated by doing the transfer from the analog to servo internally on
  // the Serial Wombat 18AB chip using scaled output mode on the servos.
  servoY.writePublicData(joyY.readPublicData());
  servoX.writePublicData(joyX.readPublicData());
}
