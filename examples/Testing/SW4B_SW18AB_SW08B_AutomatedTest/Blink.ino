void blinkTest(SerialWombatChip &sw, uint8_t blinkPin, uint8_t sourcePin )
{
  resetAll();
	SerialWombatBlink swBlink(sw);
  SerialWombatPin inputPWM(sw,sourcePin);
  inputPWM.pinMode(OUTPUT);
  inputPWM.writePublicData(0);
  swBlink.begin(blinkPin,sourcePin);
  initializePulseReaduS(sw,blinkPin);
  inputPWM.writePublicData(0x8000);
  delay(50);
 test("BLINK_01", pulseCounts(sw,blinkPin), 1);  // 1 change
      test("Blink_02", pulseRead(sw,blinkPin), 30000,10000);  // Should be a 50 mS pulse
  
}
