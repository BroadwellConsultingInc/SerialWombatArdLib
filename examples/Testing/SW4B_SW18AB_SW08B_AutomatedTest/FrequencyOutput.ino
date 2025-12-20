SerialWombatFrequencyOutput_18AB SWFrequencyOutput18(SW18AB_6B),SWFrequencyOutput8(SW8B_68);

void frequencyOutputTest(SerialWombatChip &sw, uint8_t pin)
{
  SerialWombatFrequencyOutput* fo = NULL;
  if (&sw == &SW18AB_6B)
  {  
     fo = &SWFrequencyOutput18;
  }
  else if (&sw == &SW8B_68)
  {
    fo = &SWFrequencyOutput8;
  }

  resetAll();
  fo->begin(pin,2000,0x8000);
  initializePulseReaduS(sw,pin);
  delay(100);
  uint16_t highTime = pulseRead(sw,pin);
  test("FreqOutput_00", highTime, 250,10,2);  // Should be a 250 uS high time (500us period)

  fo->writePublicData(1000); // 1000 Hz
  delay(100);
  highTime = pulseRead(sw,pin);
  test("FreqOutput_01", highTime, 500,10,2);  // Should be a 500 uS high time (1000us period)

  
   resetAll();
  fo->begin(pin,14400,0x8000);
  fo->writePublicData(14400); // 25000 Hz
  initializePulseReaduS(sw,pin);
  delay(100);
  highTime = pulseRead(sw,pin);
  test("FreqOutput_02", highTime, 34,5,2);  // Should be a 20 uS high time (68us period)

  resetAll();
  fo->begin(pin,14400,0xC000);  // 75% duty cycle
  fo->writePublicData(14400); // 25000 Hz
  initializePulseReaduS(sw,pin);
  delay(100);
  highTime = pulseRead(sw,pin);
  test("FreqOutput_03", highTime, 51,5,2);  // Should be a 30 uS high time (68uS period)
}
