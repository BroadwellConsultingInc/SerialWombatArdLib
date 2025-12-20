SerialWombatDebouncedInput  debouncedInput18(SW18AB_6B);
SerialWombatPWM_4AB DBPWM5(SW4B_6D);
void debounceTest(SerialWombatChip &sw)
{
  SerialWombatDebouncedInput* debouncedInput;
  if (&sw == &SW18AB_6B)
  {
    uint8_t SW18ABpin = 5;
    debouncedInput = &debouncedInput18;
  DBPWM5.begin(SW18ABPinTo4BPin(SW18ABpin));
  DBPWM5.setFrequency_SW4AB(SW4AB_PWMFrequency_125_Hz);
  DBPWM5.writeDutyCycle(0x8000); // 4 ms
  debouncedInput->begin(SW18ABpin,6,false,false);  // 6ms time.  
  delay(500);
  debouncedInput->readTransitionsState();

  if (debouncedInput->transitions < 2)
  {
    pass(1);
  }
  else
  {
    fail(1);
  }

  DBPWM5.setFrequency_SW4AB(SW4AB_PWMFrequency_63_Hz);
  DBPWM5.writeDutyCycle(0x8000); // 8 ms
  delay(500);
  debouncedInput->readTransitionsState();
  if (debouncedInput->transitions < 15)
  {
    fail(1);
  }
  else
  {
    pass(1);
  }

  DBPWM5.begin(SW18ABPinTo4BPin(SW18ABpin));
  DBPWM5.setFrequency_SW4AB(SW4AB_PWMFrequency_125_Hz);
  DBPWM5.writeDutyCycle(0xF000); 
  delay(100);
  if (debouncedInput->readTransitionsState())
  {
    pass(1);
  }
  else
  {
    fail(1);
  }

   DBPWM5.writeDutyCycle(0x1000); 
  delay(100);
  if (debouncedInput->readTransitionsState())
  {
    fail(1);
  }
  else
  {
    pass(1);
  }
  }
  else if (&sw == &SW8B_68)
  {
    test ("Debounce for SW8B Not Implemented. ", 0);
  }
  else if (&sw == &SW4B_6C)
  {
    test ("Debounce for SW4B Not Implemented. ", 0);
  }
}
