SerialWombatHSClock HSClock(SW18AB_6B);

void hsClockTest()
{

  resetAll();
  HSClock.begin(15,2000);
  initializePulseReaduS(SW18AB_6B,15);
  delay(100);
  uint16_t highTime = pulseRead(SW18AB_6B,15);
  test("HSCLOCK_00", highTime, 250,10);  // Should be a 250 uS high time (500us period)

  resetAll();
  
}
