SerialWombatHSCounter HSCounter(SW18AB_6B);

void hsCounterTest()
{
SerialWombatPWM_4AB SWPWM19(SW4B_6E);
  resetAll();

{
  
  resetAll();
  SWPWM19.begin(1);
  SWPWM19.setFrequency_SW4AB(SW4AB_PWMFrequency_31250_Hz);
  SWPWM19.writeDutyCycle(0x8000);
  HSCounter.begin(19); 
  delay(1000);
  pulseRead(SW18AB_6B,15);
  test("HSCounter_00", HSCounter.readFrequency(), 31250,500);  
  test("HSCounter_00a", HSCounter.readPublicData(), 31250,500);  


  uint32_t startTime = millis();
  uint32_t counts = HSCounter.readCounts(true);
  counts = HSCounter.readCounts(false);
  while (millis() - startTime < 500);

  delay(0);
  
  uint32_t counts1 = HSCounter.readCounts(false);

  while (millis() - startTime < 1000);
  delay(0);
  uint32_t counts2 = HSCounter.readCounts(false);
   test("HSCounter_01a", counts, 0,500); 
  test("HSCounter_01", counts1, 15625,500);  
  test("HSCounter_02", counts2, 31250,500);  
}
{
  resetAll();
  SWPWM19.begin(1);
  SWPWM19.setFrequency_SW4AB(SW4AB_PWMFrequency_125_Hz);
  SWPWM19.writeDutyCycle(0x8000);
  HSCounter.begin(19,  SerialWombatHSCounter::publicDataOutput::PULSE_COUNT); 
  delay(10000);
  test("HSCounter_10", HSCounter.readPublicData(), 1250,50);  

}

{
  
  resetAll();
  SWPWM19.begin(1);
  SWPWM19.setFrequency_SW4AB(SW4AB_PWMFrequency_125_Hz);
  SWPWM19.writeDutyCycle(0x8000);
  HSCounter.begin(19,  SerialWombatHSCounter::publicDataOutput::PULSE_COUNT,8000); 
  delay(10000);
  test("HSCounter_20", HSCounter.readPublicData(), 1000,50);  

}
{
  resetAll();
  SWPWM19.begin(1);
  SWPWM19.setFrequency_SW4AB(SW4AB_PWMFrequency_31250_Hz);
  SWPWM19.writeDutyCycle(0x8000);
  HSCounter.begin(19,  SerialWombatHSCounter::publicDataOutput::PULSE_COUNT,100,100); 
  delay(10000);
  test("HSCounter_30", HSCounter.readPublicData(), 3125,10);  

}
  resetAll();
  
}
